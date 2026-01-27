#include "uiCellGraph.h"
#include "ui_uiCellGraph.h"

#include <algorithm>
#include <limits>

CellGraph::CellGraph(Experiment &experiment, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CellGraph)
{
    ui->setupUi(this);
    m_plot = new QwtPlot();
    m_expTempCurve = new QwtPlotCurve(tr("Temperature"));
    m_expRpmCurve = new QwtPlotCurve(tr("Motor Speed"));
    m_currentTempCurve = new QwtPlotCurve(tr("Temperature"));
    m_currentRpmCurve = new QwtPlotCurve(tr("Motor Speed"));

    m_plot->setTitle(tr("Temperature and RPM vs. Time"));
    m_plot->setCanvasBackground(Qt::white);
    m_plot->setAxisTitle(QwtPlot::xBottom, tr("Time (s)"));
    m_plot->enableAxis(QwtPlot::yRight);
    m_plot->setAxisTitle(QwtPlot::yLeft, "Temperature (°C)");
    m_plot->setAxisTitle(QwtPlot::yRight, "RPM");
    
    m_expTempCurve->setPen(Qt::red, 1); // Set curve color and thickness
    m_expTempCurve->attach(m_plot);
    
    m_expRpmCurve->setPen(Qt::black, 1); // Set curve color and thickness
    m_expRpmCurve->setYAxis(QwtPlot::yRight);  // Use the right Y axis for curve2
    m_expRpmCurve->attach(m_plot);

    m_currentTempCurve->setPen(Qt::blue, 2); // Set curve color and thickness
    m_currentTempCurve->attach(m_plot);

    m_currentRpmCurve->setPen(Qt::gray, 2); // Set curve color and thickness
    m_currentRpmCurve->setYAxis(QwtPlot::yRight);  // Use the right Y axis for curve2
    m_currentRpmCurve->attach(m_plot);


    m_legend = new QwtLegend;
    m_plot->insertLegend(m_legend, QwtPlot::BottomLegend);


    m_plot->replot();

    ui->graphLayout->addWidget(m_plot);

    m_dataPushStartTimestamp = 0;
    updateTheExperiment(experiment);
}

CellGraph::~CellGraph()
{
    delete m_expRpmCurve;
    delete m_expTempCurve;
    delete ui;
}

void CellGraph::updateTheExperiment(Experiment &experiment)
{
    m_experiment = experiment;
    std::vector<double> tempTimeData, rpmTimeData;
    std::vector<double> temperatureData;
    std::vector<double> rpmData;

    Profile profile = experiment.profile();
    bool hasTempRange = false;
    double minTemp = std::numeric_limits<double>::max();
    double maxTemp = std::numeric_limits<double>::lowest();
    bool hasTempSample = false;
    unsigned long lastTempSampleMsec = 0;
    for(int i = 0; i < profile.tempArcsInSeq().size(); i++){
        TempArc tempArc = profile.tempArcsInSeq().at(i);
        minTemp = std::min<double>(minTemp, tempArc.startTemp());
        minTemp = std::min<double>(minTemp, tempArc.finishTemp());
        maxTemp = std::max<double>(maxTemp, tempArc.startTemp());
        maxTemp = std::max<double>(maxTemp, tempArc.finishTemp());
        hasTempRange = true;

        unsigned long arcStartMsecs = tempArc.startTimeMsec();
        unsigned long arcFinishMsecs = tempArc.finishTimeMsec();

        if(!hasTempSample || lastTempSampleMsec < arcStartMsecs){
            tempTimeData.push_back(arcStartMsecs / 1000.0);
            temperatureData.push_back(tempArc.calculateY(arcStartMsecs));
            hasTempSample = true;
            lastTempSampleMsec = arcStartMsecs;
        }

        for(unsigned long time = arcStartMsecs + GRAPH_RESOLUTION_MSECS;
            time < arcFinishMsecs;
            time += GRAPH_RESOLUTION_MSECS){
            if(time <= lastTempSampleMsec){
                continue;
            }
            tempTimeData.push_back(time / 1000.0  /*- (m_dataPushStartTimestamp/1000.0)*/);
            temperatureData.push_back(tempArc.calculateY(time));
            lastTempSampleMsec = time;
        }

        if(!hasTempSample || arcFinishMsecs > lastTempSampleMsec){
            tempTimeData.push_back(arcFinishMsecs / 1000.0);
            temperatureData.push_back(tempArc.calculateY(arcFinishMsecs));
            hasTempSample = true;
            lastTempSampleMsec = arcFinishMsecs;
        }
    }


    bool hasRpmSample = false;
    unsigned long lastRpmSampleMsec = 0;
    for(int i = 0; i < profile.rpmArcsInSeq().size(); i++){
        RPMArc rpmArc = profile.rpmArcsInSeq().at(i);
        unsigned long arcStartMsecs = rpmArc.startTimeMsec();
        unsigned long arcFinishMsecs = rpmArc.finishTimeMsec();

        if(!hasRpmSample || lastRpmSampleMsec < arcStartMsecs){
            rpmTimeData.push_back(arcStartMsecs / 1000.0);
            rpmData.push_back(rpmArc.calculateY(arcStartMsecs));
            hasRpmSample = true;
            lastRpmSampleMsec = arcStartMsecs;
        }

        for(unsigned long time = arcStartMsecs + GRAPH_RESOLUTION_MSECS;
            time < arcFinishMsecs;
            time += GRAPH_RESOLUTION_MSECS){
            if(time <= lastRpmSampleMsec){
                continue;
            }
            rpmTimeData.push_back(time / 1000.0 /*- (m_dataPushStartTimestamp/1000.0)*/);
            rpmData.push_back(rpmArc.calculateY(time));
            lastRpmSampleMsec = time;
        }

        if(!hasRpmSample || arcFinishMsecs > lastRpmSampleMsec){
            rpmTimeData.push_back(arcFinishMsecs / 1000.0);
            rpmData.push_back(rpmArc.calculateY(arcFinishMsecs));
            hasRpmSample = true;
            lastRpmSampleMsec = arcFinishMsecs;
        }
    }
    m_expRpmCurve->setSamples(rpmTimeData.data(), rpmData.data(), rpmTimeData.size());
    m_expTempCurve->setSamples(tempTimeData.data(), temperatureData.data(), tempTimeData.size());

    m_plot->replot();

    //qDebug() << "graph updated.";

}

