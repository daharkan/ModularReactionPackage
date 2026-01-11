#include "uiCellOverviewWidget.h"
#include "ui_uiCellOverviewWidget.h"
#include <QDateTime>
#include <QMouseEvent>

CellOverviewWidget::CellOverviewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CellOverviewWidget)
{
    ui->setupUi(this);
    applyActiveStyle(false);
}

CellOverviewWidget::~CellOverviewWidget()
{
    delete ui;
}

void CellOverviewWidget::setSlotInfo(const QString &sideLabel, int slotIndex)
{
    m_sideLabel = sideLabel;
    m_slotIndex = slotIndex;
    updateHeader();
}

void CellOverviewWidget::setCellData(const Cell &cell)
{
    m_isActive = cell.isPlugged() && !cell.cellID().empty();
    m_cellId = cell.cellID();
    updateHeader();

    Experiment experiment = cell.asignedExperiment();
    QString expName = QString::fromStdString(experiment.name());
    if (expName.isEmpty()) {
        expName = "--";
    }
    QString assignedBy = QString::fromStdString(experiment.owner().username());
    if (assignedBy.isEmpty()) {
        assignedBy = "--";
    }

    QString assignedAt = "--";
    if (experiment.startSystemTimeMSecs() > 0) {
        QDateTime assignedDate = QDateTime::fromMSecsSinceEpoch(experiment.startSystemTimeMSecs());
        assignedAt = assignedDate.toString("yyyy-MM-dd HH:mm");
    }

    ui->experimentNameValueLabel->setText(expName);
    ui->assignedByValueLabel->setText(assignedBy);
    ui->assignedAtValueLabel->setText(assignedAt);

    float temp = cell.isExtTempPlugged() ? cell.currentTempExt() : cell.currentTempInner();
    ui->rpmValueLabel->setText(QString::number(cell.currentRPM()));
    ui->tempValueLabel->setText(QString::number(temp, 'f', 1));

    applyActiveStyle(m_isActive);
}

void CellOverviewWidget::setInactive()
{
    m_isActive = false;
    m_cellId.clear();
    updateHeader();

    ui->experimentNameValueLabel->setText("--");
    ui->assignedByValueLabel->setText("--");
    ui->assignedAtValueLabel->setText("--");
    ui->rpmValueLabel->setText("--");
    ui->tempValueLabel->setText("--");

    applyActiveStyle(false);
}

std::string CellOverviewWidget::cellId() const
{
    return m_cellId;
}

void CellOverviewWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_isActive) {
        return;
    }
    emit sgn_cellClicked(m_cellId);
    QWidget::mousePressEvent(event);
}

void CellOverviewWidget::updateHeader()
{
    QString header = QString("%1-%2").arg(m_sideLabel).arg(m_slotIndex);
    if (m_isActive && !m_cellId.empty()) {
        header = QString("%1 (%2)").arg(header, QString::fromStdString(m_cellId));
    } else {
        header = header + " (empty)";
    }
    ui->slotLabel->setText(header);
}

void CellOverviewWidget::applyActiveStyle(bool active)
{
    if (active) {
        setStyleSheet("");
        return;
    }

    setStyleSheet("background-color: rgb(46, 52, 54);"
                  "color: rgb(180, 180, 180);"
                  "border: 1px solid rgb(80, 80, 80);"
                  "border-radius: 6px;");
}
