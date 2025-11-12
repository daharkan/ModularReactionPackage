#ifndef BUSBOARDV1_H
#define BUSBOARDV1_H

#include "IBusboard.h"
#include "BusboardSerialManager.h"
#include "common/Cell.h"

#define CELL_COUNT 10

class BusboardV1 : public IBusboard
{


public:
    BusboardV1();
    bool connectBoard() override;
    void sendExampleString() override;
    bool checkHealth() override;
    bool sendUpdateString(QString str) override;
    std::vector<std::string> getCellIdList() override;

    std::vector<Cell>& getCellArray() override;

private:
    std::vector<Cell> m_cellArray;
    float calculateTargetTemp(Cell cell);
    int calculateTargetRPM(Cell cell);



private slots:
    void cellStatusUpdated(Cell& cell);

};

#endif // BUSBOARDV1_H
