#ifndef BUSBOARDV1_H
#define BUSBOARDV1_H

#include "IBusboard.h"
#include "BusboardSerialManager.h"
#include "common/Cell.h"
#include "common/FlowStatus.h"

#define CELL_COUNT 5

class BusboardV1 : public IBusboard
{


public:
    BusboardV1();
    ~BusboardV1();
    bool connectBoard() override;
    void sendExampleString() override;
    bool checkHealth() override;
    bool sendUpdateString(QString str) override;
    std::vector<std::string> getCellIdList() override;
    unsigned int machineStatusSequence() const override;

    std::vector<Cell>& getCellArray() override;
    FlowStatus flowStatus() const override;

private:
    std::vector<Cell> m_cellArray;
    FlowStatus m_flowStatus;
    BusboardSerialManager *m_serialManager = nullptr;
    unsigned int m_machineStatusSequence = 0;
    float calculateTargetTemp(Cell cell);
    int calculateTargetRPM(Cell cell);



private slots:
    void cellStatusUpdated(Cell& cell);
    void presenceStatusUpdated(int slotIndex, bool isPresent);
    void machineStatusUpdated(const QString &busboardId, const QVector<int> &slotStates);
    void dutyStatusUpdated(int slotIndex, int heaterDuty, int peltierDuty);

};

#endif // BUSBOARDV1_H
