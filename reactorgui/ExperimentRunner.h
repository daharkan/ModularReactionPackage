#ifndef EXPERIMENTRUNNER_H
#define EXPERIMENTRUNNER_H

#include <QObject>
#include "Experiment.h"
#include "Cell.h"
#include <string>

enum ExperimentRunState
{
    STATE_UNDEFINED,
    STATE_INITILIZED,
    STATE_PREHEAT,
    STATE_RUNNING,
    STATE_COMPLETED
};

class ExperimentRunner : public QObject
{
    Q_OBJECT
public:
    explicit ExperimentRunner(QObject *parent = nullptr);

    void assignExperiment(Experiment exp);
    void setCellId(const std::string &cellId);
    void run();
    void reset();

    ExperimentRunState state() const;

private:
    Experiment m_experiment;
    ExperimentRunState m_state = STATE_UNDEFINED;
    Cell m_cell;
    std::string m_cellId;
    unsigned long m_startingTimestampMsec = 0;

    std::vector<double> m_tempTimeData, m_rpmTimeData;
    std::vector<double> m_temperatureData;
    std::vector<double> m_rpmData;



    void delay(int msec);

signals:
    void sgn_updateExperimentState(ExperimentRunState state);

};

#endif // EXPERIMENTRUNNER_H
