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

namespace {
constexpr const char *kStateUnplugged = "UNPLUGGED";
constexpr const char *kStateIdle = "IDLE";
constexpr const char *kStatePreheat = "PREHEAT";
constexpr const char *kStateRunning = "RUNNING";
constexpr const char *kStateCompleted = "COMPLETED";
constexpr unsigned int kMaxMissedMachineStatus = 5;
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
    float currentTemp = cell.isExtTempPlugged() ? cell.currentTempExt() : cell.currentTempInner();
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

    bool succDB = false;

    if(RedisDBManager::getInstance()->isConnected()){
        return succ;
    }
    succDB = RedisDBManager::getInstance()->connectToDB(m_redisHost, m_redisPort);
    return succ && succDB;
}

void ServiceRunner::startServiceLoop()
{
    while(true){
        bool anyHealthy = false;
        for (const auto& busboard : m_busboards) {
            if (!busboard->checkHealth()) {
                continue;
            }
            anyHealthy = true;
            processBusboard(busboard);
        }

        if (!anyHealthy) {
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
                continue;
            }

            bool startReady = updateExperimentStartIfReady(experiment, dbCell, nowMs);
            double targetTemp = preheatTargetTemp(experiment);
            int targetRpm = 0;
            unsigned long long totalDurationMs = totalExperimentDurationMs(experiment);
            unsigned long long startMs = experiment.startSystemTimeMSecs();
            unsigned long long elapsedMs = startMs > 0 && nowMs > startMs ? nowMs - startMs : 0;

            std::string experimentState = determineExperimentState(dbCell, experiment, nowMs);
            if (startReady && startMs > 0
                && (experimentState == kStateRunning || experimentState == kStateCompleted)) {
                unsigned long long effectiveElapsed = elapsedMs;
                if (totalDurationMs > 0 && effectiveElapsed > totalDurationMs) {
                    effectiveElapsed = totalDurationMs;
                }
                calculateExperimentTargets(experiment, effectiveElapsed, &targetTemp, &targetRpm);
            }

            dbCell.setAssignedRPM(targetRpm);
            dbCell.setAssignedTemp(targetTemp);

            unsigned int motorSelect = 0;
            bool motorSelectChanged = false;
            auto motorSelectIt = m_lastMotorSelect.find(cellID);
            if (motorSelectIt == m_lastMotorSelect.end() || motorSelectIt->second != motorSelect) {
                motorSelectChanged = true;
            }

            experiment.setState(experimentState);
            dbCell.setAsignedExperiment(experiment);

            if(targetRpm != boardCellMap[cellID].assignedRPM()
                || abs(targetTemp - boardCellMap[cellID].assignedTemp()) > TEMP_EPSILON
                || motorSelectChanged){
                std::string updateString = boardCellMap[cellID].generateUpdateDataStringToBoard(targetTemp, targetRpm, motorSelect);
                QCoreApplication::processEvents();

                QString str = QString::fromStdString(updateString);
                busboard->sendUpdateString(str);
                m_lastMotorSelect[cellID] = motorSelect;
            }
            QCoreApplication::processEvents();
            continue;
        }

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
