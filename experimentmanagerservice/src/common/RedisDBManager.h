#ifndef REDISDBMANAGER_H
#define REDISDBMANAGER_H
#include <QObject>
#include <cpp_redis/cpp_redis>
#include "Cell.h"
#include "CellTarget.h"
#include "CellVisualsHistory.h"
#include "FlowStatus.h"

#define DB_CELLTABLE_KEY "cells"
#define DB_TARGETTABLE_KEY "targets"
#define DB_CELLIDS_TABLE_KEY "cellids"

#define DB_CELLJSON_KEY "cell"
#define DB_TARGETJSON_KEY "celltarget"
#define DB_FLOW_KEY "flowstatus"

class RedisDBManager: public QObject
{
    Q_OBJECT

public:
    static RedisDBManager* getInstance();
    bool connectToDB(std::string hostname, int port);
    std::vector<Cell> getCellList(std::vector<std::string> cellIDList);
    std::vector<CellTarget> getCellTargets(std::vector<std::string> cellIDList);
    std::vector<std::string> getBusboardCellIds(std::string busboardID);

    bool pushCellTarget(CellTarget celltarget);
    bool pushCellTargets(std::vector<CellTarget> celltargets);

    bool pushBusboardCellIds(std::string busboardID, std::vector<std::string> cellIDs);
    bool pushCellList(std::vector<Cell> cells);
    bool pushCellVisuals(std::string cellID, CellVisualsHistory history);
    CellVisualsHistory getCellVisualsHistory(std::string cellID);
    bool isConnected();
    bool pushFlowStatus(const FlowStatus& flowStatus);
    FlowStatus getFlowStatus();


private:
    RedisDBManager();
    static RedisDBManager *m_instance;
    cpp_redis::client m_client;

    unsigned long m_prevTimestamp = 0;


};

#endif // REDISDBMANAGER_H
