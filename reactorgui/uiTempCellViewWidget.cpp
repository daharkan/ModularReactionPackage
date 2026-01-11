#include "uiTempCellViewWidget.h"

#include <QVBoxLayout>

#include "RedisDBManager.h"

TempCellViewWidget::TempCellViewWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_cellWidget = new CellWidget(this);
    layout->addWidget(m_cellWidget);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &TempCellViewWidget::refreshCellData);
    m_updateTimer->start(500);

    ensureRedisConnection();
}

TempCellViewWidget::~TempCellViewWidget()
{
}

void TempCellViewWidget::setCellId(const std::string &cellId)
{
    m_cellId = cellId;
    m_cellWidget->setCellId(cellId);
    refreshCellData();
}

std::string TempCellViewWidget::cellId() const
{
    return m_cellId;
}

void TempCellViewWidget::refreshCellData()
{
    if (m_cellId.empty()) {
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

    m_cellWidget->updateCell(cells.front());
}

void TempCellViewWidget::ensureRedisConnection()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }
}
