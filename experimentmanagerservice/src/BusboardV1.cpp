#include "BusboardV1.h"
#include "common/RedisDBManager.h"

#define MIN_CELL_UPDATE_INTERVAL_MSECS 100

BusboardV1::BusboardV1()
{
    m_cellArray.resize(CELL_COUNT, Cell());
    m_cellCount = CELL_COUNT;
    m_serialManager = new BusboardSerialManager(this);
    QObject::connect(m_serialManager, &BusboardSerialManager::sgn_updateCell, this, &BusboardV1::cellStatusUpdated);
    QObject::connect(m_serialManager, &BusboardSerialManager::sgn_presenceUpdate, this, &BusboardV1::presenceStatusUpdated);
}

BusboardV1::~BusboardV1()
{
}

bool BusboardV1::connectBoard()
{
    bool succ = m_serialManager->connectAndAssignThePort();
    m_busboardID = m_serialManager->recievedBusboardSerial();
    return succ;
}

void BusboardV1::sendExampleString()
{
    m_serialManager->sendExampleString("aabbcc");
}

bool BusboardV1::checkHealth()
{
    return m_serialManager->isSerialPortOK();
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
    return m_serialManager->writeCellUpdateString(str);
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
        if (!m_busboardID.empty() && !cell.cellID().empty()) {
            std::string prefixedId = m_busboardID + "_" + cell.cellID();
            if (cell.cellID().find(m_busboardID + "_") != 0) {
                cell.setCellID(prefixedId);
            }
        }

        //qDebug() << "updating cell status pos " << idx << " with inner: " << cell.currentTempInner() << " with rpm: " << cell.currentRPM();
        m_cellArray[idx-1] = cell;
        m_flowStatus.setFlowRateLpm(cell.flowRateLpm());
        m_flowStatus.setFlowTemp(cell.flowTemp());
        m_flowStatus.setTimestamp(Cell::getCurrentTimeMillis());
    }
}

void BusboardV1::presenceStatusUpdated(int slotIndex, bool isPresent)
{
    if (slotIndex < 1 || slotIndex > CELL_COUNT) {
        return;
    }

    Cell &cell = m_cellArray[slotIndex - 1];
    cell.setIsPlugged(isPresent);
    cell.setLastUpdatedTimestamp(Cell::getCurrentTimeMillis());
}
