#ifndef UICELLWIDGET_H
#define UICELLWIDGET_H

#include <QWidget>
#include <string>
#include "uiCellGraph.h"

namespace Ui {
class CellWidget;
}

class CellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellWidget(QWidget *parent = nullptr);
    ~CellWidget();

    void setPositionIndex(int positionIndex);
    void setExperimentAndInit(Experiment experiment);
    void pushTempAndRPMToCellGraph(float temp, int rpm);




    //View operations...
    void setCurrentExtTempView(float temp);
    void setCurrentBlockTempView(float temp);
    void setCurrentRPMView(int rpm);
    void setTargetTempView(float temp);
    void setTargetRPMView(int rpm);
    void setMotorAmpView(float amp);

public slots:
    void updateCell(Cell &cell);

signals:
    void sgn_assignExperimentRequested(const std::string &cellId);

private:
    Ui::CellWidget *ui;
    Experiment m_assignedExperiment;
    CellGraph *m_cellGraph = nullptr;
    Cell m_cell;
    int m_positionIndex = 0;

    bool m_firstStartedRunning = false;
    std::string m_loadedHistoryExperimentId;
    unsigned long long m_lastHistorySyncMs = 0;

    void ensureExperimentGraph(const Experiment &experiment);
    void loadVisualHistoryIfNeeded(const Experiment &experiment);
    void clearExperimentGraph();
    bool hasExperimentAssigned(const Experiment &experiment) const;
    void updateTargetSyncStatus(const Cell &cell);

};

#endif // UICELLWIDGET_H
