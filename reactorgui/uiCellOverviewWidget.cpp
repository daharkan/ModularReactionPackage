#include "uiCellOverviewWidget.h"
#include "ui_uiCellOverviewWidget.h"
#include <QDateTime>
#include <QMenu>
#include <QMouseEvent>
#include <QContextMenuEvent>

CellOverviewWidget::CellOverviewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CellOverviewWidget)
{
    ui->setupUi(this);
    m_stateKey = "empty";
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

    Experiment experiment = cell.asignedExperiment();
    QString expName = QString::fromStdString(experiment.name());
    if (expName.isEmpty()) {
        expName = "--";
    }
    QString assignedBy = QString::fromStdString(experiment.owner().username());
    if (assignedBy.isEmpty()) {
        assignedBy = "--";
    }

    QString stateKey = QString::fromStdString(experiment.state()).toLower();
    bool isCompleted = stateKey == "completed";
    ui->finishesAtLabel->setText(isCompleted ? tr("Finished at:") : tr("Finishes at:"));

    QString assignedAt = "--";
    if (experiment.startSystemTimeMSecs() > 0) {
        QDateTime assignedDate = QDateTime::fromMSecsSinceEpoch(experiment.startSystemTimeMSecs());
        assignedAt = assignedDate.toString("yyyy-MM-dd HH:mm");
    }

    QString finishesAt = "--";
    unsigned long long totalDurationMs = experiment.totalProfileDurationMSecs();
    if (totalDurationMs == 0) {
        const auto &arcs = experiment.profile().tempArcsInSeq();
        for (const auto &arc : arcs) {
            totalDurationMs += arc.durationMSec();
        }
    }
    if (experiment.startSystemTimeMSecs() > 0 && totalDurationMs > 0) {
        unsigned long long startMs = experiment.startSystemTimeMSecs();
        unsigned long long finishMs = startMs + totalDurationMs;
        if (isCompleted) {
            qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
            unsigned long long adjustedDuration = totalDurationMs;
            for (int i = 0; i < 2; ++i) {
                if (finishMs <= static_cast<unsigned long long>(nowMs)) {
                    break;
                }
                if (adjustedDuration < 1000ULL) {
                    break;
                }
                adjustedDuration /= 1000ULL;
                finishMs = startMs + adjustedDuration;
            }
        }
        QDateTime finishDate = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(finishMs));
        finishesAt = finishDate.toString("yyyy-MM-dd HH:mm");
    }

    ui->experimentNameValueLabel->setText(expName);
    ui->assignedByValueLabel->setText(assignedBy);
    ui->assignedAtValueLabel->setText(assignedAt);
    ui->finishesAtValueLabel->setText(finishesAt);

    ui->rpmValueLabel->setText(QString::number(cell.currentRPM()));
    ui->tempValueLabel->setText(QString("Ext %1 / Blk %2")
                                    .arg(cell.currentTempExt(), 0, 'f', 1)
                                    .arg(cell.currentTempInner(), 0, 'f', 1));
    ui->heaterDutyValueLabel->setText(cell.heaterDutyPercent() < 0 ? "--" : QString::number(cell.heaterDutyPercent()));
    ui->peltierDutyValueLabel->setText(cell.peltierDutyPercent() < 0 ? "--" : QString::number(cell.peltierDutyPercent()));

    if (stateKey.isEmpty()) {
        m_stateKey = "active";
        applyActiveStyle(true);
        updateHeader();
        return;
    }

    applyStateStyle(stateKey);
    updateHeader();
}

void CellOverviewWidget::setInactive()
{
    m_isActive = false;
    m_cellId.clear();

    ui->experimentNameValueLabel->setText("--");
    ui->assignedByValueLabel->setText("--");
    ui->assignedAtValueLabel->setText("--");
    ui->finishesAtLabel->setText(tr("Finishes at:"));
    ui->finishesAtValueLabel->setText("--");
    ui->rpmValueLabel->setText("--");
    ui->tempValueLabel->setText("--");
    ui->heaterDutyValueLabel->setText("--");
    ui->peltierDutyValueLabel->setText("--");

    setSelected(false);
    applyStateStyle("empty");
    updateHeader();
}

