#include "uiMainWindow.h"
#include "ui_uiMainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OnlyCellSerialManager::getInstance()->connectAndAssignThePort();

    m_cellWidget = new CellWidget;
    ui->cellLayout->addWidget(m_cellWidget);
    connect(OnlyCellSerialManager::getInstance(), &OnlyCellSerialManager::sgn_updatePIDParameters, this, &MainWindow::updatePIDParametersView);
    connect(OnlyCellSerialManager::getInstance(), &OnlyCellSerialManager::sgn_updateCell, m_cellWidget, &CellWidget::updateCell);

    connect(ui->assignExp1Button, &QPushButton::clicked, this, &MainWindow::assignExp1Clicked);
    connect(ui->updatePIDParametersButton, &QPushButton::clicked, this, &MainWindow::updatePIDParametersOnBoardClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}


TempArc MainWindow::createLinearTempArc(float startTemp, float endTemp, float rank, unsigned long startTimeMSec)
{
    unsigned long linearDurationMSec = abs((endTemp - startTemp) / rank) * 1000.0;
    double startTimeSec = startTimeMSec / 1000.0;

    qDebug() << " ExperimentCreateWidget::createLinearTempArc:: endTemp: " << endTemp;
    if(endTemp < startTemp){
        rank = -rank;
    }

    return TempArc(0, rank, startTemp - (rank * startTimeSec), startTimeMSec, linearDurationMSec);

}

TempArc MainWindow::createPlateuTempArc(float startTemp, unsigned long startTimeMSec, unsigned long durationMSec)
{
    return TempArc(0, 0, startTemp, startTimeMSec, durationMSec);
}




///BASIC EXPERIMENT PROFILES
Profile MainWindow::createBasicStandardProfile(int type)
{
    Profile profile;


    float targetTemp = 0;
    float stepUpRank = 0;
    float stepDownRank = 0;


    int numOfCycles = 0;

    float initialTemp = 0;
    float finalTemp = 0;

    unsigned long initialHoldingTimeMSecs = 0;
    unsigned long platueTimeMSecs = 0;
    unsigned long finalHoldingTimeMSecs = 0;


    if(type == 1){
        targetTemp = -60;
        stepUpRank = 10 / 60.0;  //converting to C/sec
        stepDownRank = 10 / 60.0;  //converting to C/sec


        numOfCycles = 1;

        initialTemp = 15;
        finalTemp = -40;

        initialHoldingTimeMSecs = 120 * 1000;
        platueTimeMSecs = 180 * 1000;
        finalHoldingTimeMSecs = 120 * 1000;
    }



    unsigned long startTimeMSec = 0;
    for(int i = 0; i < numOfCycles; i++){

        qDebug() << "startTimeMSec " << startTimeMSec;

        //ARC 1
       /* TempArc tempArc1(createPlateuTempArc(initialTemp, startTimeMSec, initialHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc1);



        float lastTemp = tempArc1.finishTemp();
        unsigned long arcStartTime = tempArc1.finishTimeMsec();

        qDebug() << "arcStartTime after  tempArc1 " << arcStartTime;
        qDebug() << "lastTemp after  tempArc1 " << lastTemp;
*/

        //ARC 2
        TempArc tempArc2(createLinearTempArc(initialTemp, targetTemp, stepUpRank, startTimeMSec));
        profile.addTempArcInSequence(tempArc2);




        float lastTemp = tempArc2.finishTemp();
        unsigned long arcStartTime = tempArc2.finishTimeMsec();
        //ARC 3
        TempArc tempArc3(createPlateuTempArc(lastTemp, arcStartTime, platueTimeMSecs));
        profile.addTempArcInSequence(tempArc3);




        lastTemp = tempArc3.finishTemp();
        arcStartTime = tempArc3.finishTimeMsec();
        //ARC 4
        TempArc tempArc4(createLinearTempArc(lastTemp, finalTemp, stepDownRank, arcStartTime));
        profile.addTempArcInSequence(tempArc4);



        lastTemp = tempArc4.finishTemp();
        arcStartTime = tempArc4.finishTimeMsec();
        //ARC 5
        TempArc tempArc5( createPlateuTempArc(lastTemp, arcStartTime, finalHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc5);

        startTimeMSec = tempArc5.finishTimeMsec();


    }
    return profile;
}



void MainWindow::assignExp1Clicked()
{
    Experiment experiment;
    experiment.setProfile(createBasicStandardProfile(1));
    m_cellWidget->setExperimentAndInit(experiment);

}

void MainWindow::assignExp2Clicked()
{

}

void MainWindow::assignExp3Clicked()
{

}








void MainWindow::updatePIDParametersView(int heaterPerc, int peltierPerc)
{
    ui->heaterDutyCLabel->setText(QString::number(heaterPerc));
    ui->peltierDutyCLabel->setText(QString::number(peltierPerc));
}

void MainWindow::updatePIDParametersOnBoardClicked()
{
    float kP = ui->pidKpLineEdit->text().toFloat();
    float ki = ui->pidKiLineEdit->text().toFloat();
    float kD = ui->pidKdLineEdit->text().toFloat();

    QString command = "pid#" + QString::number(kP) + "#" + QString::number(ki) + "#" + QString::number(kD);

    OnlyCellSerialManager::getInstance()->writeString2Queue(command);
}
