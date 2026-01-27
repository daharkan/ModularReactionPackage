#ifndef UIEXPERIMENTGRAPH_H
#define UIEXPERIMENTGRAPH_H

#include <QWidget>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>

#include "Experiment.h"
#include "Cell.h"

#define GRAPH_RESOLUTION_MSECS 200

namespace Ui {
class ExperimentGraph;
}

class ExperimentGraph : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentGraph(Experiment &experiment, QWidget *parent = nullptr);
    ~ExperimentGraph();
    void updateTheExperiment(Experiment& experiment);
    void updatePreviewProfile(const Profile &profile);
    void pushTemperatureAndRPMData(double temp, double rpm, unsigned long timestamp = 0);

private:
    Ui::ExperimentGraph *ui;
    QwtPlot *m_plot;
    QwtPlotCurve *m_tempCurve, *m_rpmCurve, *m_tempPreviewCurve;
    QwtLegend *m_legend;

};

#endif // UIEXPERIMENTGRAPH_H
