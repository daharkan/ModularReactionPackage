#include "uiTempCellViewWidget.h"

#include <QVBoxLayout>

#include "RedisDBManager.h"

TempCellViewWidget::TempCellViewWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_stackLayout = new QStackedLayout();
    m_stackLayout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(m_stackLayout);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &TempCellViewWidget::refreshCellData);
    m_updateTimer->start(500);

    ensureRedisConnection();
}

TempCellViewWidget::~TempCellViewWidget()
{
}

void TempCellViewWidget::setCellInfo(const std::string &cellId, int positionIndex)
{
    m_cellId = cellId;
    m_positionIndex = positionIndex;
    CellWidget *cellWidget = ensureCellWidget(cellId);
    if (cellWidget == nullptr) {
        return;
    }
    cellWidget->setPositionIndex(positionIndex);
    m_stackLayout->setCurrentWidget(cellWidget);
    refreshCellData();
}

std::string TempCellViewWidget::cellId() const
{
    return m_cellId;
}

int TempCellViewWidget::positionIndex() const
{
    return m_positionIndex;
}

void TempCellViewWidget::refreshCellData()
{
    if (m_cellId.empty()) {
        return;
    }

    CellWidget *cellWidget = ensureCellWidget(m_cellId);
    if (cellWidget == nullptr) {
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        ensureRedisConnection();
        if (!RedisDBManager::getInstance()->isConnected()) {
            return;
        }
    }

    std::vector<std::string> ids = {m_cellId};
    std::vector<Cell> cells = RedisDBManager::getInstance()->getCellList(ids);
    if (cells.empty()) {
        return;
    }

    cellWidget->updateCell(cells.front());
}

void TempCellViewWidget::ensureRedisConnection()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }
}

CellWidget *TempCellViewWidget::ensureCellWidget(const std::string &cellId)
{
    if (cellId.empty()) {
        return nullptr;
    }

    const QString key = QString::fromStdString(cellId);
    auto it = m_cellWidgets.find(key);
    if (it != m_cellWidgets.end()) {
        return it.value();
    }

    auto *cellWidget = new CellWidget(this);
    m_stackLayout->addWidget(cellWidget);
    m_cellWidgets.insert(key, cellWidget);
    connect(cellWidget, &CellWidget::sgn_assignExperimentRequested,
            this, &TempCellViewWidget::sgn_assignExperimentRequested);
    return cellWidget;
}
