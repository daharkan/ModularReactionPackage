#include "uiCellWidget.h"
#include "ui_uiCellWidget.h"
#include "RedisDBManager.h"
#include <QDateTime>

namespace {
QString formatDurationMs(unsigned long long durationMs)
{
    unsigned long long totalSeconds = durationMs / 1000ULL;
    unsigned long long hours = totalSeconds / 3600ULL;
    unsigned long long minutes = (totalSeconds % 3600ULL) / 60ULL;
    unsigned long long seconds = totalSeconds % 60ULL;
    if (hours > 0) {
        return QString("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
    }
    return QString("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}
} // namespace

CellWidget::CellWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CellWidget)
{
    ui->setupUi(this);
    if (ui->assignExperimentButton != nullptr) {
        connect(ui->assignExperimentButton, &QPushButton::clicked, this, [this]() {
            if (!m_cell.cellID().empty()) {
                emit sgn_assignExperimentRequested(m_cell.cellID());
            }
        });
    }

}

CellWidget::~CellWidget()
{
    delete ui;
}

void CellWidget::setCurrentExtTempView(float temp)
{
    ui->currentExtTempLabel->setText(QString::number(temp, 'f', 1));
}


void CellWidget::setCurrentBlockTempView(float temp)
{
    ui->currentInnerTempLabel->setText(QString::number(temp, 'f', 1));
}



void CellWidget::setCurrentRPMView(int rpm)
{
    ui->currentRPMLabel->setText(QString::number(rpm));
}




void CellWidget::setTargetTempView(float temp)
{
    ui->targetTempLabel->setText(QString::number(temp, 'f', 1));
}




void CellWidget::setTargetRPMView(int rpm)
{
    ui->targetRPMLabel->setText(QString::number(rpm));
}




void CellWidget::setMotorAmpView(float amp)
{
    ui->motorAmpLabel->setText(QString::number(amp));
}

void CellWidget::pushTempAndRPMToCellGraph(float temp, int rpm)
{
    if(m_cellGraph == nullptr){
        qDebug() << "no cell graph initilized...";
        return;
    }

    if(m_cellGraph->isDataPushStarted()){
        m_cellGraph->pushTemperatureAndRPMData(temp, rpm);
    }

}

void CellWidget::setExperimentAndInit(Experiment experiment)
{
    m_firstStartedRunning = false;
    m_assignedExperiment = experiment;
    if (m_cellGraph == nullptr) {
        m_cellGraph = new CellGraph(m_assignedExperiment, this);
        ui->graphLayput->addWidget(m_cellGraph);
    } else {
        m_cellGraph->updateTheExperiment(m_assignedExperiment);
    }

}

void CellWidget::setPositionIndex(int positionIndex)
{
    m_positionIndex = positionIndex;
}

void CellWidget::updateCell(Cell &cell)
{
    m_cell = cell;
    if (ui->assignExperimentButton != nullptr) {
        ui->assignExperimentButton->setEnabled(!cell.cellID().empty());
    }

    float currentTempExt = cell.currentTempExt();
    float currentTempBlock = cell.currentTempInner();
    int currentRPM = cell.currentRPM();
    float targetTemp = cell.assignedTemp();
    int targetRPM = cell.assignedRPM();

    qDebug() << ">>>> cell updated  targetTemp: " << targetTemp << ", currentTempExt: " << currentTempExt;
    setTargetRPMView(targetRPM);
    setTargetTempView(targetTemp);
    setCurrentExtTempView(currentTempExt);
    setCurrentBlockTempView(currentTempBlock);
    setCurrentRPMView(currentRPM);

    Experiment experiment = cell.asignedExperiment();
    if (hasExperimentAssigned(experiment)) {
        ensureExperimentGraph(experiment);
        loadVisualHistoryIfNeeded(experiment);
    } else {
        clearExperimentGraph();
    }

    QString expName = QString::fromStdString(experiment.name());
    if (expName.isEmpty()) {
        expName = "--";
    }
    ui->experimentNameValueLabel->setText(expName);

    QString assignedBy = QString::fromStdString(experiment.owner().username());
    if (assignedBy.isEmpty()) {
        assignedBy = "--";
    }
    ui->assignedByValueLabel->setText(assignedBy);

    QString assignedAt = "--";
    if (experiment.startSystemTimeMSecs() > 0) {
        QDateTime assignedDate = QDateTime::fromMSecsSinceEpoch(experiment.startSystemTimeMSecs());
        assignedAt = assignedDate.toString("yyyy-MM-dd HH:mm");
    }
    ui->assignedAtValueLabel->setText(assignedAt);

    QString stateText = QString::fromStdString(experiment.state());
    if (stateText.isEmpty()) {
        stateText = "--";
    }

    QString progressText = "--";
    if (hasExperimentAssigned(experiment)) {
        unsigned long long totalDurationMs = 0;
        const auto &tempArcs = experiment.profile().tempArcsInSeq();
        for (const auto &arc : tempArcs) {
            totalDurationMs += arc.durationMSec();
        }

        unsigned long long startMs = experiment.startSystemTimeMSecs();
        if (startMs > 0 && totalDurationMs > 0) {
            unsigned long long nowMs = Cell::getCurrentTimeMillis();
            unsigned long long elapsedMs = nowMs > startMs ? nowMs - startMs : 0;
            if (!tempArcs.empty()) {
                unsigned long long cumulativeMs = 0;
                int arcIndex = static_cast<int>(tempArcs.size());
                for (int i = 0; i < static_cast<int>(tempArcs.size()); ++i) {
                    cumulativeMs += tempArcs.at(i).durationMSec();
                    if (elapsedMs < cumulativeMs) {
                        arcIndex = i + 1;
                        break;
                    }
                }

                QString arcLabel = arcIndex > static_cast<int>(tempArcs.size())
                    ? QString("Arc %1/%2").arg(tempArcs.size()).arg(tempArcs.size())
                    : QString("Arc %1/%2").arg(arcIndex).arg(tempArcs.size());
                QString elapsedLabel = formatDurationMs(elapsedMs);
                QString totalLabel = formatDurationMs(totalDurationMs);
                progressText = QString("%1 (%2 / %3)").arg(arcLabel, elapsedLabel, totalLabel);
            }
        }
    }

    ui->expStateLabel->setText(stateText);
    ui->experimentProgressValueLabel->setText(progressText);

    updateTargetSyncStatus(cell);

    if (stateText == "RUNNING" && m_cellGraph != nullptr && !m_cellGraph->isDataPushStarted()) {
        m_cellGraph->initilizeExperimentGraph();
    }

}

void CellWidget::ensureExperimentGraph(const Experiment &experiment)
{
    bool experimentChanged = experiment.experimentId() != m_assignedExperiment.experimentId()
        || experiment.name() != m_assignedExperiment.name();

    if (m_cellGraph == nullptr || experimentChanged) {
        m_assignedExperiment = experiment;
        m_firstStartedRunning = false;
        if (m_cellGraph != nullptr) {
            ui->graphLayput->removeWidget(m_cellGraph);
            m_cellGraph->deleteLater();
            m_cellGraph = nullptr;
        }
        m_cellGraph = new CellGraph(m_assignedExperiment, this);
        ui->graphLayput->addWidget(m_cellGraph);
    }
}
void CellWidget::loadVisualHistoryIfNeeded(const Experiment &experiment)
{
    if (m_cellGraph == nullptr || m_cell.cellID().empty()) {
        return;
    }

    if (experiment.experimentId().empty()) {
        m_loadedHistoryExperimentId.clear();
        m_lastHistorySyncMs = 0;
        return;
    }

    if (experiment.state() != "RUNNING") {
        m_lastHistorySyncMs = 0;
        return;
    }

    if (m_loadedHistoryExperimentId != experiment.experimentId()) {
        m_loadedHistoryExperimentId = experiment.experimentId();
        m_lastHistorySyncMs = 0;
    }

    unsigned long long nowMs = Cell::getCurrentTimeMillis();
    if (m_lastHistorySyncMs != 0 && nowMs - m_lastHistorySyncMs < 500) {
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }

    CellVisualsHistory history = RedisDBManager::getInstance()->getCellVisualsHistory(m_cell.cellID());
    m_cellGraph->loadVisualHistory(history);
    m_lastHistorySyncMs = nowMs;
}

void CellWidget::clearExperimentGraph()
{
    if (m_cellGraph != nullptr) {
        ui->graphLayput->removeWidget(m_cellGraph);
        m_cellGraph->deleteLater();
        m_cellGraph = nullptr;
    }
    m_firstStartedRunning = false;
    m_assignedExperiment = Experiment();
    m_loadedHistoryExperimentId.clear();
    m_lastHistorySyncMs = 0;
}

bool CellWidget::hasExperimentAssigned(const Experiment &experiment) const
{
    return !(experiment.experimentId().empty() && experiment.name().empty());
}

void CellWidget::updateTargetSyncStatus(const Cell &cell)
{
    if (ui->assignmentStatusLabel == nullptr) {
        return;
    }

    Experiment experiment = cell.asignedExperiment();
    if (!hasExperimentAssigned(experiment) || cell.cellID().empty()) {
        ui->assignmentStatusLabel->clear();
        ui->assignmentStatusLabel->setVisible(false);
        return;
    }

    QString dotColor;

    switch (cell.targetSyncState()) {
    case Cell::TargetSyncPending:
        dotColor = "#F1C40F";
        break;
    case Cell::TargetSyncFailed:
        dotColor = "#E74C3C";
        break;
    case Cell::TargetSyncSynced:
        dotColor = "#2ECC71";
        break;
    case Cell::TargetSyncUnknown:
    default:
        ui->assignmentStatusLabel->clear();
        ui->assignmentStatusLabel->setVisible(false);
        return;
    }

    ui->assignmentStatusLabel->setText(QString());
    ui->assignmentStatusLabel->setVisible(true);
    ui->assignmentStatusLabel->setFixedSize(12, 12);
    ui->assignmentStatusLabel->setStyleSheet(QString("QLabel { background: %1; border-radius: 6px; }")
                                              .arg(dotColor));
}
