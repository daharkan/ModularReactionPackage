#include "uiMainWindow.h"
#include "ui_mainwindow.h"

#define BUSBOARD_ID "bb000"
void delay(int msec)
{

    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::milliseconds(msec);

    while (std::chrono::steady_clock::now() < endTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small sleep to prevent high CPU usage
    }

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startUpdateButton, &QPushButton::clicked, this, &MainWindow::updateTable);
    connect(ui->senPushButton, &QPushButton::clicked, this, &MainWindow::sendTargetsClicked);

    bool succ = RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    qDebug() << "succ: " << succ;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTable()
{
    std::vector<std::string> cellIDList = RedisDBManager::getInstance()->getBusboardCellIds("bb000");

    while(1){
        QApplication::processEvents();
        //delay(50);

        m_cells = RedisDBManager::getInstance()->getCellList(cellIDList);

        ui->tableWidget->clearContents();
        for(int i = 0; i < cellIDList.size(); i++){

            if(m_cells.at(0).stirrerMotorAmp() != m_prevVal){
                unsigned long tm = Cell::getCurrentTimeMillis();
                std::cout << "time: " <<  tm - m_prevTime << std::endl;
                m_prevTime = tm;
            }
            QTableWidgetItem* cellIDItem = new QTableWidgetItem(QString::fromStdString(m_cells.at(i).cellID()));
            ui->tableWidget->setItem(0, i, cellIDItem);

            QTableWidgetItem* innerTempItem = new QTableWidgetItem(QString::number(m_cells.at(i).currentTempInner()));
            ui->tableWidget->setItem(1, i, innerTempItem);

            QTableWidgetItem* extTempItem = new QTableWidgetItem(QString::number(m_cells.at(i).currentTempExt()));
            ui->tableWidget->setItem(2, i, extTempItem);

            QTableWidgetItem* rpmItem = new QTableWidgetItem(QString::number(m_cells.at(i).currentRPM()));
            ui->tableWidget->setItem(3, i, rpmItem);

            QTableWidgetItem* targetRPMItem = new QTableWidgetItem(QString::number(m_cells.at(i).assignedRPM()));
            ui->tableWidget->setItem(4, i, targetRPMItem);


            QTableWidgetItem* targetTempItem = new QTableWidgetItem(QString::number(m_cells.at(i).assignedTemp()));
            ui->tableWidget->setItem(5, i, targetTempItem);

            m_prevVal = m_cells.at(0).stirrerMotorAmp();



        }
    }
}

void MainWindow::sendTargetsClicked()
{
    float targetTemp = ui->targetTempLineEdit->text().toFloat();
    int targetRPM = ui->targetRPMLineEdit->text().toInt();
    int cellIdx = ui->cellNoComboBox->currentText().toInt()-1;
    int motorSelect = ui->motorSelectComboBox->currentIndex();


    CellTarget celltarget;
    celltarget.setCellID(m_cells.at(cellIdx).cellID());
    celltarget.setTargetRPM(targetRPM);
    celltarget.setTargetTemp(targetTemp);
    celltarget.setMotorSelect(motorSelect);
    celltarget.setTimestamp(Cell::getCurrentTimeMillis());
    RedisDBManager::getInstance()->pushCellTarget(celltarget);



}
