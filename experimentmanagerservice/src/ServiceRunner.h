#ifndef SERVICERUNNER_H
#define SERVICERUNNER_H

#include "IBusboard.h"
#include <memory>
#include <unordered_map>
#include "common/RedisDBManager.h"

class ServiceRunner
{
public:
    ServiceRunner();
    bool initService();
    void startServiceLoop();

private:
    std::vector<std::shared_ptr<IBusboard>> m_busboards;
    void processBusboard(const std::shared_ptr<IBusboard>& busboard);
    std::unordered_map<std::string, unsigned int> m_lastMotorSelect;
    std::unordered_map<std::string, std::string> m_lastExperimentId;
    std::unordered_map<std::string, bool> m_experimentRunning;
    std::unordered_map<std::string, unsigned long long> m_lastVisualTimestamp;

};

#endif // SERVICERUNNER_H
