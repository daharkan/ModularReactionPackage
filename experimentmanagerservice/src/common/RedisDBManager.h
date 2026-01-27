#ifndef REDISDBMANAGER_H
#define REDISDBMANAGER_H
#include <QObject>
#include <cpp_redis/cpp_redis>
#include <string>
#include "Cell.h"
#include "CellVisualsHistory.h"
#include "FlowStatus.h"
#include "Experiment.h"
#include "User.h"
#include <optional>

#define DB_SCHEMA_VERSION_KEY "mr10:schema_version"
#define DB_SCHEMA_VERSION_VALUE "3"

#define DB_MACHINE_TABLE_KEY "mr10:machines"
#define DB_CELLTABLE_KEY "mr10:cells"
#define DB_USERS_TABLE_KEY "mr10:users"
#define DB_EXPERIMENTS_TABLE_KEY "mr10:experiments"
#define DB_CELLVISUALS_TABLE_KEY "mr10:cell_visuals"

#define DB_CELLJSON_KEY "cell"
#define DB_EXPERIMENTJSON_KEY "experiment"
#define DB_CELLVISUALS_JSON_KEY "cellVisualsHistory"
#define DB_BUSBOARD_FLOW_KEY "flowStatus"
#define DB_BUSBOARD_CELLIDS_KEY "cellIDs"
#define DB_BUSBOARD_LAST_UPDATED_KEY "lastUpdatedTimestamp"
#define DB_USERJSON_KEY "user"
#define DB_MACHINE_BUSBOARDS_KEY "busboards"
#define DB_MACHINE_SLOTS_KEY "slots"
#define DB_MACHINE_SLOT_PLUGGED_KEY "plugged"
#define DB_MACHINE_SLOT_CELLID_KEY "cellId"

class RedisDBManager: public QObject
{
    Q_OBJECT

public:
    static RedisDBManager* getInstance();
    static void setDefaultConnection(const std::string &hostname, int port);
    static const std::string &defaultHost();
    static int defaultPort();
    bool connectToDB(std::string hostname, int port);
    bool connectToDefault();
    bool initializeSchema(bool reset);
    std::vector<Cell> getCellList(std::vector<std::string> cellIDList);
    std::vector<std::string> getMachineIds();
    bool getMachineBusboardIds(const std::string &machineId, std::string *lhsId, std::string *rhsId);
    std::vector<std::string> getBusboardCellIds(std::string busboardID);
    std::vector<std::string> getBusboardIds();
    std::vector<std::string> getCellIds();


    bool pushExperiment(const Experiment& experiment);
    std::optional<Experiment> getExperiment(const std::string& experimentId);
    std::vector<Experiment> getExperiments();
    bool deleteExperiment(const std::string& experimentId);
    bool pushUser(const User& user);
    std::optional<User> getUser(const std::string& username);
    std::vector<User> getUsers();
    bool deleteUser(const std::string& username);

    bool pushMachineStatus(const std::string& busboardID, const std::vector<int>& slotStates);
    bool removeBusboardFromMachine(const std::string& busboardID);
    bool pushBusboardCellIds(std::string busboardID, std::vector<std::string> cellIDs);
    bool pushCellList(std::vector<Cell> cells);
    bool deleteCell(const std::string& cellID);
    bool pushCellVisuals(std::string cellID, CellVisualsHistory history);
    CellVisualsHistory getCellVisualsHistory(std::string cellID);
    bool isConnected();
    bool pushFlowStatus(const std::string& busboardID, const FlowStatus& flowStatus);
    FlowStatus getFlowStatus(const std::string& busboardID);


private:
    RedisDBManager();
    static RedisDBManager *m_instance;
    static std::string m_defaultHost;
    static int m_defaultPort;
    cpp_redis::client m_client;

    unsigned long m_prevTimestamp = 0;


};

#endif // REDISDBMANAGER_H
