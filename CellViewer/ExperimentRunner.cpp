#include "ExperimentRunner.h"
#include "RedisDBManager.h"
#include "CellTarget.h"

#include <QCoreApplication>
#include <thread>

#define EPSILON_TEMP 0.2
#define LOOP_TIME_INTERVAL_MSECS 200


ExperimentRunner::ExperimentRunner(QObject *parent)
    : QObject{parent}
{

}


void ExperimentRunner::delay(int msec)
{
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::milliseconds(msec);

    while (std::chrono::steady_clock::now() < endTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small sleep to prevent high CPU usage
    }

}


void ExperimentRunner::assignExperiment(Experiment exp)
{
    m_experiment = exp;


    Profile profile = m_experiment.profile();
    for(int i = 0; i < profile.tempArcsInSeq().size(); i++){
        TempArc tempArc = profile.tempArcsInSeq().at(i);
        unsigned long arcDurationMsecs = tempArc.durationMSec();
        unsigned long lastSecs = 0;
        if(m_tempTimeData.size() > 0){
            lastSecs = m_tempTimeData.at(m_tempTimeData.size()-1);
        }

        for(unsigned long time = LOOP_TIME_INTERVAL_MSECS; time < arcDurationMsecs; time+=LOOP_TIME_INTERVAL_MSECS){
            m_tempTimeData.push_back(lastSecs+time/1000.0 );
            m_temperatureData.push_back(tempArc.calculateY(lastSecs*1000.0+time));

            qDebug() << "Experiment setpoint: temp: " << m_temperatureData.at(m_temperatureData.size()-1)
                     << "  time: " << m_tempTimeData.at(m_tempTimeData.size()-1);
        }
    }


    for(int i = 0; i < profile.rpmArcsInSeq().size(); i++){
        RPMArc rpmArc = profile.rpmArcsInSeq().at(i);
        unsigned long arcDurationMsecs = rpmArc.durationMSec();
        unsigned long lastSecs = 0;
        if(m_rpmTimeData.size() > 0){
            lastSecs = m_rpmTimeData.at(m_rpmTimeData.size()-1);
        }
        for(int time = LOOP_TIME_INTERVAL_MSECS; time < arcDurationMsecs; time+=LOOP_TIME_INTERVAL_MSECS){
            m_rpmTimeData.push_back(lastSecs+time/1000.0);
            m_rpmData.push_back(rpmArc.calculateY(lastSecs*1000.0 + time));
        }
    }

    //convert msec to secs
    m_state = STATE_INITILIZED;
}

void ExperimentRunner::setCellId(const std::string &cellId)
{
    m_cellId = cellId;
}

void ExperimentRunner::run()
{
    if (m_cellId.empty()) {
        return;
    }

    std::vector<std::string> cellIds = {m_cellId};
    std::vector<Cell> cells = RedisDBManager::getInstance()->getCellList(cellIds);
    if (cells.empty()) {
        return;
    }
    m_cell = cells.front();

    float startTemp = m_experiment.profile().tempArcsInSeq().at(0).startTemp();
    qDebug() << "ExperimentRunner    runinit...";

    while(abs(startTemp - m_cell.currentTempExt()) > EPSILON_TEMP){
        cells = RedisDBManager::getInstance()->getCellList(cellIds);
        if (cells.empty()) {
            return;
        }
        m_cell = cells.front();
        m_state = STATE_PREHEAT;

        if(m_temperatureData.size() < 1){
            return;
        }
        float targetTemp =  m_temperatureData.at(0);
        qDebug() << "ExperimentRunner    STATE_PREHEAT targetTemp: " << targetTemp;

        CellTarget celltarget;
        celltarget.setCellID(m_cellId);
        celltarget.setTargetTemp(targetTemp);
        celltarget.setTargetRPM(555);
        celltarget.setMotorSelect(0);
        celltarget.setTimestamp(Cell::getCurrentTimeMillis());
        RedisDBManager::getInstance()->pushCellTarget(celltarget);

        emit sgn_updateExperimentState(m_state);
        delay(LOOP_TIME_INTERVAL_MSECS);
    }


    m_startingTimestampMsec = Cell::getCurrentTimeMillis();

    cells = RedisDBManager::getInstance()->getCellList(cellIds);
    if (cells.empty()) {
        return;
    }
    m_cell = cells.front();
    m_state = STATE_RUNNING;

    for(int i = 0; i < m_tempTimeData.size(); i++){
        cells = RedisDBManager::getInstance()->getCellList(cellIds);
        if (cells.empty()) {
            return;
        }
        m_cell = cells.front();

        float targetTemp =  m_temperatureData.at(i);
        qDebug() << "ExperimentRunner    STATE_RUNNING targetTemp: " << targetTemp << "  time: " << m_tempTimeData.at(i);

        CellTarget celltarget;
        celltarget.setCellID(m_cellId);
        celltarget.setTargetTemp(targetTemp);
        celltarget.setTargetRPM(555);
        celltarget.setMotorSelect(0);
        celltarget.setTimestamp(Cell::getCurrentTimeMillis());
        RedisDBManager::getInstance()->pushCellTarget(celltarget);
        emit sgn_updateExperimentState(m_state);
        delay(LOOP_TIME_INTERVAL_MSECS);
    }

    cells = RedisDBManager::getInstance()->getCellList(cellIds);
    if (cells.empty()) {
        return;
    }
    m_cell = cells.front();
    m_state = STATE_COMPLETED;
    emit sgn_updateExperimentState(m_state);

}

ExperimentRunState ExperimentRunner::state() const
{
    return m_state;
}
