#include "uiCellOverviewWidget.h"
#include "ui_uiCellOverviewWidget.h"
#include <QDateTime>
#include <QMouseEvent>

namespace {
constexpr unsigned long long kPreheatWindowMs = 2ULL * 60ULL * 1000ULL;

unsigned long long experimentDurationMs(Experiment &experiment)
{
    unsigned long long total = 0;
    Profile profile = experiment.profile();
    for (const auto &arc : profile.tempArcsInSeq()) {
        total += arc.durationMSec();
    }
    return total;
}
} // namespace

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

    if (!cell.isPlugged() || cell.cellID().empty()) {
        applyStateStyle("unplugged");
        return;
    }

    bool hasExperiment = !experiment.experimentId().empty() || !experiment.name().empty();
    if (!hasExperiment) {
        applyStateStyle("idle");
        return;
    }

    unsigned long long totalDurationMs = experimentDurationMs(experiment);
    unsigned long long startMs = experiment.startSystemTimeMSecs();
    if (startMs == 0 || totalDurationMs == 0) {
        applyStateStyle("assigned");
        return;
    }

    unsigned long long nowMs = Cell::getCurrentTimeMillis();
    unsigned long long elapsedMs = nowMs > startMs ? nowMs - startMs : 0;
    if (elapsedMs < kPreheatWindowMs) {
        applyStateStyle("preheat");
    } else if (elapsedMs < totalDurationMs) {
        applyStateStyle("running");
    } else {
        applyStateStyle("completed");
    }
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

    applyStateStyle("empty");
}

std::string CellOverviewWidget::cellId() const
{
    return m_cellId;
}

void CellOverviewWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_isActive) {
        return;
    }
    emit sgn_cellClicked(m_cellId);
    QWidget::mouseDoubleClickEvent(event);
}

void CellOverviewWidget::updateHeader()
{
    QString header = QString("Cell %1").arg(m_slotIndex);
    if (m_isActive && !m_cellId.empty()) {
        setToolTip(QString::fromStdString(m_cellId));
    } else {
        setToolTip(QString());
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

void CellOverviewWidget::applyStateStyle(const QString &stateKey)
{
    if (stateKey == "empty") {
        applyActiveStyle(false);
        return;
    }

    if (stateKey == "unplugged") {
        setStyleSheet("background-color: rgb(60, 60, 60);"
                      "color: rgb(200, 200, 200);"
                      "border: 1px solid rgb(100, 100, 100);"
                      "border-radius: 6px;");
        return;
    }

    if (stateKey == "idle") {
        setStyleSheet("background-color: rgb(224, 224, 224);"
                      "color: rgb(30, 30, 30);"
                      "border: 1px solid rgb(180, 180, 180);"
                      "border-radius: 6px;");
        return;
    }

    if (stateKey == "assigned") {
        setStyleSheet("background-color: rgb(186, 225, 255);"
                      "color: rgb(20, 30, 40);"
                      "border: 1px solid rgb(120, 170, 210);"
                      "border-radius: 6px;");
        return;
    }

    if (stateKey == "preheat") {
        setStyleSheet("background-color: rgb(255, 204, 128);"
                      "color: rgb(50, 30, 0);"
                      "border: 1px solid rgb(220, 150, 70);"
                      "border-radius: 6px;");
        return;
    }

    if (stateKey == "running") {
        setStyleSheet("background-color: rgb(186, 234, 190);"
                      "color: rgb(20, 60, 20);"
                      "border: 1px solid rgb(120, 190, 120);"
                      "border-radius: 6px;");
        return;
    }

    if (stateKey == "completed") {
        setStyleSheet("background-color: rgb(200, 210, 255);"
                      "color: rgb(20, 20, 60);"
                      "border: 1px solid rgb(130, 140, 200);"
                      "border-radius: 6px;");
        return;
    }

    applyActiveStyle(true);
}
