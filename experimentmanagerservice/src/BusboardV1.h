#ifndef BUSBOARDV1_H
#define BUSBOARDV1_H

#include "IBusboard.h"
#include "BusboardSerialManager.h"
#include "common/Cell.h"
#include "common/FlowStatus.h"

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
    FlowStatus flowStatus() const;

private:
    std::vector<Cell> m_cellArray;
    FlowStatus m_flowStatus;
    float calculateTargetTemp(Cell cell);
    int calculateTargetRPM(Cell cell);



private slots:
    void cellStatusUpdated(Cell& cell);
    void presenceStatusUpdated(int slotIndex, bool isPresent);

};

#endif // BUSBOARDV1_H
