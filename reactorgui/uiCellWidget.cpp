#include "uiCellWidget.h"
#include "ui_uiCellWidget.h"
#include "RedisDBManager.h"
#include "ExperimentRunnerPool.h"
#include <QDateTime>
#include <QtConcurrent>

namespace {
constexpr unsigned long long kPreheatWindowMs = 2ULL * 60ULL * 1000ULL;

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

}

CellWidget::~CellWidget()
{
    delete ui;
}

void CellWidget::setCurrentExtTempView(float temp)
{
    ui->currentExtTempLabel->setText(QString::number(temp));
}


void CellWidget::setCurrentInnerTempView(float temp)
{
    ui->currentInnerTempLabel->setText(QString::number(temp));
}



void CellWidget::setCurrentRPMView(int rpm)
{
    ui->currentRPMLabel->setText(QString::number(rpm));
}




void CellWidget::setTargetTempView(float temp)
{
    ui->targetTempLabel->setText(QString::number(temp));
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

    if (m_expRunner == nullptr) {
        return;
    }
    m_expRunner->assignExperiment(m_assignedExperiment);
    QtConcurrent::run([this]() {
        m_expRunner->run();  // Sınıf üyesi fonksiyonunu çağırıyoruz
    });
}

void CellWidget::setPositionIndex(int positionIndex)
{
    m_positionIndex = positionIndex;
    ExperimentRunner *runner = ExperimentRunnerPool::instance().runnerForPosition(positionIndex);
    if (runner == m_expRunner) {
        return;
    }
    if (m_expRunner != nullptr) {
        disconnect(m_expRunner, &ExperimentRunner::sgn_updateExperimentState, this, &CellWidget::updateExpState);
    }
    m_expRunner = runner;
    if (m_expRunner != nullptr) {
        connect(m_expRunner, &ExperimentRunner::sgn_updateExperimentState, this, &CellWidget::updateExpState);
    }
}

void CellWidget::setCellId(const std::string &cellId)
{
    if (m_expRunner == nullptr) {
        return;
    }
    m_expRunner->setCellId(cellId);
}



void CellWidget::updateExpState(ExperimentRunState state)
{
    qDebug() << ">>>> updateExpState: ";

    QString expState = "";
    if(state == STATE_INITILIZED){
        expState = "INITILIZED";
    }else if(state == STATE_PREHEAT){
        expState = "PREHEAT";
    }else if(state == STATE_RUNNING){
        if(!m_firstStartedRunning){
            m_firstStartedRunning = true;
            if (m_cellGraph != nullptr) {
                m_cellGraph->initilizeExperimentGraph();
            }
        }
        expState = "RUNNING";
    }else if(state == STATE_COMPLETED){
        expState = "COMPLETED";
    }
    qDebug() << ">>>> updateExpState expState: " << expState;

    ui->expStateLabel->setText(expState);

}

void CellWidget::updateCell(Cell &cell)
{
    m_cell = cell;
    if (m_expRunner != nullptr) {
        m_expRunner->setCellId(cell.cellID());
    }

    float currentTempExt = cell.currentTempExt();
    float currentTempInner = cell.currentTempInner();
    int currentRPM = cell.currentRPM();
    float targetTemp = cell.assignedTemp();
    int targetRPM = cell.assignedRPM();

    qDebug() << ">>>> cell updated  targetTemp: " << targetTemp << ", currentTempExt: " << currentTempExt;
    setTargetRPMView(targetRPM);
    setTargetTempView(targetTemp);
    setCurrentExtTempView(currentTempExt);
    setCurrentInnerTempView(currentTempInner);
    setCurrentRPMView(currentRPM);

    Experiment experiment = cell.asignedExperiment();
    if (hasExperimentAssigned(experiment)) {
        ensureExperimentGraph(experiment);
        loadVisualHistoryIfNeeded(experiment);
    } else {
        clearExperimentGraph();
    }

    pushTempAndRPMToCellGraph(currentTempExt, currentRPM);
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

    QString stateText = "--";
    QString progressText = "--";
    if (!cell.isPlugged() || cell.cellID().empty()) {
        stateText = "UNPLUGGED";
    } else if (experiment.experimentId().empty() && experiment.name().empty()) {
        stateText = "IDLE";
    } else {
        unsigned long long totalDurationMs = 0;
        const auto &tempArcs = experiment.profile().tempArcsInSeq();
        for (const auto &arc : tempArcs) {
            totalDurationMs += arc.durationMSec();
        }

        unsigned long long startMs = experiment.startSystemTimeMSecs();
        if (startMs == 0 || totalDurationMs == 0) {
            stateText = "PREHEAT";
        } else {
            unsigned long long nowMs = Cell::getCurrentTimeMillis();
            unsigned long long elapsedMs = nowMs > startMs ? nowMs - startMs : 0;
            if (elapsedMs < kPreheatWindowMs) {
                stateText = "PREHEAT";
            } else if (elapsedMs < totalDurationMs) {
                stateText = "RUNNING";
            } else {
                stateText = "COMPLETED";
            }

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
        return;
    }

    if (m_loadedHistoryExperimentId == experiment.experimentId()) {
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    CellVisualsHistory history = RedisDBManager::getInstance()->getCellVisualsHistory(m_cell.cellID());
    m_cellGraph->loadVisualHistory(history);
    m_loadedHistoryExperimentId = experiment.experimentId();
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
}

bool CellWidget::hasExperimentAssigned(const Experiment &experiment) const
{
    return !(experiment.experimentId().empty() && experiment.name().empty());
}
