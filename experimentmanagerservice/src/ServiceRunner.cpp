#include "ServiceRunner.h"
#include "src/BusboardV1.h"
#include "src/common/CellVisualsHistory.h"
#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QString>
#include <chrono>
#include <cmath>
#include <optional>
#include <algorithm>
#include <cctype>
#include <thread>
#include <QtConcurrent/QtConcurrent>

#define MIN_CELL_UPDATE_INTERVAL_MSECS 100
#define TEMP_EPSILON 0.5
constexpr unsigned long long kTargetSyncRetryIntervalMs = 1000;
constexpr unsigned long long kTargetSyncTimeoutMs = 3000;
// Change this to adjust how far ahead we compute the future target temperature.
constexpr unsigned long long kFutureTargetLeadMs = 60ULL * 1000ULL;

namespace {
constexpr const char *kStateUnplugged = "UNPLUGGED";
constexpr const char *kStateIdle = "IDLE";
constexpr const char *kStatePreheat = "PREHEAT";
constexpr const char *kStateRunning = "RUNNING";
constexpr const char *kStateCompleted = "COMPLETED";
constexpr unsigned int kMaxMissedMachineStatus = 15;
constexpr unsigned long long kMachineStatusCheckIntervalMs = 500;

struct BusboardMeta {
    std::string machineId;
    std::string side;
};

std::optional<BusboardMeta> parseBusboardMeta(const std::string &busboardId)
{
    if (busboardId.empty()) {
        return std::nullopt;
    }

    std::string upper = busboardId;
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });

    std::string side;
    if (upper.find("LHS") != std::string::npos) {
        side = "LHS";
    } else if (upper.find("RHS") != std::string::npos) {
        side = "RHS";
    } else {
        return std::nullopt;
    }

    std::string digits;
    for (char c : upper) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        }
    }
    if (digits.empty()) {
        return std::nullopt;
    }
    if (digits.size() < 3) {
        digits.insert(digits.begin(), 3 - digits.size(), '0');
    }

    return BusboardMeta{digits, side};
}

unsigned long long totalExperimentDurationMs(Experiment& experiment)
{
    unsigned long long totalDurationMs = 0;
    const auto &tempArcs = experiment.profile().tempArcsInSeq();
    for (const auto &arc : tempArcs) {
        totalDurationMs += arc.durationMSec();
    }
    return totalDurationMs;
}

unsigned long long clampElapsedMs(unsigned long long elapsedMs, unsigned long long totalDurationMs)
{
    if (totalDurationMs > 0 && elapsedMs > totalDurationMs) {
        return totalDurationMs;
    }
    return elapsedMs;
}

bool hasExperimentAssigned(Experiment& experiment)
{
    return !(experiment.experimentId().empty() && experiment.name().empty());
}

std::string determineExperimentState(Cell &cell, Experiment &experiment, unsigned long long nowMs)
{
    if (!cell.isPlugged() || cell.cellID().empty()) {
        return kStateUnplugged;
    }

    if (!hasExperimentAssigned(experiment)) {
        return kStateIdle;
    }

    unsigned long long totalDurationMs = totalExperimentDurationMs(experiment);
    unsigned long long startMs = experiment.startSystemTimeMSecs();
    if (startMs == 0 || totalDurationMs == 0) {
        return kStatePreheat;
    }

    unsigned long long elapsedMs = nowMs > startMs ? nowMs - startMs : 0;
    if (elapsedMs < totalDurationMs) {
        return kStateRunning;
    }
    return kStateCompleted;
}

bool updateExperimentStartIfReady(Experiment &experiment, const Cell &cell, unsigned long long nowMs)
{
    if (experiment.startSystemTimeMSecs() > 0) {
        return true;
    }

    const auto &tempArcs = experiment.profile().tempArcsInSeq();
    if (tempArcs.empty()) {
        return false;
    }

    float startTemp = tempArcs.front().startTemp();
    float currentTemp = cell.currentTempExt();
    if (!std::isfinite(currentTemp)) {
        currentTemp = cell.currentTempInner();
    }
    if (std::abs(currentTemp - startTemp) <= TEMP_EPSILON) {
        experiment.setStartSystemTimeMSecs(nowMs);
        return true;
    }
    return false;
}

