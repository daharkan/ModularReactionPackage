#include "ServiceRunner.h"
#include "src/BusboardV1.h"
#include "src/common/CellTarget.h"
#include "src/common/CellVisualsHistory.h"
#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QString>
#include <chrono>
#include <cmath>
#include <thread>
#include <QtConcurrent/QtConcurrent>

#define HOSTNAME "127.0.0.1"
#define PORT 6379

#define MIN_CELL_UPDATE_INTERVAL_MSECS 100
#define TEMP_EPSILON 0.5

namespace {
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


ServiceRunner::ServiceRunner()
{
}

bool ServiceRunner::initService()
{
    bool succ = false;
    m_busboards.clear();
    bool hasLeft = false;
    bool hasRight = false;

    int portCount = QSerialPortInfo::availablePorts().size();
    for (int i = 0; i < portCount; i++) {
        std::shared_ptr<BusboardV1> busboard = std::make_shared<BusboardV1>();
        if (!busboard->connectBoard()) {
            continue;
        }

        std::string busboardId = busboard->busboardID();
        QString idUpper = QString::fromStdString(busboardId).toUpper();
        bool isLeft = idUpper.contains("LHS");
        bool isRight = idUpper.contains("RHS");

        if (isLeft && hasLeft) {
            continue;
        }
        if (isRight && hasRight) {
            continue;
        }

        m_busboards.push_back(busboard);
        succ = true;

        if (isLeft) {
            hasLeft = true;
        }
        if (isRight) {
            hasRight = true;
        }

        if (hasLeft && hasRight) {
            break;
        }
    }

    bool succDB = false;

    if(RedisDBManager::getInstance()->isConnected()){
        return succ;
    }
    succDB = RedisDBManager::getInstance()->connectToDB(HOSTNAME, PORT);
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

    std::vector<Cell> cellsFromBoard = busboard->getCellArray();
    std::vector<std::string> cellIDList = busboard->getCellIdList();
    std::vector<std::string> activeCellIDs;
    activeCellIDs.reserve(cellIDList.size());
    for (const auto& id : cellIDList) {
        if (!id.empty()) {
            activeCellIDs.push_back(id);
        }
    }

    if(cellsFromBoard.size() != cellIDList.size()){
        qDebug() << "cellsFromBoard size mismatch";
        return;
    }
    std::vector<Cell> cellsFromDB = RedisDBManager::getInstance()->getCellList(activeCellIDs);
    std::vector<CellTarget> celltargets = RedisDBManager::getInstance()->getCellTargets(activeCellIDs);

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

    std::map<std::string, CellTarget> cellTargetsMap;
    for(int i = 0; i < celltargets.size(); i++){
        if(celltargets.at(i).cellID().empty()){
            continue;
        }
        cellTargetsMap[celltargets.at(i).cellID()] = celltargets.at(i);
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
            bool forceUpdate = false;
            const std::string &experimentId = experiment.experimentId();
            auto lastExpIt = m_lastExperimentId.find(cellID);
            if (lastExpIt == m_lastExperimentId.end() || lastExpIt->second != experimentId) {
                m_lastExperimentId[cellID] = experimentId;
                m_lastVisualTimestamp.erase(cellID);
                RedisDBManager::getInstance()->pushCellVisuals(cellID, CellVisualsHistory());
                forceUpdate = true;
            }

            const auto &tempArcs = experiment.profile().tempArcsInSeq();
            if (tempArcs.empty()) {
                continue;
            }

            unsigned long long nowMs = Cell::getCurrentTimeMillis();
            bool startReady = updateExperimentStartIfReady(experiment, dbCell, nowMs);
            double targetTemp = preheatTargetTemp(experiment);
            int targetRpm = 0;
            unsigned long long totalDurationMs = totalExperimentDurationMs(experiment);
            unsigned long long startMs = experiment.startSystemTimeMSecs();
            unsigned long long elapsedMs = startMs > 0 && nowMs > startMs ? nowMs - startMs : 0;

            if (startReady && startMs > 0) {
                unsigned long long effectiveElapsed = elapsedMs;
                if (totalDurationMs > 0 && effectiveElapsed > totalDurationMs) {
                    effectiveElapsed = totalDurationMs;
                }
                calculateExperimentTargets(experiment, effectiveElapsed, &targetTemp, &targetRpm);
            }

            dbCell.setAsignedExperiment(experiment);
            dbCell.setAssignedRPM(targetRpm);
            dbCell.setAssignedTemp(targetTemp);

            unsigned int motorSelect = 0;
            bool motorSelectChanged = false;
            auto motorSelectIt = m_lastMotorSelect.find(cellID);
            if (motorSelectIt == m_lastMotorSelect.end() || motorSelectIt->second != motorSelect) {
                motorSelectChanged = true;
            }

            if(forceUpdate
                || targetRpm != boardCellMap[cellID].assignedRPM()
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

        if(cellTargetsMap.size() < 1){
            continue;
        }

        auto it = cellTargetsMap.find(cellID);

        if (it != cellTargetsMap.end()) {
            CellTarget celltarget = cellTargetsMap[cellID];
            dbCell.setAssignedRPM(celltarget.targetRPM());
            dbCell.setAssignedTemp(celltarget.targetTemp());

            unsigned int motorSelect = celltarget.motorSelect();
            bool motorSelectChanged = false;
            auto motorSelectIt = m_lastMotorSelect.find(cellID);
            if (motorSelectIt == m_lastMotorSelect.end() || motorSelectIt->second != motorSelect) {
                motorSelectChanged = true;
            }

            if(celltarget.targetRPM() != boardCellMap[cellID].assignedRPM()
                || abs( celltarget.targetTemp() - boardCellMap[cellID].assignedTemp() ) > TEMP_EPSILON
                || motorSelectChanged){
                int targetRPM = celltarget.targetRPM();
                double targetTemp = celltarget.targetTemp();
                std::string updateString = boardCellMap[cellID].generateUpdateDataStringToBoard(targetTemp, targetRPM, motorSelect);
                QCoreApplication::processEvents();

                QString str = QString::fromStdString(updateString);
                busboard->sendUpdateString(str);
                m_lastMotorSelect[cellID] = motorSelect;
            }
            QCoreApplication::processEvents();
        }//
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

    RedisDBManager::getInstance()->pushFlowStatus(busboard->busboardID(), busboard->flowStatus());
    QCoreApplication::processEvents();

    RedisDBManager::getInstance()->pushBusboardCellIds(busboard->busboardID(), activeCellIDs);
    QCoreApplication::processEvents();
}
