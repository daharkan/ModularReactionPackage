#ifndef SERVICERUNNER_H
#define SERVICERUNNER_H

#include "IBusboard.h"
#include <memory>
#include <unordered_map>
#include <string>
#include "common/RedisDBManager.h"

class ServiceRunner
{
public:
    ServiceRunner(const std::string &redisHost, int redisPort);
    bool initService();
    void startServiceLoop();

private:
    struct TargetSyncInfo {
        float desiredTemp = 0.0f;
        float desiredTempFuture = 0.0f;
        int desiredRpm = 0;
        unsigned int motorSelect = 0;
        unsigned long long lastSendMs = 0;
        unsigned long long firstSendMs = 0;
        unsigned int attempt = 0;
        bool pending = false;
        std::string experimentId;
    };

    std::vector<std::shared_ptr<IBusboard>> m_busboards;
    std::string m_redisHost;
    int m_redisPort = 6379;
    void processBusboard(const std::shared_ptr<IBusboard>& busboard);
    std::unordered_map<std::string, unsigned int> m_lastMotorSelect;
    std::unordered_map<std::string, TargetSyncInfo> m_targetSync;
    std::unordered_map<std::string, std::string> m_lastExperimentId;
    std::unordered_map<std::string, unsigned long long> m_lastVisualTimestamp;
    std::unordered_map<std::string, unsigned int> m_lastMachineStatusSeq;
    std::unordered_map<std::string, unsigned int> m_missedMachineStatus;
    std::unordered_map<std::string, unsigned long long> m_lastMachineStatusCheckMs;

};

#endif // SERVICERUNNER_H
