#include "uiExperimentGraph.h"
#include "ui_uiExperimentGraph.h"

ExperimentGraph::ExperimentGraph(Experiment &experiment, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentGraph)
{
    ui->setupUi(this);
    m_plot = new QwtPlot();
    m_tempCurve = new QwtPlotCurve(tr("Temperature"));
    m_rpmCurve = new QwtPlotCurve(tr("Motor Speed"));

    m_plot->setTitle(tr("Temperature and RPM vs. Time"));
    m_plot->setCanvasBackground(Qt::white);
    m_plot->setAxisTitle(QwtPlot::xBottom, tr("Time (s)"));
    m_plot->enableAxis(QwtPlot::yRight);
    m_plot->setAxisTitle(QwtPlot::yLeft, "Temperature (°C)");
    m_plot->setAxisTitle(QwtPlot::yRight, "RPM");

    m_tempCurve->setPen(Qt::red, 2); // Set curve color and thickness
    m_tempCurve->attach(m_plot);

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
    delete m_tempCurve;
    delete ui;
}

void ExperimentGraph::updateTheExperiment(Experiment &experiment)
{
    std::vector<double> tempTimeData, rpmTimeData;
    std::vector<double> temperatureData;
    std::vector<double> rpmData;

    Profile profile = experiment.profile();
    for(int i = 0; i < profile.tempArcsInSeq().size(); i++){
        TempArc tempArc = profile.tempArcsInSeq().at(i);
        unsigned long arcDurationMsecs = tempArc.durationMSec();

        qDebug() << "arcDurationMsecs: " << arcDurationMsecs;

        unsigned long lastSecs = 0;
        if(tempTimeData.size() > 0){
            lastSecs = tempTimeData.at(tempTimeData.size()-1);
        }

        qDebug() << "lastSecs: " << lastSecs;

        for(unsigned long time = GRAPH_RESOLUTION_MSECS; time < arcDurationMsecs; time+=GRAPH_RESOLUTION_MSECS){
            tempTimeData.push_back(lastSecs+time/1000.0);
            temperatureData.push_back(tempArc.calculateY(lastSecs*1000.0+time));
        }
    }


    for(int i = 0; i < profile.rpmArcsInSeq().size(); i++){
        RPMArc rpmArc = profile.rpmArcsInSeq().at(i);
        unsigned long arcDurationMsecs = rpmArc.durationMSec();
        unsigned long lastSecs = 0;
        if(rpmTimeData.size() > 0){
            lastSecs = rpmTimeData.at(rpmTimeData.size()-1);
        }
        for(int time = GRAPH_RESOLUTION_MSECS; time < arcDurationMsecs; time+=GRAPH_RESOLUTION_MSECS){
            rpmTimeData.push_back(lastSecs+time/1000.0);
            rpmData.push_back(rpmArc.calculateY(lastSecs*1000.0 + time));
        }
    }

    //convert msec to secs


    m_rpmCurve->setSamples(rpmTimeData.data(), rpmData.data(), rpmTimeData.size());
    m_tempCurve->setSamples(tempTimeData.data(), temperatureData.data(), tempTimeData.size());
    m_plot->replot();

    qDebug() << "graph updated.";

}

void ExperimentGraph::pushTemperatureAndRPMData(double temp, double rpm, unsigned long timestamp)
{
    if(timestamp == 0){
        timestamp = Cell::getCurrentTimeMillis();
    }
    //push data
}


