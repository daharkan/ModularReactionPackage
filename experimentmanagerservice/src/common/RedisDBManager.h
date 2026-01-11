#ifndef REDISDBMANAGER_H
#define REDISDBMANAGER_H
#include <QObject>
#include <cpp_redis/cpp_redis>
#include "Cell.h"
#include "CellTarget.h"
#include "CellVisualsHistory.h"
#include "FlowStatus.h"
#include "Experiment.h"
#include "User.h"
#include <optional>

#define DB_SCHEMA_VERSION_KEY "mr10:schema_version"
#define DB_SCHEMA_VERSION_VALUE "2"

#define DB_BUSBOARD_TABLE_KEY "mr10:busboards"
#define DB_CELLTABLE_KEY "mr10:cells"
#define DB_TARGETTABLE_KEY "mr10:targets"
#define DB_USERS_TABLE_KEY "mr10:users"
#define DB_EXPERIMENTS_TABLE_KEY "mr10:experiments"

#define DB_CELLJSON_KEY "cell"
#define DB_TARGETJSON_KEY "celltarget"
#define DB_BUSBOARDJSON_KEY "busboard"
#define DB_EXPERIMENTJSON_KEY "experiment"
#define DB_BUSBOARD_FLOW_KEY "flowStatus"
#define DB_BUSBOARD_CELLIDS_KEY "cellIDs"
#define DB_BUSBOARD_LAST_UPDATED_KEY "lastUpdatedTimestamp"
#define DB_USERJSON_KEY "user"

class RedisDBManager: public QObject
{
    Q_OBJECT

public:
    static RedisDBManager* getInstance();
    bool connectToDB(std::string hostname, int port);
    bool initializeSchema(bool reset);
    std::vector<Cell> getCellList(std::vector<std::string> cellIDList);
    std::vector<CellTarget> getCellTargets(std::vector<std::string> cellIDList);
    std::vector<std::string> getBusboardCellIds(std::string busboardID);
    std::vector<std::string> getBusboardIds();
    std::vector<std::string> getCellIds();

    bool pushCellTarget(CellTarget celltarget);
    bool pushCellTargets(std::vector<CellTarget> celltargets);

    bool pushExperiment(const Experiment& experiment);
    std::optional<Experiment> getExperiment(const std::string& experimentId);
    std::vector<Experiment> getExperiments();
    bool deleteExperiment(const std::string& experimentId);
    bool pushUser(const User& user);
    std::optional<User> getUser(const std::string& username);
    std::vector<User> getUsers();
    bool deleteUser(const std::string& username);

    bool pushBusboardCellIds(std::string busboardID, std::vector<std::string> cellIDs);
    bool pushCellList(std::vector<Cell> cells);
    bool pushCellVisuals(std::string cellID, CellVisualsHistory history);
    CellVisualsHistory getCellVisualsHistory(std::string cellID);
    bool isConnected();
    bool pushFlowStatus(const std::string& busboardID, const FlowStatus& flowStatus);
    FlowStatus getFlowStatus(const std::string& busboardID);


private:
    RedisDBManager();
    static RedisDBManager *m_instance;
    cpp_redis::client m_client;

    unsigned long m_prevTimestamp = 0;


};

#endif // REDISDBMANAGER_H