double preheatTargetTemp(Experiment &experiment)
{
    const auto &tempArcs = experiment.profile().tempArcsInSeq();
    if (tempArcs.empty()) {
        return 0.0;
    }
    return tempArcs.front().startTemp();
}

void calculateExperimentTargets(Experiment &experiment,
                                unsigned long long elapsedMs,
                                double *targetTemp,
                                int *targetRpm)
{
    *targetTemp = experiment.profile().calculateTemp(elapsedMs);
    *targetRpm = experiment.profile().calculateRPM(elapsedMs);
}
} // namespace


ServiceRunner::ServiceRunner(const std::string &redisHost, int redisPort)
    : m_redisHost(redisHost)
    , m_redisPort(redisPort)
{
}

bool ServiceRunner::initService()
{
    bool succ = false;
    m_busboards.clear();
    std::shared_ptr<IBusboard> lhsBusboard;
    std::shared_ptr<IBusboard> rhsBusboard;
    std::string lhsMachineId;
    std::string rhsMachineId;

    int portCount = QSerialPortInfo::availablePorts().size();
    for (int i = 0; i < portCount; i++) {
        std::shared_ptr<BusboardV1> busboard = std::make_shared<BusboardV1>();
        if (!busboard->connectBoard()) {
            continue;
        }

        std::string busboardId = busboard->busboardID();
        auto metaOpt = parseBusboardMeta(busboardId);
        if (!metaOpt.has_value()) {
            qWarning() << "Unknown busboard id, ignoring:" << QString::fromStdString(busboardId);
            continue;
        }

        const BusboardMeta &meta = metaOpt.value();
        if (meta.side == "LHS") {
            if (lhsBusboard) {
                continue;
            }
            lhsBusboard = busboard;
            lhsMachineId = meta.machineId;
            if (rhsBusboard && !rhsMachineId.empty() && rhsMachineId != lhsMachineId) {
                qWarning() << "Machine ID mismatch. Ignoring RHS busboard"
                           << QString::fromStdString(rhsBusboard->busboardID())
                           << "for LHS"
                           << QString::fromStdString(busboardId);
                rhsBusboard.reset();
                rhsMachineId.clear();
            }
        } else if (meta.side == "RHS") {
            if (rhsBusboard) {
                continue;
            }
            if (lhsBusboard && !lhsMachineId.empty() && lhsMachineId != meta.machineId) {
                qWarning() << "Machine ID mismatch. Ignoring RHS busboard"
                           << QString::fromStdString(busboardId)
                           << "for LHS"
                           << QString::fromStdString(lhsBusboard->busboardID());
                continue;
            }
            rhsBusboard = busboard;
            rhsMachineId = meta.machineId;
        }

        if (lhsBusboard && rhsBusboard) {
            break;
        }
    }

    if (lhsBusboard) {
        m_busboards.push_back(lhsBusboard);
    }
    if (rhsBusboard) {
        m_busboards.push_back(rhsBusboard);
    }
    succ = !m_busboards.empty();
    qInfo() << "initService busboards:" << m_busboards.size() << "succ:" << succ;

    bool succDB = false;

    if(RedisDBManager::getInstance()->isConnected()){
        qInfo() << "initService redis already connected";
        return succ;
    }
    succDB = RedisDBManager::getInstance()->connectToDB(m_redisHost, m_redisPort);
    qInfo() << "initService redis connect result:" << succDB;
    return succ && succDB;
}

void ServiceRunner::startServiceLoop()
{
    while(true){
        bool anyHealthy = false;
        for (const auto& busboard : m_busboards) {
            if (!busboard->checkHealth()) {
                qWarning() << "busboard health check failed:" << QString::fromStdString(busboard->busboardID());
                continue;
            }
            anyHealthy = true;
            processBusboard(busboard);
        }

        if (!anyHealthy) {
            qWarning() << "no healthy busboards; exiting service loop";
            break;
        }
    }
}

