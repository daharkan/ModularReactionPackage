#include "uiExperimentGraph.h"
#include "ui_uiExperimentGraph.h"

ExperimentGraph::ExperimentGraph(Experiment &experiment, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentGraph)
{
    ui->setupUi(this);
    m_plot = new QwtPlot();
    m_tempCurve = new QwtPlotCurve(tr("Temperature"));
    m_tempPreviewCurve = new QwtPlotCurve(tr("Preview"));
    m_rpmCurve = new QwtPlotCurve(tr("Motor Speed"));

    m_plot->setTitle(tr("Temperature and RPM vs. Time"));
    m_plot->setCanvasBackground(Qt::white);
    m_plot->setAxisTitle(QwtPlot::xBottom, tr("Time (s)"));
    m_plot->enableAxis(QwtPlot::yRight);
    m_plot->setAxisTitle(QwtPlot::yLeft, "Temperature (°C)");
    m_plot->setAxisTitle(QwtPlot::yRight, "RPM");

    m_tempCurve->setPen(Qt::red, 2); // Set curve color and thickness
    m_tempCurve->attach(m_plot);

    m_tempPreviewCurve->setPen(QColor(0, 120, 215), 2, Qt::DashLine);
    m_tempPreviewCurve->attach(m_plot);

    m_rpmCurve->setPen(Qt::black, 2); // Set curve color and thickness
    m_rpmCurve->setYAxis(QwtPlot::yRight);  // Use the right Y axis for curve2
    m_rpmCurve->attach(m_plot);

    m_legend = new QwtLegend;
    m_plot->insertLegend(m_legend, QwtPlot::BottomLegend);


    m_plot->replot();

    ui->graphLayout->addWidget(m_plot);

    updateTheExperiment(experiment);
}

ExperimentGraph::~ExperimentGraph()
{
    delete m_rpmCurve;
    delete m_tempPreviewCurve;
    delete m_tempCurve;
    delete ui;
}

namespace {
void buildTempSeries(const Profile &profile,
                     std::vector<double> &tempTimeData,
                     std::vector<double> &temperatureData)
{
    bool hasTempSample = false;
    unsigned long lastTempSampleMsec = 0;
    const auto tempArcs = profile.tempArcsInSeq();
    for (int i = 0; i < tempArcs.size(); i++) {
        TempArc tempArc = tempArcs.at(i);
        unsigned long arcStartMsecs = tempArc.startTimeMsec();
        unsigned long arcFinishMsecs = tempArc.finishTimeMsec();

        if (!hasTempSample || lastTempSampleMsec < arcStartMsecs) {
            tempTimeData.push_back(arcStartMsecs / 1000.0);
            temperatureData.push_back(tempArc.calculateY(arcStartMsecs));
            hasTempSample = true;
            lastTempSampleMsec = arcStartMsecs;
        }

        for (unsigned long time = arcStartMsecs + GRAPH_RESOLUTION_MSECS;
             time < arcFinishMsecs;
             time += GRAPH_RESOLUTION_MSECS) {
            if (time <= lastTempSampleMsec) {
                continue;
            }
            tempTimeData.push_back(time / 1000.0);
            temperatureData.push_back(tempArc.calculateY(time));
            lastTempSampleMsec = time;
        }

        if (!hasTempSample || arcFinishMsecs > lastTempSampleMsec) {
            tempTimeData.push_back(arcFinishMsecs / 1000.0);
            temperatureData.push_back(tempArc.calculateY(arcFinishMsecs));
            hasTempSample = true;
            lastTempSampleMsec = arcFinishMsecs;
        }
    }
}
} // namespace

void ExperimentGraph::updateTheExperiment(Experiment &experiment)
{
    std::vector<double> tempTimeData, rpmTimeData;
    std::vector<double> temperatureData;
    std::vector<double> rpmData;

    Profile profile = experiment.profile();
    buildTempSeries(profile, tempTimeData, temperatureData);


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
            rpmTimeData.push_back(time / 1000.0);
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

    //convert msec to secs


    m_rpmCurve->setSamples(rpmTimeData.data(), rpmData.data(), rpmTimeData.size());
    m_tempCurve->setSamples(tempTimeData.data(), temperatureData.data(), tempTimeData.size());
    m_plot->replot();

    qDebug() << "graph updated.";

}

void ExperimentGraph::updatePreviewProfile(const Profile &profile)
{
    std::vector<double> tempTimeData;
    std::vector<double> temperatureData;
    buildTempSeries(profile, tempTimeData, temperatureData);

    if (tempTimeData.empty()) {
        std::vector<double> dummyList;
        m_tempPreviewCurve->setSamples(dummyList.data(), dummyList.data(), 0);
    } else {
        m_tempPreviewCurve->setSamples(tempTimeData.data(), temperatureData.data(), tempTimeData.size());
    }
    m_plot->replot();
}

void ExperimentGraph::pushTemperatureAndRPMData(double temp, double rpm, unsigned long timestamp)
{
    if(timestamp == 0){
        timestamp = Cell::getCurrentTimeMillis();
    }
    //push data
}
