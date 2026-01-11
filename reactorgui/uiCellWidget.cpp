#include "uiCellWidget.h"
#include "ui_uiCellWidget.h"
#include <QtConcurrent>

CellWidget::CellWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CellWidget)
{
    ui->setupUi(this);

    m_expRunner = new ExperimentRunner;
    connect(m_expRunner, &ExperimentRunner::sgn_updateExperimentState, this, &CellWidget::updateExpState);

}

CellWidget::~CellWidget()
{
    delete ui;
}

void CellWidget::setCurrentExtTempView(float temp)
{
    ui->currentExtTempLabel->setText(QString::number(temp));
}


void CellWidget::setCurrentInnerTempView(float temp)
{
    ui->currentInnerTempLabel->setText(QString::number(temp));
}



void CellWidget::setCurrentRPMView(int rpm)
{
    ui->currentRPMLabel->setText(QString::number(rpm));
}




void CellWidget::setTargetTempView(float temp)
{
    ui->targetTempLabel->setText(QString::number(temp));
}




void CellWidget::setTargetRPMView(int rpm)
{
    ui->targetRPMLabel->setText(QString::number(rpm));
}




void CellWidget::setMotorAmpView(float amp)
{
    ui->motorAmpLabel->setText(QString::number(amp));
}

void CellWidget::pushTempAndRPMToCellGraph(float temp, int rpm)
{
    if(m_cellGraph == nullptr){
        qDebug() << "no cell graph initilized...";
        return;
    }

    if(m_cellGraph->isDataPushStarted()){
        m_cellGraph->pushTemperatureAndRPMData(temp, rpm);
    }

}

void CellWidget::setExperimentAndInit(Experiment experiment)
{
    m_firstStartedRunning = false;
    m_assignedExperiment = experiment;
    m_cellGraph = new CellGraph(m_assignedExperiment);

    ui->graphLayput->addWidget(m_cellGraph);

    m_expRunner->assignExperiment(m_assignedExperiment);
    QtConcurrent::run([this]() {
        m_expRunner->run();  // Sınıf üyesi fonksiyonunu çağırıyoruz
    });
}

void CellWidget::setCellId(const std::string &cellId)
{
    m_expRunner->setCellId(cellId);
}



void CellWidget::updateExpState(ExperimentRunState state)
{
    QString expState = "";
    if(state == STATE_INITILIZED){
        expState = "INITILIZED";
    }else if(state == STATE_PREHEAT){
        expState = "PREHEAT";
    }else if(state == STATE_RUNNING){
        if(!m_firstStartedRunning){
            m_firstStartedRunning = true;
            m_cellGraph->initilizeExperimentGraph();
        }
        expState = "RUNNING";
    }else if(state == STATE_COMPLETED){
        expState = "COMPLETED";
    }

    ui->expStateLabel->setText(expState);

}

void CellWidget::updateCell(Cell &cell)
{
    m_cell = cell;
    m_expRunner->setCellId(cell.cellID());

    float currentTempExt = cell.currentTempExt();
    float currentTempInner = cell.currentTempInner();
    int currentRPM = cell.currentRPM();
    float targetTemp = cell.assignedTemp();
    int targetRPM = cell.assignedRPM();

    qDebug() << ">>>> cell updated  targetTemp: " << targetTemp << ", currentTempExt: " << currentTempExt;
    setTargetRPMView(targetRPM);
    setTargetTempView(targetTemp);
    setCurrentExtTempView(currentTempExt);
    setCurrentInnerTempView(currentTempInner);
    setCurrentRPMView(currentRPM);

    pushTempAndRPMToCellGraph(currentTempExt, currentRPM);

}