void CellOverviewWidget::setSelected(bool selected)
{
    if (m_isSelected == selected) {
        return;
    }
    m_isSelected = selected;
    if (m_stateKey.isEmpty()) {
        applyActiveStyle(m_isActive);
        return;
    }
    applyStateStyle(m_stateKey);
}

std::string CellOverviewWidget::cellId() const
{
    return m_cellId;
}

void CellOverviewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isActive) {
        emit sgn_cellSelected(m_cellId, m_slotIndex);
        event->ignore();
        return;
    }
    QWidget::mousePressEvent(event);
}

void CellOverviewWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_isActive) {
        return;
    }
    emit sgn_cellClicked(m_cellId, m_slotIndex);
    QWidget::mouseDoubleClickEvent(event);
}

void CellOverviewWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_isActive || m_cellId.empty()) {
        return;
    }

    QMenu menu(this);
    QAction *assignAction = menu.addAction(tr("Assign Experiment..."));
    QAction *selectedAction = menu.exec(event->globalPos());
    if (selectedAction == assignAction) {
        emit sgn_assignExperimentRequested(m_cellId, m_slotIndex);
    }
}

void CellOverviewWidget::updateHeader()
{
    QString headerLeft = QString("Cell %1").arg(m_slotIndex);
    QString stateLabel;
    if (!m_stateKey.isEmpty() && m_stateKey != "empty" && m_stateKey != "active") {
        stateLabel = m_stateKey.toUpper();
    }
    if (m_isActive && !m_cellId.empty()) {
        setToolTip(QString::fromStdString(m_cellId));
    } else {
        setToolTip(QString());
    }
    ui->slotLabel->setTextFormat(Qt::PlainText);
    ui->slotLabel->setText(headerLeft);
    ui->stateLabel->setTextFormat(Qt::PlainText);
    ui->stateLabel->setText(stateLabel);
}

void CellOverviewWidget::applyActiveStyle(bool active)
{
    const QString selectedBorder = "rgb(33, 50, 104)";
    auto applyStyle = [&](const QString &background, const QString &text, const QString &border) {
        QString borderColor = m_isSelected ? selectedBorder : border;
        setStyleSheet(QString("background-color: %1;"
                              "color: %2;"
                              "border: 2px solid %3;"
                              "border-radius: 6px;")
                          .arg(background, text, borderColor));
    };

    if (active) {
        applyStyle("rgb(241, 245, 251)", "rgb(33, 50, 104)", "rgb(199, 212, 234)");
        return;
    }

    applyStyle("rgb(238, 242, 248)", "rgb(152, 164, 186)", "rgb(214, 224, 239)");
}

void CellOverviewWidget::applyStateStyle(const QString &stateKey)
{
    m_stateKey = stateKey;
    if (stateKey == "empty") {
        applyActiveStyle(false);
        return;
    }

    const QString selectedBorder = "rgb(33, 50, 104)";
    auto applyStyle = [&](const QString &background, const QString &text, const QString &border) {
        QString borderColor = m_isSelected ? selectedBorder : border;
        setStyleSheet(QString("background-color: %1;"
                              "color: %2;"
                              "border: 2px solid %3;"
                              "border-radius: 6px;")
                          .arg(background, text, borderColor));
    };

    if (stateKey == "unplugged") {
        applyStyle("rgb(21, 29, 51)", "rgb(144, 164, 199)", "rgb(38, 51, 86)");
        return;
    }

    if (stateKey == "idle") {
        applyStyle("rgb(246, 249, 253)", "rgb(33, 50, 104)", "rgb(214, 224, 241)");
        return;
    }

    if (stateKey == "assigned") {
        applyStyle("rgb(215, 241, 232)", "rgb(31, 92, 74)", "rgb(167, 222, 200)");
        return;
    }

    if (stateKey == "preheat") {
        applyStyle("rgb(225, 241, 238)", "rgb(33, 92, 82)", "rgb(182, 227, 210)");
        return;
    }

    if (stateKey == "running") {
        applyStyle("rgb(189, 232, 215)", "rgb(28, 88, 71)", "rgb(125, 205, 176)");
        return;
    }

    if (stateKey == "completed") {
        applyStyle("rgb(219, 229, 246)", "rgb(33, 50, 104)", "rgb(177, 195, 230)");
        return;
    }

    applyActiveStyle(true);
}
