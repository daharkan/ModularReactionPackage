#ifndef BUSBOARDSIM_H
#define BUSBOARDSIM_H

#include "IBusboard.h"

class BusboardSim : public IBusboard
{

public:
    //BusboardSim();
    bool connectBoard() override;
    void sendExampleString() override = 0;
    bool checkHealth() override;
    std::vector<Cell>& getCellArray() override = 0;
    bool sendUpdateString(QString str) override = 0;
    std::vector<std::string> getCellIdList() override = 0;

};

#endif // BUSBOARDSIM_H