void CellGraph::loadVisualHistory(const CellVisualsHistory& history)
{
    const auto& visuals = history.visuals();
    if (visuals.empty()) {
        return;
    }

    m_currentTempList.clear();
    m_currentRPMList.clear();
    m_currentTimestampList.clear();

    for (const auto& visual : visuals) {
        m_currentTempList.push_back(visual.temperature());
        m_currentRPMList.push_back(visual.rpm());
        m_currentTimestampList.push_back(visual.timestamp());
    }

    unsigned long firstTimestamp = m_currentTimestampList.front();
    if (firstTimestamp == 0) {
        firstTimestamp = Cell::getCurrentTimeMillis();
    }
    m_dataPushStartTimestamp = firstTimestamp;
    m_pushingDataStarted = true;

    QList<double> timeData;
    for (int i = 0; i < m_currentTimestampList.size(); ++i) {
        timeData.push_back((m_currentTimestampList.at(i) - m_dataPushStartTimestamp) / 1000.0);
    }

    m_currentRpmCurve->setSamples(timeData.data(), m_currentRPMList.data(), timeData.size());
    m_currentTempCurve->setSamples(timeData.data(), m_currentTempList.data(), timeData.size());
    applyTempAxisRange();
    m_plot->replot();
}


void CellGraph::pushTemperatureAndRPMData(double temp, double rpm, unsigned long timestamp)
{
    if(!m_pushingDataStarted){
        return;
    }
    unsigned long currentT = Cell::getCurrentTimeMillis();
    if(timestamp == 0){
        timestamp = currentT;
    }

    //qDebug() << "pushing data. timestamp: " << timestamp << "  m_dataPushStartTimestamp: " << m_dataPushStartTimestamp;

    m_currentTempList.push_back(temp);
    m_currentRPMList.push_back(rpm);
    m_currentTimestampList.push_back(timestamp);

    QList<double> timeData;


    for(int i = 0; i < m_currentTimestampList.size(); i++){
        timeData.push_back((m_currentTimestampList.at(i)-m_dataPushStartTimestamp)/1000.0);
    }

    if(timeData.size() > 2)
        //qDebug() << "pushing data. timeData: " << timeData.at(2);

    m_currentRpmCurve->setSamples(timeData.data(), m_currentRPMList.data(), timeData.size());
    m_currentTempCurve->setSamples(timeData.data(), m_currentTempList.data(), timeData.size());
    applyTempAxisRange();
    m_plot->replot();
}


void CellGraph::applyTempAxisRange()
{
    if (!m_hasTempRange) {
        return;
    }
    double minTemp = m_minTempRange;
    double maxTemp = m_maxTempRange;
    if (minTemp == maxTemp) {
        maxTemp += 1.0;
    }
    m_plot->setAxisAutoScale(QwtPlot::yLeft, false);
    m_plot->setAxisScale(QwtPlot::yLeft, minTemp, maxTemp);
}

void CellGraph::initilizeExperimentGraph()
{
    m_dataPushStartTimestamp = Cell::getCurrentTimeMillis();
    m_pushingDataStarted = true;
    updateTheExperiment(m_experiment);
}
