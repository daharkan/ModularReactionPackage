#include "uiMainWindow.h"
#include "ui_uiMainWindow.h"

#define DEFAULT_REDIS_HOST "127.0.0.1"
#define DEFAULT_REDIS_PORT 6379

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_cellWidget = new CellWidget;
    ui->cellLayout->addWidget(m_cellWidget);

    connect(ui->assignExp1Button, &QPushButton::clicked, this, &MainWindow::assignExp1Clicked);
    connect(ui->updatePIDParametersButton, &QPushButton::clicked, this, &MainWindow::updatePIDParametersOnBoardClicked);

    bool connected = RedisDBManager::getInstance()->connectToDB(DEFAULT_REDIS_HOST, DEFAULT_REDIS_PORT);
    if (!connected) {
        qDebug() << "Redis connection failed.";
    }

    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &MainWindow::pollCellStatus);
    m_pollTimer->start(200);

    ui->updatePIDParametersButton->setEnabled(false);
    ui->updatePIDParametersButton->setToolTip("PID updates are handled by ExperimentManagerService.");
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
    qDebug() << "PID updates are managed by ExperimentManagerService.";
}

void MainWindow::pollCellStatus()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        return;
    }

    if (m_busboardId.empty()) {
        std::vector<std::string> busboardIds = RedisDBManager::getInstance()->getBusboardIds();
        if (busboardIds.empty()) {
            return;
        }
        m_busboardId = busboardIds.front();
    }

    if (m_firstCellId.empty()) {
        std::vector<std::string> cellIds = RedisDBManager::getInstance()->getBusboardCellIds(m_busboardId);
        if (cellIds.empty()) {
            return;
        }
        m_firstCellId = cellIds.front();
        m_cellWidget->setCellId(m_firstCellId);
    }

    std::vector<std::string> cellIds = {m_firstCellId};
    std::vector<Cell> cells = RedisDBManager::getInstance()->getCellList(cellIds);
    if (cells.empty()) {
        return;
    }

    m_cellWidget->updateCell(cells.front());
}
