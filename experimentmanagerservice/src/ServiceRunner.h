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
    std::vector<std::shared_ptr<IBusboard>> m_busboards;
    std::string m_redisHost;
    int m_redisPort = 6379;
    void processBusboard(const std::shared_ptr<IBusboard>& busboard);
    std::unordered_map<std::string, unsigned int> m_lastMotorSelect;
    std::unordered_map<std::string, std::string> m_lastExperimentId;
    std::unordered_map<std::string, unsigned long long> m_lastVisualTimestamp;
    std::unordered_map<std::string, unsigned int> m_lastMachineStatusSeq;
    std::unordered_map<std::string, unsigned int> m_missedMachineStatus;
    std::unordered_map<std::string, unsigned long long> m_lastMachineStatusCheckMs;

};

#endif // SERVICERUNNER_H