void ServiceRunner::processBusboard(const std::shared_ptr<IBusboard>& busboard)
{
    QCoreApplication::processEvents();

    const std::string busboardId = busboard->busboardID();
    if (busboardId.empty()) {
        return;
    }

    unsigned long long nowMs = Cell::getCurrentTimeMillis();
    unsigned int statusSeq = busboard->machineStatusSequence();
    bool hasNewMachineStatus = false;

    auto &lastSeq = m_lastMachineStatusSeq[busboardId];
    auto &missedCount = m_missedMachineStatus[busboardId];
    auto &lastCheckMs = m_lastMachineStatusCheckMs[busboardId];

    if (statusSeq > 0 && statusSeq != lastSeq) {
        lastSeq = statusSeq;
        missedCount = 0;
        lastCheckMs = nowMs;
        hasNewMachineStatus = true;
    } else if (lastCheckMs == 0) {
        lastCheckMs = nowMs;
    } else if (nowMs - lastCheckMs >= kMachineStatusCheckIntervalMs) {
        missedCount++;
        lastCheckMs = nowMs;
    }

    if (missedCount >= kMaxMissedMachineStatus) {
        const auto &cells = busboard->getCellArray();
        for (const auto &cell : cells) {
            if (!cell.cellID().empty()) {
                RedisDBManager::getInstance()->deleteCell(cell.cellID());
            }
        }
        RedisDBManager::getInstance()->removeBusboardFromMachine(busboardId);
        return;
    }

    std::vector<Cell> cellsFromBoard = busboard->getCellArray();
    std::vector<std::string> cellIdsBySlot;
    std::vector<std::string> activeCellIDs;
    cellIdsBySlot.reserve(cellsFromBoard.size());
    activeCellIDs.reserve(cellsFromBoard.size());

    for (const auto &cell : cellsFromBoard) {
        if (cell.isPlugged() && !cell.cellID().empty()) {
            activeCellIDs.push_back(cell.cellID());
            cellIdsBySlot.push_back(cell.cellID());
        } else {
            cellIdsBySlot.push_back("");
            if (!cell.isPlugged() && !cell.cellID().empty()) {
                RedisDBManager::getInstance()->deleteCell(cell.cellID());
            }
        }
    }

    if (hasNewMachineStatus) {
        std::vector<int> slotStates;
        slotStates.reserve(cellsFromBoard.size());
        for (const auto &cell : cellsFromBoard) {
            slotStates.push_back(cell.isPlugged() ? 1 : 0);
        }
        RedisDBManager::getInstance()->pushMachineStatus(busboardId, slotStates);
        QCoreApplication::processEvents();
    }

    std::vector<Cell> cellsFromDB = RedisDBManager::getInstance()->getCellList(activeCellIDs);

    std::map<std::string, Cell> boardCellMap;
    for(int i = 0; i < cellsFromBoard.size(); i++){
        if(cellsFromBoard.at(i).cellID().empty()){
            continue;
        }
        boardCellMap[cellsFromBoard.at(i).cellID()] = cellsFromBoard.at(i);
    }
    QCoreApplication::processEvents();

    std::map<std::string, Cell> dbCellMap;
    for(int i = 0; i < cellsFromDB.size(); i++){
        if(cellsFromDB.at(i).cellID().empty()){
            continue;
        }
        dbCellMap[cellsFromDB.at(i).cellID()] = cellsFromDB.at(i);
    }
    QCoreApplication::processEvents();

    for(int i = 0; i < activeCellIDs.size(); i++){
        std::string cellID = activeCellIDs.at(i);
        if(cellID.empty()){
            continue;
        }
        dbCellMap[cellID].updateBoardRelatedAttributes(boardCellMap[cellID]);
    }
    QCoreApplication::processEvents();


    for(int i = 0; i < activeCellIDs.size(); i++){
        std::string cellID = activeCellIDs.at(i);

        if(cellID.empty()){
            continue;
        }

        Cell &dbCell = dbCellMap[cellID];
        if (dbCell.cellID().empty()) {
            dbCell.setCellID(cellID);
        }

        Experiment experiment = dbCell.asignedExperiment();
        bool hasExperiment = hasExperimentAssigned(experiment);
        if (hasExperiment) {
            const auto &tempArcs = experiment.profile().tempArcsInSeq();
            if (tempArcs.empty()) {
                experiment.setState(determineExperimentState(dbCell, experiment, nowMs));
                dbCell.setAsignedExperiment(experiment);
                dbCell.setTargetSyncState(Cell::TargetSyncUnknown);
                m_targetSync.erase(cellID);
                continue;
            }

            bool startReady = updateExperimentStartIfReady(experiment, dbCell, nowMs);
            double targetTemp = preheatTargetTemp(experiment);
            double targetTempFuture = targetTemp;
            int targetRpm = 0;
            unsigned long long totalDurationMs = totalExperimentDurationMs(experiment);
            unsigned long long startMs = experiment.startSystemTimeMSecs();
            unsigned long long elapsedMs = startMs > 0 && nowMs > startMs ? nowMs - startMs : 0;

            std::string experimentState = determineExperimentState(dbCell, experiment, nowMs);
            if (startReady && startMs > 0
                && (experimentState == kStateRunning || experimentState == kStateCompleted)) {
                unsigned long long effectiveElapsed = clampElapsedMs(elapsedMs, totalDurationMs);
                calculateExperimentTargets(experiment, effectiveElapsed, &targetTemp, &targetRpm);
                unsigned long long futureElapsed = clampElapsedMs(effectiveElapsed + kFutureTargetLeadMs, totalDurationMs);
                targetTempFuture = experiment.profile().calculateTemp(futureElapsed);
            }

            unsigned int motorSelect = 1;
            bool motorSelectChanged = false;
            auto motorSelectIt = m_lastMotorSelect.find(cellID);
            if (motorSelectIt == m_lastMotorSelect.end() || motorSelectIt->second != motorSelect) {
                motorSelectChanged = true;
            }

            auto boardIt = boardCellMap.find(cellID);
            if (boardIt == boardCellMap.end()) {
                experiment.setState(experimentState);
                dbCell.setAsignedExperiment(experiment);
                dbCell.setTargetSyncState(Cell::TargetSyncUnknown);
                continue;
            }
            Cell &boardCell = boardIt->second;

            TargetSyncInfo &sync = m_targetSync[cellID];
            if (sync.experimentId != experiment.experimentId()) {
                sync = TargetSyncInfo{};
                sync.experimentId = experiment.experimentId();
            }

            bool targetMatchLive = std::abs(targetTemp - boardCell.assignedTemp()) <= TEMP_EPSILON
                && targetRpm == static_cast<int>(boardCell.assignedRPM());
            bool desiredChanged = !sync.pending
                || std::abs(sync.desiredTemp - targetTemp) > TEMP_EPSILON
                || std::abs(sync.desiredTempFuture - targetTempFuture) > TEMP_EPSILON
                || sync.desiredRpm != targetRpm
                || sync.motorSelect != motorSelect;

            bool shouldSend = motorSelectChanged || !targetMatchLive || desiredChanged;
            if (shouldSend) {
                bool canSend = desiredChanged
                    || sync.lastSendMs == 0
                    || (nowMs - sync.lastSendMs >= kTargetSyncRetryIntervalMs);
                if (canSend) {
                    std::string updateString = boardCell.generateUpdateDataStringToBoard(static_cast<float>(targetTemp),
                                                                                         static_cast<float>(targetTempFuture),
                                                                                         targetRpm,
                                                                                         motorSelect);
                    QCoreApplication::processEvents();

                    QString str = QString::fromStdString(updateString);
                    busboard->sendUpdateString(str);
                    m_lastMotorSelect[cellID] = motorSelect;

                    if (desiredChanged || !sync.pending) {
                        sync.firstSendMs = nowMs;
                        sync.attempt = 0;
                    }
                    sync.desiredTemp = targetTemp;
                    sync.desiredTempFuture = targetTempFuture;
                    sync.desiredRpm = targetRpm;
                    sync.motorSelect = motorSelect;
                    sync.lastSendMs = nowMs;
                    sync.pending = true;
                    sync.attempt++;
                }
            }

            int syncState = Cell::TargetSyncUnknown;
            if (sync.pending) {
                bool matchLastSent = std::abs(boardCell.assignedTemp() - sync.desiredTemp) <= TEMP_EPSILON
                    && static_cast<int>(boardCell.assignedRPM()) == sync.desiredRpm;
                if (matchLastSent) {
                    sync.pending = false;
                    sync.firstSendMs = 0;
                    sync.attempt = 0;
                    syncState = Cell::TargetSyncSynced;
                } else if (sync.firstSendMs > 0 && nowMs - sync.firstSendMs >= kTargetSyncTimeoutMs) {
                    syncState = Cell::TargetSyncFailed;
                } else {
                    syncState = Cell::TargetSyncPending;
                }
            } else {
                syncState = targetMatchLive ? Cell::TargetSyncSynced : Cell::TargetSyncPending;
            }

            dbCell.setTargetSyncState(syncState);

            experiment.setState(experimentState);
            dbCell.setAsignedExperiment(experiment);
            QCoreApplication::processEvents();
            continue;
        }

        m_targetSync.erase(cellID);
        dbCell.setTargetSyncState(Cell::TargetSyncUnknown);
        experiment.setState(determineExperimentState(dbCell, experiment, nowMs));
        dbCell.setAsignedExperiment(experiment);
    }

    std::vector<Cell> cellsToDB;
    for (const auto& pair : dbCellMap) {
        cellsToDB.push_back(pair.second);
    }
    RedisDBManager::getInstance()->pushCellList(cellsToDB);
    QCoreApplication::processEvents();

    for (auto& cell : cellsToDB) {
        const std::string& cellID = cell.cellID();
        if (cellID.empty()) {
            continue;
        }

        Experiment experiment = cell.asignedExperiment();
        if (!hasExperimentAssigned(experiment)) {
            m_lastExperimentId.erase(cellID);
            m_lastVisualTimestamp.erase(cellID);
            continue;
        }

        if (experiment.state() != kStateRunning) {
            continue;
        }

        const std::string& experimentId = experiment.experimentId();
        auto lastExpIt = m_lastExperimentId.find(cellID);
        if (lastExpIt == m_lastExperimentId.end() || lastExpIt->second != experimentId) {
            m_lastExperimentId[cellID] = experimentId;
            m_lastVisualTimestamp.erase(cellID);
            RedisDBManager::getInstance()->pushCellVisuals(cellID, CellVisualsHistory());
        }

        unsigned long long nowMs = Cell::getCurrentTimeMillis();
        unsigned long long totalDurationMs = totalExperimentDurationMs(experiment);
        unsigned long long startMs = experiment.startSystemTimeMSecs();
        if (startMs > 0 && totalDurationMs > 0) {
            unsigned long long elapsedMs = nowMs > startMs ? nowMs - startMs : 0;
            if (elapsedMs >= totalDurationMs) {
                continue;
            }
        }

        unsigned long long lastTimestamp = 0;
        auto lastTimestampIt = m_lastVisualTimestamp.find(cellID);
        if (lastTimestampIt != m_lastVisualTimestamp.end()) {
            lastTimestamp = lastTimestampIt->second;
        }
        if (lastTimestamp != 0 && nowMs - lastTimestamp < MIN_CELL_UPDATE_INTERVAL_MSECS) {
            continue;
        }

        CellVisualsHistory history;
        history.addCellVisuals(cell.toCellVisuals());
        RedisDBManager::getInstance()->pushCellVisuals(cellID, history);
        m_lastVisualTimestamp[cellID] = nowMs;
    }

    RedisDBManager::getInstance()->pushFlowStatus(busboardId, busboard->flowStatus());
    QCoreApplication::processEvents();

    RedisDBManager::getInstance()->pushBusboardCellIds(busboardId, cellIdsBySlot);
    QCoreApplication::processEvents();
}
