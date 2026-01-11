#ifndef UICellGraph_H
#define UICellGraph_H

#include <QWidget>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>

#include "Experiment.h"
#include "Cell.h"
#include "CellVisualsHistory.h"

#define GRAPH_RESOLUTION_MSECS 300

namespace Ui {
class CellGraph;
}

class CellGraph : public QWidget
{
    Q_OBJECT

public:
    explicit CellGraph(Experiment &experiment, QWidget *parent = nullptr);
    ~CellGraph();
    void updateTheExperiment(Experiment& experiment);
    void pushTemperatureAndRPMData(double temp, double rpm, unsigned long timestamp = 0);
    void initilizeExperimentGraph();
    void loadVisualHistory(const CellVisualsHistory& history);

    bool isDataPushStarted()
    {
        return m_pushingDataStarted;
    }

private:
    Ui::CellGraph *ui;
    QwtPlot *m_plot;
    QwtPlotCurve *m_expTempCurve, *m_expRpmCurve;
    QwtPlotCurve *m_currentTempCurve, *m_currentRpmCurve;
    QwtLegend *m_legend;
    bool m_pushingDataStarted = false;
    unsigned long m_dataPushStartTimestamp = 0;
    bool m_hasTempRange = false;
    double m_minTempRange = 0.0;
    double m_maxTempRange = 0.0;

    QList<double> m_currentTempList;
    QList<double> m_currentRPMList;
    QList<unsigned long > m_currentTimestampList;
    Experiment m_experiment;

    void applyTempAxisRange();
};

#endif // UICellGraph_H
