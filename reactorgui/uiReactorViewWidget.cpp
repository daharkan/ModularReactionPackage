#include "uiReactorViewWidget.h"
#include "ui_uiReactorViewWidget.h"
#include "RedisDBManager.h"
#include <QMap>

ReactorViewWidget::ReactorViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReactorViewWidget)
{
    ui->setupUi(this);

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    setupCellOverviewWidgets();

    m_flowTimer = new QTimer(this);
    connect(m_flowTimer, &QTimer::timeout, this, &ReactorViewWidget::updateFlowStatus);
    m_flowTimer->start(1000);
}

ReactorViewWidget::~ReactorViewWidget()
{
    delete ui;
}

void ReactorViewWidget::updateFlowStatus()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        return;
    }

    refreshBusboardIds();
    updateCellGroup(m_lhsBusboardId, m_lhsCells, "LHS");
    updateCellGroup(m_rhsBusboardId, m_rhsCells, "RHS");

    FlowStatus flowStatus = selectActiveFlowStatus();

    if (flowStatus.timestamp() == 0) {
        ui->flowRateValueLabel->setText("--");
        ui->flowTempValueLabel->setText("--");
        return;
    }

    ui->flowRateValueLabel->setText(QString::number(flowStatus.flowRateLpm(), 'f', 2));
    ui->flowTempValueLabel->setText(QString::number(flowStatus.flowTemp(), 'f', 1));
}

void ReactorViewWidget::handleCellClicked(const std::string &cellId, int positionIndex)
{
    if (cellId.empty()) {
        return;
    }
    emit sgn_openCellView(cellId, positionIndex);
}

void ReactorViewWidget::setupCellOverviewWidgets()
{
    m_lhsCells.clear();
    m_rhsCells.clear();

    for (int i = 0; i < 5; i++) {
        CellOverviewWidget *lhsWidget = new CellOverviewWidget(this);
        lhsWidget->setSlotInfo("LHS", i + 1);
        connect(lhsWidget, &CellOverviewWidget::sgn_cellClicked, this, &ReactorViewWidget::handleCellClicked);
        ui->lhsCellsLayout->addWidget(lhsWidget);
        m_lhsCells.push_back(lhsWidget);

        CellOverviewWidget *rhsWidget = new CellOverviewWidget(this);
        int displayIndex = 10 - i;
        rhsWidget->setSlotInfo("RHS", displayIndex);
        connect(rhsWidget, &CellOverviewWidget::sgn_cellClicked, this, &ReactorViewWidget::handleCellClicked);
        ui->rhsCellsLayout->addWidget(rhsWidget);
        m_rhsCells.push_back(rhsWidget);
    }
}

void ReactorViewWidget::refreshBusboardIds()
{
    std::vector<std::string> busboardIds = RedisDBManager::getInstance()->getBusboardIds();
    m_lhsBusboardId.clear();
    m_rhsBusboardId.clear();

    for (const auto &id : busboardIds) {
        QString upper = QString::fromStdString(id).toUpper();
        if (upper.contains("LHS") && m_lhsBusboardId.empty()) {
            m_lhsBusboardId = id;
        }
        if (upper.contains("RHS") && m_rhsBusboardId.empty()) {
            m_rhsBusboardId = id;
        }
    }
}

void ReactorViewWidget::updateCellGroup(const std::string &busboardId, QVector<CellOverviewWidget*> &widgets, const QString &sideLabel)
{
    QMap<int, Cell> cellsBySlot;
    if (!busboardId.empty()) {
        std::vector<std::string> cellIds = RedisDBManager::getInstance()->getBusboardCellIds(busboardId);
        if (!cellIds.empty()) {
            std::vector<Cell> cells = RedisDBManager::getInstance()->getCellList(cellIds);
            for (const auto &cell : cells) {
                int slot = cell.positionIdx();
                if (slot >= 1 && slot <= widgets.size()) {
                    cellsBySlot[slot] = cell;
                }
            }
        }
    }

    for (int i = 0; i < widgets.size(); i++) {
        int slotIndex = i + 1;
        if (sideLabel == "RHS") {
            slotIndex = widgets.size() - i;
        }
        CellOverviewWidget *widget = widgets.at(i);
        int displayIndex = slotIndex;
        if (sideLabel == "RHS") {
            displayIndex = 5 + slotIndex;
        }
        widget->setSlotInfo(sideLabel, displayIndex);
        if (cellsBySlot.contains(slotIndex)) {
            widget->setCellData(cellsBySlot.value(slotIndex));
        } else {
            widget->setInactive();
        }
    }
}

FlowStatus ReactorViewWidget::selectActiveFlowStatus() const
{
    FlowStatus lhsStatus;
    FlowStatus rhsStatus;
    if (!m_lhsBusboardId.empty()) {
        lhsStatus = RedisDBManager::getInstance()->getFlowStatus(m_lhsBusboardId);
    }
    if (!m_rhsBusboardId.empty()) {
        rhsStatus = RedisDBManager::getInstance()->getFlowStatus(m_rhsBusboardId);
    }

    auto hasMeaningfulData = [](const FlowStatus &status) {
        if (status.timestamp() == 0) {
            return false;
        }
        return status.flowRateLpm() > 0.0 || status.flowTemp() > 0.0;
    };

    bool lhsActive = hasMeaningfulData(lhsStatus);
    bool rhsActive = hasMeaningfulData(rhsStatus);

    if (lhsActive && rhsActive) {
        if (rhsStatus.timestamp() >= lhsStatus.timestamp()) {
            return rhsStatus;
        }
        return lhsStatus;
    }
    if (rhsActive) {
        return rhsStatus;
    }
    return lhsStatus;
}
