#ifndef SERVICERUNNER_H
#define SERVICERUNNER_H

#include "IBusboard.h"
#include <memory>
#include "common/RedisDBManager.h"

class ServiceRunner
{
public:
    ServiceRunner();
    bool initService();
    void startServiceLoop();

private:
    std::shared_ptr<IBusboard>  m_busboard = nullptr;
    std::vector<Cell> getUpdatedCells();

};

#endif // SERVICERUNNER_H
