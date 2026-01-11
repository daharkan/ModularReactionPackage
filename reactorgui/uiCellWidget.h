#ifndef UICELLWIDGET_H
#define UICELLWIDGET_H

#include <QWidget>
#include <string>
#include "ExperimentRunner.h"
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

    void setExperimentAndInit(Experiment experiment);
    void setCellId(const std::string &cellId);
    void pushTempAndRPMToCellGraph(float temp, int rpm);




    //View operations...
    void setCurrentExtTempView(float temp);
    void setCurrentInnerTempView(float temp);
    void setCurrentRPMView(int rpm);
    void setTargetTempView(float temp);
    void setTargetRPMView(int rpm);
    void setMotorAmpView(float amp);

public slots:
    void updateCell(Cell &cell);
    void updateExpState(ExperimentRunState state);

private:
    Ui::CellWidget *ui;
    Experiment m_assignedExperiment;
    CellGraph *m_cellGraph = nullptr;
    Cell m_cell;
    ExperimentRunner *m_expRunner;

    bool m_firstStartedRunning = false;

};

#endif // UICELLWIDGET_H
