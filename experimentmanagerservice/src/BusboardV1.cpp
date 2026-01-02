#include "BusboardV1.h"
#include "common/RedisDBManager.h"

#define MIN_CELL_UPDATE_INTERVAL_MSECS 100

BusboardV1::BusboardV1()
{
    m_cellArray.resize(CELL_COUNT, Cell());
    m_cellCount = CELL_COUNT;
    QObject::connect(BusboardSerialManager::getInstance(), &BusboardSerialManager::sgn_updateCell, this, &BusboardV1::cellStatusUpdated);
}

bool BusboardV1::connectBoard()
{
    bool succ = BusboardSerialManager::getInstance()->connectAndAssignThePort();
    m_busboardID = BusboardSerialManager::getInstance()->recievedBusboardSerial();
    return succ;
}

void BusboardV1::sendExampleString()
{
    BusboardSerialManager::getInstance()->sendExampleString("aabbcc");
}

bool BusboardV1::checkHealth()
{
    return BusboardSerialManager::getInstance()->isSerialPortOK();
}


std::vector<Cell>& BusboardV1::getCellArray()
{
    return m_cellArray;
}

FlowStatus BusboardV1::flowStatus() const
{
    return m_flowStatus;
}

bool BusboardV1::sendUpdateString(QString str)
{
    return BusboardSerialManager::getInstance()->writeCellUpdateString(str);
}

std::vector<std::string> BusboardV1::getCellIdList()
{
    std::vector<std::string> cellIdList;
    for(int i = 0; i < m_cellArray.size(); i++){
        cellIdList.push_back(m_cellArray.at(i).cellID());
    }
    return cellIdList;
}

float BusboardV1::calculateTargetTemp(Cell cell)
{
    unsigned long currentMSecs = Cell::getCurrentTimeMillis();
    Profile profile = cell.asignedExperiment().profile();
    float targetTemp = profile.calculateTemp(currentMSecs);
    return targetTemp;

}



int BusboardV1::calculateTargetRPM(Cell cell)
{
    unsigned long currentMSecs = Cell::getCurrentTimeMillis();
    Profile profile = cell.asignedExperiment().profile();
    int targetRPM = profile.calculateRPM(currentMSecs);
    return targetRPM;
}

void BusboardV1::cellStatusUpdated(Cell &cell)
{
    int idx = cell.positionIdx();
    if(idx > 0 && idx < CELL_COUNT+1){
        cell.setLastUpdatedTimestamp(Cell::getCurrentTimeMillis());

        //qDebug() << "updating cell status pos " << idx << " with inner: " << cell.currentTempInner() << " with rpm: " << cell.currentRPM();
        m_cellArray[idx-1] = cell;
        m_flowStatus.setFlowRateLpm(cell.flowRateLpm());
        m_flowStatus.setFlowTemp(cell.flowTemp());
        m_flowStatus.setTimestamp(Cell::getCurrentTimeMillis());
    }
}
