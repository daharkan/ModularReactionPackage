#include "uiMainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startUpdateButton, &QPushButton::clicked, this, [this]() {
        if (m_updateTimer && !m_updateTimer->isActive()) {
            updateTable();
            m_updateTimer->start(500);
        }
    });
    connect(ui->senPushButton, &QPushButton::clicked, this, &MainWindow::sendTargetsClicked);

    RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateTable);

    ui->tableWidget->setRowCount(9);
    QStringList headers;
    headers << "Cell ID"
            << "Inner Temp"
            << "Ext. Temp"
            << "RPM"
            << "Target RPM"
            << "Target Temp"
            << "Stirrer Amp"
            << "Flow Rate (LPM)"
            << "Flow Temp";
    ui->tableWidget->setVerticalHeaderLabels(headers);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTable()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        return;
    }

    if (m_busboardId.empty()) {
        std::vector<std::string> busboardIds = RedisDBManager::getInstance()->getBusboardIds();
        if (!busboardIds.empty()) {
            m_busboardId = busboardIds.front();
        }
    }

    if (m_busboardId.empty()) {
        return;
    }

    std::vector<std::string> cellIDList = RedisDBManager::getInstance()->getBusboardCellIds(m_busboardId);
    m_cells = RedisDBManager::getInstance()->getCellList(cellIDList);

    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(static_cast<int>(cellIDList.size()));

    ui->cellNoComboBox->blockSignals(true);
    ui->cellNoComboBox->clear();
    for (int i = 0; i < static_cast<int>(cellIDList.size()); i++) {
        ui->cellNoComboBox->addItem(QString::number(i + 1));
    }
    ui->cellNoComboBox->blockSignals(false);

    for(int i = 0; i < m_cells.size(); i++){
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

        QTableWidgetItem* stirrerAmpItem = new QTableWidgetItem(QString::number(m_cells.at(i).stirrerMotorAmp()));
        ui->tableWidget->setItem(6, i, stirrerAmpItem);

        QTableWidgetItem* flowRateItem = new QTableWidgetItem(QString::number(m_cells.at(i).flowRateLpm(), 'f', 3));
        ui->tableWidget->setItem(7, i, flowRateItem);

        QTableWidgetItem* flowTempItem = new QTableWidgetItem(QString::number(m_cells.at(i).flowTemp(), 'f', 1));
        ui->tableWidget->setItem(8, i, flowTempItem);
    }
}

void MainWindow::sendTargetsClicked()
{
    float targetTemp = ui->targetTempLineEdit->text().toFloat();
    int targetRPM = ui->targetRPMLineEdit->text().toInt();
    int cellIdx = ui->cellNoComboBox->currentText().toInt()-1;

    if (cellIdx < 0 || cellIdx >= static_cast<int>(m_cells.size())) {
        return;
    }

    CellTarget celltarget;
    celltarget.setCellID(m_cells.at(cellIdx).cellID());
    celltarget.setTargetRPM(targetRPM);
    celltarget.setTargetTemp(targetTemp);
    celltarget.setTimestamp(Cell::getCurrentTimeMillis());
    RedisDBManager::getInstance()->pushCellTarget(celltarget);



}
