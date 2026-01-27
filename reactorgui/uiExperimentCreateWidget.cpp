#include "uiExperimentCreateWidget.h"
#include "qmessagebox.h"
#include "ui_uiExperimentCreateWidget.h"
#include "RedisDBManager.h"

#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGuiApplication>
#include <QHeaderView>
#include <QComboBox>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>
#include <QRadioButton>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <QUuid>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <unordered_map>

#include "uiCellOverviewWidget.h"

ExperimentCreateWidget::ExperimentCreateWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentCreateWidget)
{
    ui->setupUi(this);

    ui->plateauTimeComboBox->addItem(tr("sec."), IDX_TIME_SEC);
    ui->plateauTimeComboBox->addItem(tr("min."), IDX_TIME_MIN);
    ui->plateauTimeComboBox->addItem(tr("hour"), IDX_TIME_HOUR);

    ui->finalHoldTimeCombobox->addItem(tr("sec."), IDX_TIME_SEC);
    ui->finalHoldTimeCombobox->addItem(tr("min."), IDX_TIME_MIN);
    ui->finalHoldTimeCombobox->addItem(tr("hour"), IDX_TIME_HOUR);

    ui->initHoldTimeComboBox->addItem(tr("sec."), IDX_TIME_SEC);
    ui->initHoldTimeComboBox->addItem(tr("min."), IDX_TIME_MIN);
    ui->initHoldTimeComboBox->addItem(tr("hour"), IDX_TIME_HOUR);

    ui->stepsDownDurationComboBox->addItem(tr("sec."), IDX_TIME_SEC);
    ui->stepsDownDurationComboBox->addItem(tr("min."), IDX_TIME_MIN);
    ui->stepsDownDurationComboBox->addItem(tr("hour"), IDX_TIME_HOUR);

    ui->stepsUpDurationComboBox->addItem(tr("sec."), IDX_TIME_SEC);
    ui->stepsUpDurationComboBox->addItem(tr("min."), IDX_TIME_MIN);
    ui->stepsUpDurationComboBox->addItem(tr("hour"), IDX_TIME_HOUR);


    ui->adv_arcDurationComboBox->addItem(tr("sec."), IDX_TIME_SEC);
    ui->adv_arcDurationComboBox->addItem(tr("min."), IDX_TIME_MIN);
    ui->adv_arcDurationComboBox->addItem(tr("hour"), IDX_TIME_HOUR);

    connect(ui->createPushButton, &QPushButton::clicked, this, &ExperimentCreateWidget::updateClicked);


    connect(ui->basicStandard, &QPushButton::clicked, this, &ExperimentCreateWidget::handleBasicsButtonClicked);
    connect(ui->basicLinUpStepDown, &QPushButton::clicked, this, &ExperimentCreateWidget::handleBasicsButtonClicked);
    connect(ui->basicStepUpLinDown, &QPushButton::clicked, this, &ExperimentCreateWidget::handleBasicsButtonClicked);
    connect(ui->basicStepUpStepDown, &QPushButton::clicked, this, &ExperimentCreateWidget::handleBasicsButtonClicked);
    connect(ui->advancedProfile, &QPushButton::clicked, this, &ExperimentCreateWidget::advancedProfileButtonClicked);

    connect(ui->adv_addPlatueRadioButton, &QRadioButton::clicked, this, &ExperimentCreateWidget::adv_addPlatueClicked);
    connect(ui->adv_addLinearRadioButton, &QRadioButton::clicked, this, &ExperimentCreateWidget::adv_addLinearClicked);
    connect(ui->adv_addStepsRadioButton, &QRadioButton::clicked, this, &ExperimentCreateWidget::adv_addStepsClicked);
    connect(ui->adv_add2ndDegRadioButton, &QRadioButton::clicked, this, &ExperimentCreateWidget::adv_add2ndDegClicked);

    connect(ui->adv_addArcToExpButton, &QPushButton::clicked, this, &ExperimentCreateWidget::adv_addArcToExperimentClicked);
    connect(ui->adv_delLastArcButton, &QPushButton::clicked, this, &ExperimentCreateWidget::adv_delLastArcClicked);
    connect(ui->adv_clearProfileButton, &QPushButton::clicked, this, &ExperimentCreateWidget::adv_clearProfileClicked);
    connect(ui->adv_createPushButton, &QPushButton::clicked, this, &ExperimentCreateWidget::updateClicked);

    connect(ui->adv_arcDurationLineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_addArcDurationChanged);
    connect(ui->adv_arcDurationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ExperimentCreateWidget::adv_addArcDurationChanged);
    connect(ui->adv_arcRampLineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_rampChanged);
    connect(ui->adv_initTempLineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_finalTempChanged);
    connect(ui->adv_polyALineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_polyAChanged);
    connect(ui->adv_finalTempLineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_finalTempChanged);

    ui->numOfStepsDownLineEdit->setValidator(new QIntValidator(0,9999,this));
    ui->numOfStepsUpLineEdit->setValidator(new QIntValidator(0,9999,this));
    ui->numOfCyclesLineEdit->setValidator(new QIntValidator(0,9999,this));
    ui->initRPMLineEdit->setValidator(new QIntValidator(0,999999,this));

    ui->initRampLineEdit->setValidator(new QDoubleValidator(0,99,2,this));
    ui->finalRampLineEdit->setValidator(new QDoubleValidator(0,99,2,this));

    ui->finalTempLineEdit->setValidator(new QDoubleValidator(-9999,9999,2,this));
    ui->initTempLineEdit->setValidator(new QDoubleValidator(-9999,9999,2,this));
    ui->targetTempLineEdit->setValidator(new QDoubleValidator(-9999,9999,2,this));

    ui->plateauTimeLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));
    ui->finalHoldTimeLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));
    ui->initHoldTimeLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));
    ui->stepsDownDurationLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));
    ui->stepsUpDurationLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));

    ui->adv_initTempLineEdit->setValidator(new QDoubleValidator(-9999,9999,2,this));
    ui->adv_finalTempLineEdit->setValidator(new QDoubleValidator(-9999,9999,2,this));
    ui->adv_arcRampLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));
    ui->adv_arcDurationLineEdit->setValidator(new QDoubleValidator(0,9999,2,this));

    ui->adv_numOfCyclesLineEdit->setValidator(new QIntValidator(0,9999,this));
    ui->adv_stepCntLieEdit->setValidator(new QIntValidator(0,9999,this));

    ui->adv_polyALineEdit->setValidator(new QDoubleValidator(-9999,9999,4,this));

    ui->numOfCyclesLineEdit->setText("1");
    ui->adv_numOfCyclesLineEdit->setText("1");


    m_expGraph = new ExperimentGraph(m_currentExperiment);
    ui->expGraphLayout->addWidget(m_expGraph);


    ui->stackedWidget->setCurrentIndex(PAGE_WARNING_IDX);
    m_lastPageIndex = PAGE_WARNING_IDX;

    // setVisibleAllBasicExperimentItems(false);
    QLabel *label = new QLabel(this);
    label->setText(tr("Please select an experiment type to create."));
    QFont font = label->font();
    font.setBold(true);
    font.setPointSize(font.pointSize()+2);
    label->setFont(font);
    ui->warningMessageBoxLayout->addWidget(label);


    m_advArcButtonGroup = new QButtonGroup(this);
    m_advArcButtonGroup->addButton(ui->adv_addPlatueRadioButton);
    m_advArcButtonGroup->addButton(ui->adv_addStepsRadioButton);
    m_advArcButtonGroup->addButton(ui->adv_addLinearRadioButton);
    m_advArcButtonGroup->addButton(ui->adv_add2ndDegRadioButton);

    m_assignButton = new QPushButton(tr("Assign"), this);
    ui->basicActionLayout->addWidget(m_assignButton);
    connect(m_assignButton, &QPushButton::clicked, this, &ExperimentCreateWidget::assignExperimentToCells);

    m_assignButtonAdvanced = new QPushButton(tr("Assign"), this);
    ui->advActionLayout->addWidget(m_assignButtonAdvanced);
    connect(m_assignButtonAdvanced, &QPushButton::clicked, this, &ExperimentCreateWidget::assignExperimentToCells);

    const QList<QLineEdit*> edits = findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        connect(edit, &QLineEdit::textChanged, this, [this](const QString &) {
            updatePreviewFromUi();
        });
    }

    const QList<QComboBox*> combos = findChildren<QComboBox*>();
    for (QComboBox *combo : combos) {
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this](int) {
                    updatePreviewFromUi();
                });
    }

}

ExperimentCreateWidget::~ExperimentCreateWidget()
{
    delete ui;
}

void ExperimentCreateWidget::setCurrentUser(const User &user)
{
    m_currentUser = user;
}

void ExperimentCreateWidget::loadExperiment(const Experiment &experiment, Mode mode)
{
    m_currentExperiment = experiment;
    m_mode = mode;
    m_currentExpType = static_cast<ExperimentType>(experiment.experimentType());

    applyExperimentType(m_currentExpType);
    applyUiState(experiment.settingsJson());
    m_expGraph->updateTheExperiment(m_currentExperiment);
    updatePreviewFromUi();
    updateModeUi();
}

void ExperimentCreateWidget::setVisibleAllBasicExperimentItems(bool en)
{
    ui->numOfStepsDownLineEdit->setVisible(en);
    ui->numOfStepsDownLabel->setVisible(en);
    ui->stepsDownDurationLineEdit->setVisible(en);
    ui->stepsDownDurationLabel->setVisible(en);
    ui->stepsDownDurationComboBox->setVisible(en);

    ui->numOfStepsUpLineEdit->setVisible(en);
    ui->numOfStepsUpLabel->setVisible(en);
    ui->stepsUpDurationLineEdit->setVisible(en);
    ui->stepsUpDurationLabel->setVisible(en);
    ui->stepsUpDurationComboBox->setVisible(en);

    ui->finalHoldTimeCombobox->setVisible(en);
    ui->finalHoldTimeLabel->setVisible(en);
    ui->finalHoldTimeLineEdit->setVisible(en);
    ui->initHoldTimeLineEdit->setVisible(en);
    ui->initHoldTimeComboBox->setVisible(en);
    ui->initHoldTimeLabel->setVisible(en);
    ui->initRPMLabel->setVisible(en);
    ui->initRPMRPMLabel->setVisible(en);
    ui->initRPMLineEdit->setVisible(en);
    ui->initTempLineEdit->setVisible(en);
    ui->initTempCLabel->setVisible(en);
    ui->initTempLabel->setVisible(en);
    ui->numOfCyclesLineEdit->setVisible(en);
    ui->numOfCyclesLabel->setVisible(en);
    ui->targetTempCLabel->setVisible(en);
    ui->targetTempLineEdit->setVisible(en);
    ui->targetTempLabel->setVisible(en);
    ui->plateauTimeLineEdit->setVisible(en);
    ui->plateauTimeLabel->setVisible(en);
    ui->plateauTimeComboBox->setVisible(en);
    ui->finalTempCLabel->setVisible(en);
    ui->finalTempLabel->setVisible(en);
    ui->finalTempLineEdit->setVisible(en);

}

void ExperimentCreateWidget::updateBasicGroupVisibility()
{
    bool showSteps = m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN
        || m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN
        || m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN;

    ui->basicStepsGroupBox->setVisible(showSteps);
}

TempArc ExperimentCreateWidget::createLinearTempArc(float startTemp, float endTemp, float rank, unsigned long startTimeMSec)
{
    double linearDurationSec = std::fabs((endTemp - startTemp) / rank);
    unsigned long linearDurationMSec = static_cast<unsigned long>(linearDurationSec * 1000.0);
    double startTimeSec = startTimeMSec / 1000.0;

    qDebug() << " ExperimentCreateWidget::createLinearTempArc:: endTemp: " << endTemp;
    if(endTemp < startTemp){
        rank = -rank;
    }

    return TempArc(0, rank, startTemp - (rank * startTimeSec), startTimeMSec, linearDurationMSec);

}

TempArc ExperimentCreateWidget::createPlateuTempArc(float startTemp, unsigned long startTimeMSec, unsigned long durationMSec)
{
    return TempArc(0, 0, startTemp, startTimeMSec, durationMSec);
}

std::vector<TempArc> ExperimentCreateWidget::createStepTempArc(float startTemp, float endTemp, int stepCount, unsigned long stepUpDownDuration, unsigned long startTimeMSec)
{
    qDebug() << "createStepTempArc::startTemp: " << startTemp;
    qDebug() << "createStepTempArc::endTemp: " << endTemp;
    qDebug() << "createStepTempArc::stepCount: " << stepCount;
    qDebug() << "createStepTempArc::stepUpDownDuration: " << stepUpDownDuration;
    qDebug() << "createStepTempArc::startTimeMSec: " << startTimeMSec;


    stepCount++;
    std::vector<TempArc> resultTempArcs;
    for(int i = 0; i < stepCount; i++){
        float stepTempDiff = (endTemp-startTemp) /stepCount;
        float stepStartTemp = startTemp + i * stepTempDiff;
        float stepEndTemp = stepStartTemp + stepTempDiff;

        unsigned long stepDuration = (stepUpDownDuration / stepCount) / 2;
        unsigned long stepStartTime = startTimeMSec + i * (stepDuration*2);


        qDebug() << "createStepTempArc::stepTempDiff: " << stepTempDiff;
        qDebug() << "createStepTempArc::stepStartTemp: " << stepStartTemp;
        qDebug() << "createStepTempArc::stepEndTemp: " << stepEndTemp;
        qDebug() << "createStepTempArc::stepDuration: " << stepDuration;
        qDebug() << "createStepTempArc::stepStartTime: " << stepStartTime;


        double stepDurationSec = stepDuration / 1000.0;
        float stepRank = std::fabs(stepEndTemp - stepStartTemp) / stepDurationSec;
        TempArc tempArcLin = createLinearTempArc(stepStartTemp, stepEndTemp, stepRank, stepStartTime);
        TempArc tempArcPlt = createPlateuTempArc(tempArcLin.finishTemp(), tempArcLin.finishTimeMsec(), stepDuration);

        resultTempArcs.push_back(tempArcLin);
        resultTempArcs.push_back(tempArcPlt);
    }

    return resultTempArcs;
}

TempArc ExperimentCreateWidget::create2ndDegArc(float startTemp, float endTemp, float a, unsigned long startTimeMSec, unsigned long durationMSec)
{
    qDebug() << "create2ndDegArc::a: " << a;

    double durationSec = durationMSec / 1000.0;
    double startTimeSec = startTimeMSec / 1000.0;

    float b = ((endTemp - startTemp) / durationSec) - (a * ((2 * startTimeSec) + durationSec));
    float c = startTemp - (a*startTimeSec*startTimeSec) - b*startTimeSec;
    qDebug() << "create2ndDegArc::b: " << b;
    qDebug() << "create2ndDegArc::c: " << c;

    return TempArc(a,b,c,startTimeMSec,durationMSec);
}


void ExperimentCreateWidget::handleBasicsButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    // Uncheck all buttons except the clicked one
    if (clickedButton != ui->basicStandard) {
        ui->basicStandard->setChecked(false);
    }
    if (clickedButton != ui->basicLinUpStepDown) {
        ui->basicLinUpStepDown->setChecked(false);
    }
    if (clickedButton != ui->basicStepUpLinDown) {
        ui->basicStepUpLinDown->setChecked(false);
    }
    if (clickedButton != ui->basicStepUpStepDown) {
        ui->basicStepUpStepDown->setChecked(false);
    }


    setVisibleAllBasicExperimentItems(true);

    ui->stackedWidget->setCurrentIndex(PAGE_WARNING_IDX);

    QLayoutItem *item;
    while ((item = ui->warningMessageBoxLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->hide();
        }
        delete item;
    }

    QMessageBox messageBox;
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No); // Set buttons
    messageBox.setText(tr("Are you sure you want to create a new experiment? The current one will be destroyed."));

    ui->warningMessageBoxLayout->addWidget(&messageBox);
    int ret = messageBox.exec();
    if (ret == QMessageBox::Yes) {


        // Clear all line edits
        ui->initTempLineEdit->clear();
        ui->numOfStepsUpLineEdit->clear();
        ui->stepsUpDurationLineEdit->clear();
        ui->initHoldTimeLineEdit->clear();
        ui->finalTempLineEdit->clear();
        ui->finalHoldTimeLineEdit->clear();
        ui->numOfStepsDownLineEdit->clear();
        ui->stepsDownDurationLineEdit->clear();
        ui->numOfCyclesLineEdit->clear();
        ui->targetTempLineEdit->clear();
        ui->plateauTimeLineEdit->clear();

        ui->numOfCyclesLineEdit->setText("1");

        m_currentExperiment.setProfile(Profile());
        m_expGraph->updateTheExperiment(m_currentExperiment);




        // Show/hide fields based on the clicked button
        if (clickedButton == ui->basicStepUpStepDown) {
            m_currentExpType = EXP_BASIC_ST_UP_ST_DOWN;

            ui->initRampCLabel->setVisible(false);
            ui->initRampLabel->setVisible(false);
            ui->initRampLineEdit->setVisible(false);


            ui->finalRampCLabel->setVisible(false);
            ui->finalRampLabel->setVisible(false);
            ui->finalRampLineEdit->setVisible(false);

        }

        else if (clickedButton == ui->basicStepUpLinDown) {
            m_currentExpType = EXP_BASIC_ST_UP_LI_DOWN;

            ui->initRampCLabel->setVisible(false);
            ui->initRampLabel->setVisible(false);
            ui->initRampLineEdit->setVisible(false);

            ui->numOfStepsDownLineEdit->setVisible(false);
            ui->numOfStepsDownLabel->setVisible(false);
            ui->stepsDownDurationLineEdit->setVisible(false);
            ui->stepsDownDurationLabel->setVisible(false);
            ui->stepsDownDurationComboBox->setVisible(false);
        }

        else if (clickedButton == ui->basicLinUpStepDown) {
            m_currentExpType = EXP_BASIC_LI_UP_ST_DOWN;

            ui->finalRampCLabel->setVisible(false);
            ui->finalRampLabel->setVisible(false);
            ui->finalRampLineEdit->setVisible(false);

            ui->numOfStepsUpLineEdit->setVisible(false);
            ui->numOfStepsUpLabel->setVisible(false);
            ui->stepsUpDurationLineEdit->setVisible(false);
            ui->stepsUpDurationLabel->setVisible(false);
            ui->stepsUpDurationComboBox->setVisible(false);
        }

        else if (clickedButton == ui->basicStandard) {
            m_currentExpType = EXP_BASIC_STANDARD;

            ui->numOfStepsDownLineEdit->setVisible(false);
            ui->numOfStepsDownLabel->setVisible(false);
            ui->stepsDownDurationLineEdit->setVisible(false);
            ui->stepsDownDurationLabel->setVisible(false);
            ui->stepsDownDurationComboBox->setVisible(false);

            ui->numOfStepsUpLineEdit->setVisible(false);
            ui->numOfStepsUpLabel->setVisible(false);
            ui->stepsUpDurationLineEdit->setVisible(false);
            ui->stepsUpDurationLabel->setVisible(false);
            ui->stepsUpDurationComboBox->setVisible(false);
        }

        updateBasicGroupVisibility();

        ui->stackedWidget->setCurrentIndex(PAGE_CREATE_IDX);
        m_lastPageIndex = PAGE_CREATE_IDX;
        updatePreviewFromUi();

    } else if (ret == QMessageBox::No) {
        // Handle 'No' button clicked
        qDebug() << "No button clicked";

        ui->stackedWidget->setCurrentIndex(m_lastPageIndex);
    }


}

void ExperimentCreateWidget::blinkLineEdit(QLineEdit* lineEdit, int blinkCount)
{
    if (!lineEdit) return;

    QTimer* timer = new QTimer(lineEdit);
    int* counter = new int(0);
    QString originalStyleSheet = lineEdit->styleSheet();

    QObject::connect(timer, &QTimer::timeout, [lineEdit, timer, counter, blinkCount, originalStyleSheet]() {
        if (*counter >= blinkCount * 2) {
            timer->stop();
            lineEdit->setStyleSheet(originalStyleSheet);
            timer->deleteLater();
            delete counter;
            return;
        }

        if (*counter % 2 == 0) {
            lineEdit->setStyleSheet(originalStyleSheet +
                                    "border: 2px solid red; border-radius: 3px;");
        } else {
            lineEdit->setStyleSheet(originalStyleSheet);
        }

        (*counter)++;
    });

    timer->start(200); // 200ms interval
}

bool ExperimentCreateWidget::validateBasicInputs(bool showErrors)
{
    auto requireNonEmpty = [this, showErrors](QLineEdit *edit) -> bool {
        if (!edit || !edit->isVisible()) {
            return true;
        }
        if (edit->text().trimmed().isEmpty()) {
            if (showErrors) {
                blinkLineEdit(edit);
            }
            return false;
        }
        return true;
    };

    auto requirePositive = [this, showErrors, &requireNonEmpty](QLineEdit *edit) -> bool {
        if (!requireNonEmpty(edit)) {
            return false;
        }
        bool ok = false;
        double value = edit->text().toDouble(&ok);
        if (!ok || value <= 0.0) {
            if (showErrors) {
                blinkLineEdit(edit);
            }
            return false;
        }
        return true;
    };

    if (!requireNonEmpty(ui->initTempLineEdit)
        || !requireNonEmpty(ui->targetTempLineEdit)
        || !requireNonEmpty(ui->finalTempLineEdit)
        || !requireNonEmpty(ui->initHoldTimeLineEdit)
        || !requireNonEmpty(ui->plateauTimeLineEdit)
        || !requireNonEmpty(ui->finalHoldTimeLineEdit)
        || !requirePositive(ui->numOfCyclesLineEdit)) {
        return false;
    }

    if (m_currentExpType == EXP_BASIC_STANDARD) {
        return requirePositive(ui->initRampLineEdit)
            && requirePositive(ui->finalRampLineEdit);
    }

    if (m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN) {
        return requirePositive(ui->initRampLineEdit)
            && requirePositive(ui->numOfStepsDownLineEdit)
            && requirePositive(ui->stepsDownDurationLineEdit);
    }

    if (m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN) {
        return requirePositive(ui->finalRampLineEdit)
            && requirePositive(ui->numOfStepsUpLineEdit)
            && requirePositive(ui->stepsUpDurationLineEdit);
    }

    if (m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN) {
        return requirePositive(ui->numOfStepsUpLineEdit)
            && requirePositive(ui->stepsUpDurationLineEdit)
            && requirePositive(ui->numOfStepsDownLineEdit)
            && requirePositive(ui->stepsDownDurationLineEdit);
    }

    return false;
}

bool ExperimentCreateWidget::buildProfile(Profile &profile)
{
    if (m_currentExpType == EXP_BASIC_STANDARD
        || m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN
        || m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN
        || m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN) {
        if (!validateBasicInputs(true)) {
            return false;
        }

        if (m_currentExpType == EXP_BASIC_STANDARD) {
            profile = createBasicStandardProfile();
        } else if (m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN) {
            profile = createLinearUpStepDownProfile();
        } else if (m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN) {
            profile = createStepUpLinearDownProfile();
        } else if (m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN) {
            profile = createStepUpStepDownProfile();
        }
    } else if (m_currentExpType == EXP_ADVANCED_LIN || m_currentExpType == EXP_ADVANCED_PLATUE ||
               m_currentExpType == EXP_ADVANCED_STEPS || m_currentExpType == EXP_ADVANCED_2NDDEG) {
        profile = m_currentExperiment.profile();
    }

    if (profile.tempArcsInSeq().empty()) {
        return false;
    }

    const auto tempArcs = profile.tempArcsInSeq();
    TempArc firstTempArc = tempArcs.at(0);
    TempArc lastTempARc = tempArcs.at(tempArcs.size() - 1);

    float rpmC = ui->initRPMLineEdit->text().toFloat();
    RPMArc rpmarc(0, 0, rpmC, firstTempArc.startTimeMsec(),
                  lastTempARc.finishTimeMsec() - firstTempArc.startTimeMsec());
    profile.addRPMArcInSequence(rpmarc);
    profile.setTotalMaxProfileDuration(lastTempARc.finishTimeMsec());

    return true;
}

void ExperimentCreateWidget::applyExperimentType(ExperimentType type)
{
    if (type == EXP_UNDEFINED) {
        return;
    }

    m_currentExpType = type;

    if (type == EXP_BASIC_STANDARD || type == EXP_BASIC_LI_UP_ST_DOWN ||
        type == EXP_BASIC_ST_UP_LI_DOWN || type == EXP_BASIC_ST_UP_ST_DOWN) {
        ui->basicStandard->setChecked(type == EXP_BASIC_STANDARD);
        ui->basicLinUpStepDown->setChecked(type == EXP_BASIC_LI_UP_ST_DOWN);
        ui->basicStepUpLinDown->setChecked(type == EXP_BASIC_ST_UP_LI_DOWN);
        ui->basicStepUpStepDown->setChecked(type == EXP_BASIC_ST_UP_ST_DOWN);
        ui->advancedProfile->setChecked(false);
        ui->stackedWidget->setCurrentIndex(PAGE_CREATE_IDX);
        setVisibleAllBasicExperimentItems(true);

        ui->initRampCLabel->setVisible(true);
        ui->initRampLabel->setVisible(true);
        ui->initRampLineEdit->setVisible(true);
        ui->finalRampCLabel->setVisible(true);
        ui->finalRampLabel->setVisible(true);
        ui->finalRampLineEdit->setVisible(true);
        ui->numOfStepsDownLineEdit->setVisible(true);
        ui->numOfStepsDownLabel->setVisible(true);
        ui->stepsDownDurationLineEdit->setVisible(true);
        ui->stepsDownDurationLabel->setVisible(true);
        ui->stepsDownDurationComboBox->setVisible(true);
        ui->numOfStepsUpLineEdit->setVisible(true);
        ui->numOfStepsUpLabel->setVisible(true);
        ui->stepsUpDurationLineEdit->setVisible(true);
        ui->stepsUpDurationLabel->setVisible(true);
        ui->stepsUpDurationComboBox->setVisible(true);

        if (type == EXP_BASIC_ST_UP_ST_DOWN) {
            ui->initRampCLabel->setVisible(false);
            ui->initRampLabel->setVisible(false);
            ui->initRampLineEdit->setVisible(false);
            ui->finalRampCLabel->setVisible(false);
            ui->finalRampLabel->setVisible(false);
            ui->finalRampLineEdit->setVisible(false);
        } else if (type == EXP_BASIC_ST_UP_LI_DOWN) {
            ui->initRampCLabel->setVisible(false);
            ui->initRampLabel->setVisible(false);
            ui->initRampLineEdit->setVisible(false);

            ui->numOfStepsDownLineEdit->setVisible(false);
            ui->numOfStepsDownLabel->setVisible(false);
            ui->stepsDownDurationLineEdit->setVisible(false);
            ui->stepsDownDurationLabel->setVisible(false);
            ui->stepsDownDurationComboBox->setVisible(false);
        } else if (type == EXP_BASIC_LI_UP_ST_DOWN) {
            ui->finalRampCLabel->setVisible(false);
            ui->finalRampLabel->setVisible(false);
            ui->finalRampLineEdit->setVisible(false);

            ui->numOfStepsUpLineEdit->setVisible(false);
            ui->numOfStepsUpLabel->setVisible(false);
            ui->stepsUpDurationLineEdit->setVisible(false);
            ui->stepsUpDurationLabel->setVisible(false);
            ui->stepsUpDurationComboBox->setVisible(false);
        } else if (type == EXP_BASIC_STANDARD) {
            ui->numOfStepsDownLineEdit->setVisible(false);
            ui->numOfStepsDownLabel->setVisible(false);
            ui->stepsDownDurationLineEdit->setVisible(false);
            ui->stepsDownDurationLabel->setVisible(false);
            ui->stepsDownDurationComboBox->setVisible(false);

            ui->numOfStepsUpLineEdit->setVisible(false);
            ui->numOfStepsUpLabel->setVisible(false);
            ui->stepsUpDurationLineEdit->setVisible(false);
            ui->stepsUpDurationLabel->setVisible(false);
            ui->stepsUpDurationComboBox->setVisible(false);
        }
        updateBasicGroupVisibility();
        return;
    }

    if (type == EXP_ADVANCED_LIN || type == EXP_ADVANCED_PLATUE ||
        type == EXP_ADVANCED_STEPS || type == EXP_ADVANCED_2NDDEG) {
        ui->basicStandard->setChecked(false);
        ui->basicLinUpStepDown->setChecked(false);
        ui->basicStepUpLinDown->setChecked(false);
        ui->basicStepUpStepDown->setChecked(false);
        ui->advancedProfile->setChecked(true);
        ui->stackedWidget->setCurrentIndex(PAGE_ADVANCED_IDX);
        setVisibleAllAdvExperimentItems(false, false);

        ui->adv_initTempCLabel->setVisible(true);
        ui->adv_initTempLabel->setVisible(true);
        ui->adv_initTempLineEdit->setVisible(true);
        ui->adv_arcDurationLineEdit->setVisible(true);
        ui->adv_arcDurationLabel->setVisible(true);
        ui->adv_arcDurationComboBox->setVisible(true);
        ui->adv_numOfCyclesLabel->setVisible(true);
        ui->adv_numOfCyclesLineEdit->setVisible(true);

        if (type == EXP_ADVANCED_PLATUE) {
            ui->adv_addPlatueRadioButton->setChecked(true);
        } else if (type == EXP_ADVANCED_LIN) {
            ui->adv_finalTempLabel->setVisible(true);
            ui->adv_finalTempLineEdit->setVisible(true);
            ui->adv_finalTempCLabel->setVisible(true);
            ui->adv_arcRampLabel->setVisible(true);
            ui->adv_arcRampLineEdit->setVisible(true);
            ui->adv_arcRampCLabel->setVisible(true);
            ui->adv_arcRampLabel->setText(tr("Arc Ramp:"));
            ui->adv_addLinearRadioButton->setChecked(true);
        } else if (type == EXP_ADVANCED_STEPS) {
            ui->adv_finalTempLabel->setVisible(true);
            ui->adv_finalTempLineEdit->setVisible(true);
            ui->adv_finalTempCLabel->setVisible(true);
            ui->adv_stepCntLabel->setVisible(true);
            ui->adv_stepCntLieEdit->setVisible(true);
            ui->adv_arcRampLabel->setVisible(true);
            ui->adv_arcRampLineEdit->setVisible(true);
            ui->adv_arcRampCLabel->setVisible(true);
            ui->adv_arcRampLabel->setText(tr("Arc Ramp:"));
            ui->adv_addStepsRadioButton->setChecked(true);
        } else if (type == EXP_ADVANCED_2NDDEG) {
            ui->adv_finalTempLabel->setVisible(true);
            ui->adv_finalTempLineEdit->setVisible(true);
            ui->adv_finalTempCLabel->setVisible(true);
            ui->adv_arcRampLabel->setVisible(true);
            ui->adv_arcRampLineEdit->setVisible(true);
            ui->adv_arcRampCLabel->setVisible(true);
            ui->adv_arcRampLineEdit->setReadOnly(true);
            ui->adv_arcRampLabel->setText(tr("Extremum Temp (calc):"));
            ui->adv_arcRampCLabel->setText(tr("°C"));
            ui->adv_polyALabel->setVisible(true);
            ui->adv_polyALineEdit->setVisible(true);
            ui->adv_polyDescrLabel->setVisible(true);
            ui->adv_add2ndDegRadioButton->setChecked(true);
            m_adv2ndDegDriver = Adv2ndDegDriver::Target;
        }
    }
}

void ExperimentCreateWidget::updateModeUi()
{
    if (m_mode == Mode::Create) {
        ui->createPushButton->setText(tr("Create"));
        ui->adv_createPushButton->setText(tr("Create"));
        ui->createPushButton->show();
        ui->adv_createPushButton->show();
        setInputsEnabled(true);
        return;
    }

    if (m_mode == Mode::Edit) {
        ui->createPushButton->setText(tr("Update"));
        ui->adv_createPushButton->setText(tr("Update"));
        ui->createPushButton->show();
        ui->adv_createPushButton->show();
        setInputsEnabled(true);
        return;
    }

    ui->createPushButton->hide();
    ui->adv_createPushButton->hide();
    setInputsEnabled(false);
}

std::string ExperimentCreateWidget::promptExperimentName(const QString &currentName)
{
    bool ok = false;
    QString text = QInputDialog::getText(this, tr("Experiment Name"),
                                         tr("Enter experiment name:"), QLineEdit::Normal,
                                         currentName, &ok);
    if (!ok) {
        return {};
    }
    return text.trimmed().toStdString();
}

std::string ExperimentCreateWidget::captureUiState() const
{
    QJsonObject root;

    const QList<QLineEdit*> edits = findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        root.insert(edit->objectName(), edit->text());
    }

    const QList<QComboBox*> combos = findChildren<QComboBox*>();
    for (QComboBox *combo : combos) {
        root.insert(combo->objectName(), combo->currentIndex());
    }

    const QList<QRadioButton*> radios = findChildren<QRadioButton*>();
    for (QRadioButton *radio : radios) {
        root.insert(radio->objectName(), radio->isChecked());
    }

    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact).toStdString();
}

void ExperimentCreateWidget::applyUiState(const std::string &stateJson)
{
    if (stateJson.empty()) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(stateJson));
    if (!doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    const QList<QLineEdit*> edits = findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        QJsonValue value = root.value(edit->objectName());
        if (value.isString()) {
            edit->setText(value.toString());
        }
    }

    const QList<QComboBox*> combos = findChildren<QComboBox*>();
    for (QComboBox *combo : combos) {
        QJsonValue value = root.value(combo->objectName());
        if (value.isDouble()) {
            combo->setCurrentIndex(value.toInt());
        }
    }

    const QList<QRadioButton*> radios = findChildren<QRadioButton*>();
    for (QRadioButton *radio : radios) {
        QJsonValue value = root.value(radio->objectName());
        if (value.isBool()) {
            radio->setChecked(value.toBool());
        }
    }
}

void ExperimentCreateWidget::setInputsEnabled(bool enabled)
{
    const QList<QLineEdit*> edits = findChildren<QLineEdit*>();
    for (QLineEdit *edit : edits) {
        edit->setEnabled(enabled);
    }
    const QList<QComboBox*> combos = findChildren<QComboBox*>();
    for (QComboBox *combo : combos) {
        combo->setEnabled(enabled);
    }
    ui->basicStandard->setEnabled(enabled);
    ui->basicLinUpStepDown->setEnabled(enabled);
    ui->basicStepUpLinDown->setEnabled(enabled);
    ui->basicStepUpStepDown->setEnabled(enabled);
    ui->advancedProfile->setEnabled(enabled);
    ui->adv_addPlatueRadioButton->setEnabled(enabled);
    ui->adv_addLinearRadioButton->setEnabled(enabled);
    ui->adv_addStepsRadioButton->setEnabled(enabled);
    ui->adv_add2ndDegRadioButton->setEnabled(enabled);
    ui->adv_addArcToExpButton->setEnabled(enabled);
    ui->adv_delLastArcButton->setEnabled(enabled);
    ui->adv_clearProfileButton->setEnabled(enabled);
}

void ExperimentCreateWidget::assignExperimentToCells()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }

    bool isAdmin = m_currentUser.role() == ROLE_ADMIN || m_currentUser.role() == ROLE_ROOT;

    std::vector<std::string> cellIds = RedisDBManager::getInstance()->getCellIds();
    std::vector<Cell> cells = RedisDBManager::getInstance()->getCellList(cellIds);
    std::vector<Cell> filteredCells;
    filteredCells.reserve(cells.size());
    std::unordered_set<std::string> seenIds;
    for (const auto &cell : cells) {
        if (!cell.isPlugged()) {
            continue;
        }
        const std::string &cellId = cell.cellID();
        if (cellId.empty()) {
            continue;
        }
        if (!seenIds.insert(cellId).second) {
            continue;
        }
        filteredCells.push_back(cell);
    }

    if (filteredCells.empty()) {
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Assign Experiment"));
    if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect available = screen->availableGeometry();
        dialog.resize(static_cast<int>(available.width() * 0.9),
                      static_cast<int>(available.height() * 0.85));
    } else {
        dialog.resize(900, 700);
    }
    dialog.setSizeGripEnabled(true);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QHBoxLayout *listLayout = new QHBoxLayout();

    QListWidget *lhsListWidget = new QListWidget(&dialog);
    lhsListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    lhsListWidget->setSpacing(8);

    QListWidget *rhsListWidget = new QListWidget(&dialog);
    rhsListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    rhsListWidget->setSpacing(8);

    std::vector<Cell> lhsCells;
    std::vector<Cell> rhsCells;
    std::unordered_map<std::string, Cell> cellById;
    for (const auto &cell : filteredCells) {
        cellById[cell.cellID()] = cell;
        QString upperId = QString::fromStdString(cell.cellID()).toUpper();
        if (upperId.contains("RHS")) {
            rhsCells.push_back(cell);
        } else {
            lhsCells.push_back(cell);
        }
    }

    auto sortByPositionAsc = [](const Cell &a, const Cell &b) {
        return a.positionIdx() < b.positionIdx();
    };
    auto sortByPositionDesc = [](const Cell &a, const Cell &b) {
        return a.positionIdx() > b.positionIdx();
    };
    std::sort(lhsCells.begin(), lhsCells.end(), sortByPositionAsc);
    std::sort(rhsCells.begin(), rhsCells.end(), sortByPositionDesc);

    auto addCellItem = [&](QListWidget *targetList, const Cell &cell, int displayIndex) {
        QListWidgetItem *item = new QListWidgetItem(targetList);
        item->setData(Qt::UserRole, QString::fromStdString(cell.cellID()));
        item->setData(Qt::UserRole + 1, displayIndex);

        CellOverviewWidget *cellWidget = new CellOverviewWidget(targetList);
        cellWidget->setSlotInfo(tr("Cell"), displayIndex);
        cellWidget->setCellData(cell);
        item->setSizeHint(cellWidget->sizeHint());
        targetList->setItemWidget(item, cellWidget);

        QString owner = QString::fromStdString(cell.asignedExperiment().owner().username());
        bool available = cell.asignedExperiment().name().empty() ||
                         owner == QString::fromStdString(m_currentUser.username()) ||
                         isAdmin;
        if (!available) {
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            cellWidget->setEnabled(false);
        }
    };

    for (const auto &cell : lhsCells) {
        int displayIndex = cell.positionIdx();
        addCellItem(lhsListWidget, cell, displayIndex);
    }
    for (const auto &cell : rhsCells) {
        int displayIndex = 5 + cell.positionIdx();
        addCellItem(rhsListWidget, cell, displayIndex);
    }

    listLayout->addWidget(lhsListWidget);
    listLayout->addWidget(rhsListWidget);
    layout->addLayout(listLayout);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QList<QListWidgetItem*> selectedItems = lhsListWidget->selectedItems();
    selectedItems.append(rhsListWidget->selectedItems());
    if (selectedItems.isEmpty()) {
        return;
    }

    int runningCount = 0;
    for (QListWidgetItem *item : selectedItems) {
        std::string cellId = item->data(Qt::UserRole).toString().toStdString();
        auto it = cellById.find(cellId);
        if (it == cellById.end()) {
            continue;
        }
        QString state = QString::fromStdString(it->second.asignedExperiment().state());
        if (state.compare("running", Qt::CaseInsensitive) == 0) {
            runningCount++;
        }
    }

    QString confirmMessage = tr("Assign this experiment to %1 cell(s)?\n"
                                "Existing assignments on those cells will be overwritten.")
                                 .arg(selectedItems.size());
    if (runningCount > 0) {
        confirmMessage.append(tr("\n\nWarning: %1 selected cell(s) are currently running an experiment.")
                                  .arg(runningCount));
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Assign Experiment"),
        confirmMessage,
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    std::vector<Cell> updatedCells;
    for (QListWidgetItem *item : selectedItems) {
        std::string cellId = item->data(Qt::UserRole).toString().toStdString();
        auto it = cellById.find(cellId);
        if (it == cellById.end()) {
            continue;
        }
        Cell cell = it->second;
        Experiment assignedExperiment = m_currentExperiment;
        assignedExperiment.setOwner(m_currentUser);
        assignedExperiment.setStartSystemTimeMSecs(0);
        cell.setAsignedExperiment(assignedExperiment);
        updatedCells.push_back(cell);

    }

    if (!updatedCells.empty()) {
        RedisDBManager::getInstance()->pushCellList(updatedCells);
    }
}


void ExperimentCreateWidget::updateClicked()
{
    if (m_mode == Mode::Show) {
        assignExperimentToCells();
        return;
    }

    Profile profile;
    if (!buildProfile(profile)) {
        return;
    }

    m_currentExperiment.setProfile(profile);
    m_currentExperiment.setTotalProfileDurationMSecs(profile.totalMaxProfileDuration());
    m_currentExperiment.setExperimentType(static_cast<int>(m_currentExpType));
    m_currentExperiment.setSettingsJson(captureUiState());

    std::string expName = m_currentExperiment.name();
    if (expName.empty()) {
        expName = promptExperimentName(QString::fromStdString(expName));
        if (expName.empty()) {
            return;
        }
        m_currentExperiment.setName(expName);
    }

    if (m_mode == Mode::Create) {
        if (m_currentExperiment.experimentId().empty()) {
            m_currentExperiment.setExperimentId(QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString());
        }
        m_currentExperiment.setCreatedAtMSecs(Cell::getCurrentTimeMillis());
        m_currentExperiment.setOwner(m_currentUser);
        m_currentExperiment.setStartSystemTimeMSecs(0);
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }
    RedisDBManager::getInstance()->pushExperiment(m_currentExperiment);

    m_expGraph->updateTheExperiment(m_currentExperiment);
    emit sgn_experimentSaved();
}





///ADVANCED EXPERIMENTS----------


void ExperimentCreateWidget::setVisibleAllAdvExperimentItems(bool en, bool clearFields)
{
    ui->adv_arcDurationComboBox->setVisible(en);
    ui->adv_arcDurationLabel->setVisible(en);
    ui->adv_arcDurationLineEdit->setVisible(en);
    ui->adv_arcRampLabel->setVisible(en);
    ui->adv_arcRampCLabel->setVisible(en);
    ui->adv_arcRampLineEdit->setVisible(en);
    ui->adv_arcRampLineEdit->setReadOnly(false);
    ui->adv_arcRampLabel->setText(tr("Arc Ramp:"));
    ui->adv_arcRampCLabel->setText(tr("°C/min"));
    ui->adv_finalTempLineEdit->setVisible(en);
    ui->adv_finalTempLabel->setVisible(en);
    ui->adv_finalTempCLabel->setVisible(en);
    ui->adv_initTempCLabel->setVisible(en);
    ui->adv_initTempLabel->setVisible(en);
    ui->adv_initTempLineEdit->setVisible(en);
    ui->adv_polyALineEdit->setVisible(en);
    ui->adv_polyALabel->setVisible(en);
    ui->adv_polyDescrLabel->setVisible(en);
    ui->adv_stepCntLabel->setVisible(en);
    ui->adv_stepCntLieEdit->setVisible(en);

    if (clearFields) {
        ui->adv_initTempLineEdit->clear();
        ui->adv_finalTempLineEdit->clear();
        ui->adv_arcRampLineEdit->clear();
        ui->adv_arcDurationLineEdit->clear();
        ui->adv_numOfCyclesLineEdit->clear();
        ui->adv_stepCntLieEdit->clear();
        ui->adv_polyALineEdit->clear();

        ui->adv_initTempLineEdit->setEnabled(true);
    }

}

void ExperimentCreateWidget::advancedProfileButtonClicked()
{
    ui->basicStandard->setChecked(false);
    ui->basicLinUpStepDown->setChecked(false);
    ui->basicStepUpLinDown->setChecked(false);
    ui->basicStepUpStepDown->setChecked(false);

    //  hideAllAdvExperimentItems(false
    ui->stackedWidget->setCurrentIndex(PAGE_WARNING_IDX);

    QLayoutItem *item;
    while ((item = ui->warningMessageBoxLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->hide();
        }
        delete item;
    }

    QMessageBox messageBox;
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No); // Set buttons
    messageBox.setText(tr("Are you sure you want to create a custom new experiment? The current one will be destroyed."));

    ui->warningMessageBoxLayout->addWidget(&messageBox);
    int ret = messageBox.exec();
    if (ret == QMessageBox::Yes) {

        setVisibleAllAdvExperimentItems(false);

        m_currentExperiment.setProfile(Profile());
        m_expGraph->updateTheExperiment(m_currentExperiment);


        ui->stackedWidget->setCurrentIndex(PAGE_ADVANCED_IDX);
        m_lastPageIndex = PAGE_ADVANCED_IDX;

    } else if (ret == QMessageBox::No) {
        // Handle 'No' button clicked
        qDebug() << "No button clicked";
        ui->stackedWidget->setCurrentIndex(m_lastPageIndex);
    }



}


void ExperimentCreateWidget::adv_forceInitialTemp()
{
    Profile profile = m_currentExperiment.profile();
    if(profile.tempArcsInSeq().size() > 0){
        float initialTemp = profile.tempArcsInSeq().at(profile.tempArcsInSeq().size()-1).finishTemp();
        ui->adv_initTempLineEdit->setText(QString::number(initialTemp));
        ui->adv_initTempLineEdit->setEnabled(false);

    }
}

void ExperimentCreateWidget::adv_update2ndDegBindings(Adv2ndDegDriver driver)
{
    if (m_currentExpType != EXP_ADVANCED_2NDDEG) {
        return;
    }
    (void)driver;

    Profile profile = m_currentExperiment.profile();
    const auto tempArcs = profile.tempArcsInSeq();

    double initialTemp = 0.0;
    if (!tempArcs.empty()) {
        initialTemp = tempArcs.back().finishTemp();
    } else {
        QString initialText = ui->adv_initTempLineEdit->text().trimmed();
        if (initialText.isEmpty()) {
            return;
        }
        initialTemp = initialText.toDouble();
    }

    QString durationText = ui->adv_arcDurationLineEdit->text().trimmed();
    if (durationText.isEmpty()) {
        return;
    }

    double durationValue = durationText.toDouble();
    if (durationValue <= 0.0) {
        return;
    }

    int arcDurationIdx = ui->adv_arcDurationComboBox->currentData().toInt();
    double arcDurationTimeMult = 1.0;
    if (arcDurationIdx == IDX_TIME_MIN) {
        arcDurationTimeMult = 60.0;
    } else if (arcDurationIdx == IDX_TIME_HOUR) {
        arcDurationTimeMult = 3600.0;
    }
    double durationSec = durationValue * arcDurationTimeMult;
    if (durationSec <= 0.0) {
        return;
    }

    QString aText = ui->adv_polyALineEdit->text().trimmed();
    QString targetText = ui->adv_finalTempLineEdit->text().trimmed();
    if (aText.isEmpty() || targetText.isEmpty()) {
        ui->adv_arcRampLineEdit->clear();
        ui->adv_arcRampLabel->setText(tr("Extremum Temp (calc):"));
        ui->adv_arcRampCLabel->setText(tr("°C"));
        return;
    }

    double a = aText.toDouble();
    double target = targetText.toDouble();
    double bLocal = (target - initialTemp - (a * durationSec * durationSec)) / durationSec;
    double extremumTemp = initialTemp;

    if (std::fabs(a) < 1e-12) {
        extremumTemp = (a < 0.0)
            ? std::max(initialTemp, target)
            : std::min(initialTemp, target);
    } else {
        double tau = -bLocal / (2.0 * a);
        if (tau >= 0.0 && tau <= durationSec) {
            extremumTemp = (a * tau * tau) + (bLocal * tau) + initialTemp;
        } else {
            extremumTemp = (a < 0.0)
                ? std::max(initialTemp, target)
                : std::min(initialTemp, target);
        }
    }

    if (a < 0.0) {
        ui->adv_arcRampLabel->setText(tr("Max Temp (calc):"));
    } else {
        ui->adv_arcRampLabel->setText(tr("Min Temp (calc):"));
    }
    ui->adv_arcRampCLabel->setText(tr("°C"));
    ui->adv_arcRampLineEdit->setText(QString::number(extremumTemp));
}

void ExperimentCreateWidget::clearAdvancedArcSelection()
{
    if (!m_advArcButtonGroup) {
        return;
    }

    m_advArcButtonGroup->setExclusive(false);
    const auto buttons = m_advArcButtonGroup->buttons();
    for (auto *button : buttons) {
        button->setChecked(false);
    }
    m_advArcButtonGroup->setExclusive(true);
}

void ExperimentCreateWidget::updatePreviewFromUi()
{
    if (!m_expGraph) {
        return;
    }

    if (m_currentExpType == EXP_ADVANCED_2NDDEG) {
        adv_update2ndDegBindings(Adv2ndDegDriver::None);
    }

    Profile previewProfile;
    bool hasPreview = false;

    if (m_currentExpType == EXP_BASIC_STANDARD
        || m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN
        || m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN
        || m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN) {
        if (!validateBasicInputs(false)) {
            m_expGraph->updatePreviewProfile(Profile());
            return;
        }

        if (m_currentExpType == EXP_BASIC_STANDARD) {
            previewProfile = createBasicStandardProfile();
        } else if (m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN) {
            previewProfile = createLinearUpStepDownProfile();
        } else if (m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN) {
            previewProfile = createStepUpLinearDownProfile();
        } else if (m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN) {
            previewProfile = createStepUpStepDownProfile();
        }
        hasPreview = !previewProfile.tempArcsInSeq().empty();
    } else if (m_currentExpType == EXP_ADVANCED_PLATUE
               || m_currentExpType == EXP_ADVANCED_LIN
               || m_currentExpType == EXP_ADVANCED_STEPS
               || m_currentExpType == EXP_ADVANCED_2NDDEG) {
        Profile currentProfile = m_currentExperiment.profile();
        double initialTemp = 0.0;
        unsigned long initTimeMsecs = 0;
        const auto tempArcs = currentProfile.tempArcsInSeq();
        if (!tempArcs.empty()) {
            initialTemp = tempArcs.back().finishTemp();
            initTimeMsecs = tempArcs.back().finishTimeMsec();
        } else {
            QString initialText = ui->adv_initTempLineEdit->text().trimmed();
            if (initialText.isEmpty()) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            initialTemp = initialText.toDouble();
        }

        QString durationText = ui->adv_arcDurationLineEdit->text().trimmed();
        if (durationText.isEmpty()) {
            m_expGraph->updatePreviewProfile(Profile());
            return;
        }

        double durationValue = durationText.toDouble();
        if (durationValue <= 0.0) {
            m_expGraph->updatePreviewProfile(Profile());
            return;
        }

        int arcDurationIdx = ui->adv_arcDurationComboBox->currentData().toInt();
        double arcDurationTimeMult = 1.0;
        if (arcDurationIdx == IDX_TIME_MIN) {
            arcDurationTimeMult = 60.0;
        } else if (arcDurationIdx == IDX_TIME_HOUR) {
            arcDurationTimeMult = 3600.0;
        }
        unsigned long arcDurationMSecs = static_cast<unsigned long>(durationValue * arcDurationTimeMult * 1000.0);

        if (m_currentExpType == EXP_ADVANCED_PLATUE) {
            TempArc tempArc = createPlateuTempArc(static_cast<float>(initialTemp), initTimeMsecs, arcDurationMSecs);
            previewProfile.addTempArcInSequence(tempArc);
            hasPreview = true;
        } else if (m_currentExpType == EXP_ADVANCED_LIN) {
            QString finalText = ui->adv_finalTempLineEdit->text().trimmed();
            if (finalText.isEmpty()) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            double finalTemp = finalText.toDouble();
            if (std::fabs(finalTemp - initialTemp) < 1e-6) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            double durationSec = arcDurationMSecs / 1000.0;
            if (durationSec <= 0.0) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            double rampCPerSec = std::fabs(finalTemp - initialTemp) / durationSec;
            TempArc tempArc = createLinearTempArc(static_cast<float>(initialTemp),
                                                  static_cast<float>(finalTemp),
                                                  static_cast<float>(rampCPerSec),
                                                  initTimeMsecs);
            previewProfile.addTempArcInSequence(tempArc);
            hasPreview = true;
        } else if (m_currentExpType == EXP_ADVANCED_STEPS) {
            QString finalText = ui->adv_finalTempLineEdit->text().trimmed();
            QString stepsText = ui->adv_stepCntLieEdit->text().trimmed();
            if (finalText.isEmpty() || stepsText.isEmpty()) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            int stepCnt = stepsText.toInt();
            if (stepCnt < 1) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            double finalTemp = finalText.toDouble();
            if (std::fabs(finalTemp - initialTemp) < 1e-6) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            std::vector<TempArc> stepArcs = createStepTempArc(static_cast<float>(initialTemp),
                                                              static_cast<float>(finalTemp),
                                                              stepCnt,
                                                              arcDurationMSecs,
                                                              initTimeMsecs);
            for (const auto &arc : stepArcs) {
                previewProfile.addTempArcInSequence(arc);
            }
            hasPreview = !stepArcs.empty();
        } else if (m_currentExpType == EXP_ADVANCED_2NDDEG) {
            QString finalText = ui->adv_finalTempLineEdit->text().trimmed();
            QString aText = ui->adv_polyALineEdit->text().trimmed();
            if (finalText.isEmpty() || aText.isEmpty()) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            double finalTemp = finalText.toDouble();
            double a = aText.toDouble();
            double durationSec = arcDurationMSecs / 1000.0;
            if (durationSec <= 0.0) {
                m_expGraph->updatePreviewProfile(Profile());
                return;
            }
            double rampCPerSec = (finalTemp - initialTemp - (a * durationSec * durationSec)) / durationSec;
            double startTimeSec = initTimeMsecs / 1000.0;
            double bLocal = rampCPerSec;
            double b = bLocal - (2.0 * a * startTimeSec);
            double c = (a * startTimeSec * startTimeSec) - (bLocal * startTimeSec) + initialTemp;
            TempArc tempArc(static_cast<float>(a),
                            static_cast<float>(b),
                            static_cast<float>(c),
                            initTimeMsecs,
                            arcDurationMSecs);
            previewProfile.addTempArcInSequence(tempArc);
            hasPreview = true;
        }
    }

    if (hasPreview) {
        m_expGraph->updatePreviewProfile(previewProfile);
    } else {
        m_expGraph->updatePreviewProfile(Profile());
    }
}

void ExperimentCreateWidget::adv_addPlatueClicked()
{

    setVisibleAllAdvExperimentItems(false);
    m_adv2ndDegDriver = Adv2ndDegDriver::None;

    ui->adv_initTempCLabel->setVisible(true);
    ui->adv_initTempLabel->setVisible(true);
    ui->adv_initTempLineEdit->setVisible(true);

    ui->adv_arcDurationLineEdit->setVisible(true);
    ui->adv_arcDurationLabel->setVisible(true);
    ui->adv_arcDurationComboBox->setVisible(true);

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_PLATUE;
    updatePreviewFromUi();


}


void ExperimentCreateWidget::adv_addLinearClicked()
{
    setVisibleAllAdvExperimentItems(false);
    m_adv2ndDegDriver = Adv2ndDegDriver::None;

    ui->adv_initTempCLabel->setVisible(true);
    ui->adv_initTempLabel->setVisible(true);
    ui->adv_initTempLineEdit->setVisible(true);

    ui->adv_finalTempCLabel->setVisible(true);
    ui->adv_finalTempLabel->setVisible(true);
    ui->adv_finalTempLineEdit->setVisible(true);

    ui->adv_arcDurationLineEdit->setVisible(true);
    ui->adv_arcDurationLabel->setVisible(true);
    ui->adv_arcDurationComboBox->setVisible(true);

    ui->adv_arcRampCLabel->setVisible(true);
    ui->adv_arcRampLabel->setVisible(true);
    ui->adv_arcRampLineEdit->setVisible(true);
    ui->adv_arcRampLabel->setText(tr("Arc Ramp:"));

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_LIN;
    updatePreviewFromUi();
}

void ExperimentCreateWidget::adv_addStepsClicked()
{
    setVisibleAllAdvExperimentItems(false);
    m_adv2ndDegDriver = Adv2ndDegDriver::None;

    ui->adv_initTempCLabel->setVisible(true);
    ui->adv_initTempLabel->setVisible(true);
    ui->adv_initTempLineEdit->setVisible(true);

    ui->adv_finalTempCLabel->setVisible(true);
    ui->adv_finalTempLabel->setVisible(true);
    ui->adv_finalTempLineEdit->setVisible(true);

    ui->adv_arcDurationLineEdit->setVisible(true);
    ui->adv_arcDurationLabel->setVisible(true);
    ui->adv_arcDurationComboBox->setVisible(true);

    ui->adv_stepCntLabel->setVisible(true);
    ui->adv_stepCntLieEdit->setVisible(true);

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_STEPS;
    updatePreviewFromUi();


}

void ExperimentCreateWidget::adv_add2ndDegClicked()
{
    setVisibleAllAdvExperimentItems(false);
    m_adv2ndDegDriver = Adv2ndDegDriver::Target;

    ui->adv_initTempCLabel->setVisible(true);
    ui->adv_initTempLabel->setVisible(true);
    ui->adv_initTempLineEdit->setVisible(true);

    ui->adv_finalTempCLabel->setVisible(true);
    ui->adv_finalTempLabel->setVisible(true);
    ui->adv_finalTempLineEdit->setVisible(true);

    ui->adv_arcDurationLineEdit->setVisible(true);
    ui->adv_arcDurationLabel->setVisible(true);
    ui->adv_arcDurationComboBox->setVisible(true);

    ui->adv_arcRampCLabel->setVisible(true);
    ui->adv_arcRampLabel->setVisible(true);
    ui->adv_arcRampLineEdit->setVisible(true);
    ui->adv_arcRampLineEdit->setReadOnly(true);
    ui->adv_arcRampLabel->setText(tr("Extremum Temp (calc):"));
    ui->adv_arcRampCLabel->setText(tr("°C"));

    ui->adv_polyALineEdit->setVisible(true);
    ui->adv_polyALabel->setVisible(true);
    ui->adv_polyDescrLabel->setVisible(true);

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_2NDDEG;
    adv_update2ndDegBindings(Adv2ndDegDriver::None);
    updatePreviewFromUi();
}


void ExperimentCreateWidget::adv_addArcToExperimentClicked()
{
    if (m_currentExpType == EXP_UNDEFINED) {
        return;
    }

    auto requireNonEmpty = [this](QLineEdit *lineEdit) -> bool {
        if (!lineEdit) {
            return false;
        }
        if (lineEdit->text().trimmed().isEmpty()) {
            blinkLineEdit(lineEdit);
            return false;
        }
        return true;
    };

    auto requirePositive = [this, &requireNonEmpty](QLineEdit *lineEdit) -> bool {
        if (!requireNonEmpty(lineEdit)) {
            return false;
        }
        if (lineEdit->text().toDouble() <= 0) {
            blinkLineEdit(lineEdit);
            return false;
        }
        return true;
    };

    Profile profile = m_currentExperiment.profile();

    float initialTemp = ui->adv_initTempLineEdit->text().toFloat();
    unsigned long initTimeMsecs = 0;

    const auto tempArcs = profile.tempArcsInSeq();
    if (!tempArcs.empty()) {
        initialTemp = tempArcs.back().finishTemp();
        initTimeMsecs = tempArcs.back().finishTimeMsec();
    } else if (!requireNonEmpty(ui->adv_initTempLineEdit)) {
        return;
    }

    const QString arcDurationText = ui->adv_arcDurationLineEdit->text().trimmed();
    double arcDurationValue = arcDurationText.toDouble();
    int arcDurationIdx = ui->adv_arcDurationComboBox->currentData().toInt();
    double arcDurationTimeMult = 1.0;
    if (arcDurationIdx == IDX_TIME_MIN) {
        arcDurationTimeMult = 60.0;
    } else if (arcDurationIdx == IDX_TIME_HOUR) {
        arcDurationTimeMult = 3600.0;
    }
    unsigned long arcDurationMSecs = 0;



    qDebug() << "adv_addArcToExperimentClicked::initTimeMsecs " << initTimeMsecs;






    if(m_currentExpType == EXP_ADVANCED_PLATUE){
        if (!requirePositive(ui->adv_arcDurationLineEdit)) {
            return;
        }
        arcDurationMSecs = static_cast<unsigned long>(arcDurationValue * arcDurationTimeMult * 1000.0);
        TempArc tempArc(0, 0, initialTemp, initTimeMsecs, arcDurationMSecs);
        profile.addTempArcInSequence(tempArc);
    }


    else if(m_currentExpType == EXP_ADVANCED_LIN){

        if (!requireNonEmpty(ui->adv_finalTempLineEdit)) {
            return;
        }
        float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();
        if (std::fabs(finalTemp - initialTemp) < 1e-6) {
            blinkLineEdit(ui->adv_finalTempLineEdit);
            return;
        }

        const QString rampText = ui->adv_arcRampLineEdit->text().trimmed();
        const bool hasRamp = !rampText.isEmpty();
        double rampCPerMin = rampText.toDouble();
        if (!hasRamp) {
            if (!requirePositive(ui->adv_arcDurationLineEdit)) {
                return;
            }
            double durationMin = (arcDurationValue * arcDurationTimeMult) / 60.0;
            rampCPerMin = std::fabs(finalTemp - initialTemp) / durationMin;
            if (rampCPerMin <= 0) {
                blinkLineEdit(ui->adv_arcRampLineEdit);
                return;
            }
            ui->adv_arcRampLineEdit->setText(QString::number(rampCPerMin));
        } else if (rampCPerMin <= 0) {
            blinkLineEdit(ui->adv_arcRampLineEdit);
            return;
        }

        float ramp = std::fabs(static_cast<float>(rampCPerMin) / 60.0);
        profile.addTempArcInSequence(createLinearTempArc(initialTemp, finalTemp, ramp, initTimeMsecs));

    }


    else if(m_currentExpType == EXP_ADVANCED_STEPS){

        if (!requireNonEmpty(ui->adv_finalTempLineEdit)) {
            return;
        }
        if (!requirePositive(ui->adv_arcDurationLineEdit)) {
            return;
        }
        if (!requirePositive(ui->adv_stepCntLieEdit)) {
            return;
        }
        float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();
        if (std::fabs(finalTemp - initialTemp) < 1e-6) {
            blinkLineEdit(ui->adv_finalTempLineEdit);
            return;
        }
        arcDurationMSecs = static_cast<unsigned long>(arcDurationValue * arcDurationTimeMult * 1000.0);
        int stepCnt = ui->adv_stepCntLieEdit->text().toInt();
        std::vector<TempArc> stepArcs = createStepTempArc(initialTemp, finalTemp, stepCnt, arcDurationMSecs, initTimeMsecs);
        foreach (TempArc arc, stepArcs) {
            profile.addTempArcInSequence(arc);
        }

    }


    else if(m_currentExpType == EXP_ADVANCED_2NDDEG){
        if (!requirePositive(ui->adv_arcDurationLineEdit)) {
            return;
        }

        arcDurationMSecs = static_cast<unsigned long>(arcDurationValue * arcDurationTimeMult * 1000.0);
        double durationSec = arcDurationMSecs / 1000.0;
        if (durationSec <= 0.0) {
            blinkLineEdit(ui->adv_arcDurationLineEdit);
            return;
        }

        QString aText = ui->adv_polyALineEdit->text().trimmed();
        QString targetText = ui->adv_finalTempLineEdit->text().trimmed();
        if (aText.isEmpty()) {
            blinkLineEdit(ui->adv_polyALineEdit);
            return;
        }
        if (targetText.isEmpty()) {
            blinkLineEdit(ui->adv_finalTempLineEdit);
            return;
        }

        double a = aText.toDouble();
        double finalTemp = targetText.toDouble();
        double rampCPerSec = (finalTemp - initialTemp - (a * durationSec * durationSec)) / durationSec;

        double startTimeSec = initTimeMsecs / 1000.0;
        double bLocal = rampCPerSec;
        double b = bLocal - (2.0 * a * startTimeSec);
        double c = (a * startTimeSec * startTimeSec) - (bLocal * startTimeSec) + initialTemp;
        TempArc deg2Arc(static_cast<float>(a),
                        static_cast<float>(b),
                        static_cast<float>(c),
                        initTimeMsecs,
                        arcDurationMSecs);
        profile.addTempArcInSequence(deg2Arc);
    }



    m_currentExpType = EXP_UNDEFINED;
    setVisibleAllAdvExperimentItems(false);
    clearAdvancedArcSelection();
    m_currentExperiment.setProfile(profile);
    m_expGraph->updateTheExperiment(m_currentExperiment);
    updatePreviewFromUi();

}

void ExperimentCreateWidget::adv_delLastArcClicked()
{
    Profile profile = m_currentExperiment.profile();
    if (profile.removeLastTempArc()) {
        m_currentExperiment.setProfile(profile);
        m_expGraph->updateTheExperiment(m_currentExperiment);
        if (profile.tempArcsInSeq().empty()) {
            ui->adv_initTempLineEdit->setEnabled(true);
        }
    }
    updatePreviewFromUi();
}

void ExperimentCreateWidget::adv_clearProfileClicked()
{
    Profile profile = m_currentExperiment.profile();
    profile.clearProfile();
    m_currentExperiment.setProfile(profile);
    m_expGraph->updateTheExperiment(m_currentExperiment);
    if (ui->adv_addPlatueRadioButton->isChecked()) {
        adv_addPlatueClicked();
    } else if (ui->adv_addLinearRadioButton->isChecked()) {
        adv_addLinearClicked();
    } else if (ui->adv_addStepsRadioButton->isChecked()) {
        adv_addStepsClicked();
    } else if (ui->adv_add2ndDegRadioButton->isChecked()) {
        adv_add2ndDegClicked();
    } else {
        m_currentExpType = EXP_UNDEFINED;
        setVisibleAllAdvExperimentItems(false);
    }
    updatePreviewFromUi();
}





void ExperimentCreateWidget::adv_addArcDurationChanged()
{
    float initialTemp = ui->adv_initTempLineEdit->text().toFloat();
    float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();

    if(m_currentExpType == EXP_ADVANCED_LIN){
        const QString durationText = ui->adv_arcDurationLineEdit->text().trimmed();
        if (durationText.isEmpty()) {
            return;
        }

        double durationValue = durationText.toDouble();
        if (durationValue <= 0) {
            return;
        }

        int arcDurationIdx = ui->adv_arcDurationComboBox->currentData().toInt();
        double arcDurationTimeMult = 1.0;
        if(arcDurationIdx == IDX_TIME_MIN){
            arcDurationTimeMult = 60.0;
        }else if(arcDurationIdx == IDX_TIME_HOUR){
            arcDurationTimeMult = 3600.0;
        }
        double arcDurationMin = (durationValue * arcDurationTimeMult) / 60.0;
        if (arcDurationMin <= 0) {
            return;
        }

        double rank = std::fabs(finalTemp-initialTemp)/arcDurationMin;
        ui->adv_arcRampLineEdit->setText(QString::number(rank));
    } else if (m_currentExpType == EXP_ADVANCED_2NDDEG) {
        adv_update2ndDegBindings(m_adv2ndDegDriver);
    }
}

void ExperimentCreateWidget::adv_rampChanged()
{

    float initialTemp = ui->adv_initTempLineEdit->text().toFloat();
    float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();

    if(m_currentExpType == EXP_ADVANCED_LIN){
        ui->adv_arcDurationComboBox->setCurrentIndex(IDX_TIME_MIN);

        float rank = ui->adv_arcRampLineEdit->text().toFloat();   //   C/min.
        if (rank <= 0) {
            return;
        }

        float durationMin = std::fabs(finalTemp-initialTemp) / rank;
        ui->adv_arcDurationLineEdit->setText(QString::number(durationMin));
    } else if (m_currentExpType == EXP_ADVANCED_2NDDEG) {
        adv_update2ndDegBindings(m_adv2ndDegDriver);
    }
}

void ExperimentCreateWidget::adv_polyAChanged()
{
    if (m_currentExpType != EXP_ADVANCED_2NDDEG) {
        return;
    }

    m_adv2ndDegDriver = Adv2ndDegDriver::A;
    adv_update2ndDegBindings(m_adv2ndDegDriver);
}

void ExperimentCreateWidget::adv_finalTempChanged()
{
    if (m_currentExpType != EXP_ADVANCED_2NDDEG) {
        return;
    }

    m_adv2ndDegDriver = Adv2ndDegDriver::Target;
    adv_update2ndDegBindings(m_adv2ndDegDriver);
}



///BASIC EXPERIMENT PROFILES
Profile ExperimentCreateWidget::createBasicStandardProfile()
{
    Profile profile;
    int initialHoldingTimeIdx = ui->initHoldTimeComboBox->currentData().toInt();
    int initialHoldTimeMult = 1;
    if(initialHoldingTimeIdx == IDX_TIME_MIN){
        initialHoldTimeMult = 60;
    }else if(initialHoldingTimeIdx == IDX_TIME_HOUR){
        initialHoldTimeMult = 3600;
    }

    int platueTimeIdx = ui->plateauTimeComboBox->currentData().toInt();
    int platueTimeMult = 1;
    if(platueTimeIdx == IDX_TIME_MIN){
        platueTimeMult = 60;
    }else if(platueTimeIdx == IDX_TIME_HOUR){
        platueTimeMult = 3600;
    }

    int finalHoldingTimeIdx = ui->finalHoldTimeCombobox->currentData().toInt();
    int finalHoldTimeMult = 1;
    if(finalHoldingTimeIdx == IDX_TIME_MIN){
        finalHoldTimeMult = 60;
    }else if(finalHoldingTimeIdx == IDX_TIME_HOUR){
        finalHoldTimeMult = 3600;
    }

    float targetTemp = ui->targetTempLineEdit->text().toFloat();
    float stepUpRank = ui->initRampLineEdit->text().toFloat() / 60.0;  //converting to C/sec
    float stepDownRank = ui->finalRampLineEdit->text().toFloat() / 60.0;  //converting to C/sec


    int numOfCycles = ui->numOfCyclesLineEdit->text().toInt();

    float initialTemp = ui->initTempLineEdit->text().toFloat();
    float finalTemp = ui->finalTempLineEdit->text().toFloat();

    unsigned long initialHoldingTimeMSecs = initialHoldTimeMult * ui->initHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long platueTimeMSecs = platueTimeMult * ui->plateauTimeLineEdit->text().toFloat() * 1000;
    unsigned long finalHoldingTimeMSecs = finalHoldTimeMult * ui->finalHoldTimeLineEdit->text().toFloat() * 1000;


    unsigned long startTimeMSec = 0;
    for(int i = 0; i < numOfCycles; i++){

        qDebug() << "startTimeMSec " << startTimeMSec;

        //ARC 1
        TempArc tempArc1(createPlateuTempArc(initialTemp, startTimeMSec, initialHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc1);



        float lastTemp = tempArc1.finishTemp();
        unsigned long arcStartTime = tempArc1.finishTimeMsec();

        qDebug() << "arcStartTime after  tempArc1 " << arcStartTime;
        qDebug() << "lastTemp after  tempArc1 " << lastTemp;


        //ARC 2
        TempArc tempArc2(createLinearTempArc(lastTemp, targetTemp, stepUpRank, arcStartTime));
        profile.addTempArcInSequence(tempArc2);




        lastTemp = tempArc2.finishTemp();
        arcStartTime = tempArc2.finishTimeMsec();
        //ARC 3
        TempArc tempArc3(createPlateuTempArc(lastTemp, arcStartTime, platueTimeMSecs));
        profile.addTempArcInSequence(tempArc3);




        lastTemp = tempArc3.finishTemp();
        arcStartTime = tempArc3.finishTimeMsec();
        //ARC 4
        TempArc tempArc4(createLinearTempArc(lastTemp, finalTemp, stepDownRank, arcStartTime));
        profile.addTempArcInSequence(tempArc4);



        lastTemp = tempArc4.finishTemp();
        arcStartTime = tempArc4.finishTimeMsec();
        //ARC 5
        TempArc tempArc5( createPlateuTempArc(lastTemp, arcStartTime, finalHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc5);

        startTimeMSec = tempArc5.finishTimeMsec();


    }
    return profile;
}


Profile ExperimentCreateWidget::createLinearUpStepDownProfile()
{
    Profile profile;
    int initialHoldingTimeIdx = ui->initHoldTimeComboBox->currentData().toInt();
    int initialHoldTimeMult = 1;
    if(initialHoldingTimeIdx == IDX_TIME_MIN){
        initialHoldTimeMult = 60;
    }else if(initialHoldingTimeIdx == IDX_TIME_HOUR){
        initialHoldTimeMult = 3600;
    }

    int platueTimeIdx = ui->plateauTimeComboBox->currentData().toInt();
    int platueTimeMult = 1;
    if(platueTimeIdx == IDX_TIME_MIN){
        platueTimeMult = 60;
    }else if(platueTimeIdx == IDX_TIME_HOUR){
        platueTimeMult = 3600;
    }

    int finalHoldingTimeIdx = ui->finalHoldTimeCombobox->currentData().toInt();
    int finalHoldTimeMult = 1;
    if(finalHoldingTimeIdx == IDX_TIME_MIN){
        finalHoldTimeMult = 60;
    }else if(finalHoldingTimeIdx == IDX_TIME_HOUR){
        finalHoldTimeMult = 3600;
    }

    int stepDownTimeIdx = ui->stepsDownDurationComboBox->currentData().toInt();
    int stepDownTimeMult = 1;
    if(stepDownTimeIdx == IDX_TIME_MIN){
        stepDownTimeMult = 60;
    }else if(stepDownTimeIdx == IDX_TIME_HOUR){
        stepDownTimeMult = 3600;
    }

    float targetTemp = ui->targetTempLineEdit->text().toFloat();
    float stepUpRank = ui->initRampLineEdit->text().toFloat() / 60.0;  //converting to C/sec

    int numOfCycles = ui->numOfCyclesLineEdit->text().toInt();
    int stepDownCount = ui->numOfStepsDownLineEdit->text().toInt();

    float initialTemp = ui->initTempLineEdit->text().toFloat();
    float finalTemp = ui->finalTempLineEdit->text().toFloat();

    unsigned long initialHoldingTimeMSecs = initialHoldTimeMult * ui->initHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long platueTimeMSecs = platueTimeMult * ui->plateauTimeLineEdit->text().toFloat() * 1000;
    unsigned long finalHoldingTimeMSecs = finalHoldTimeMult * ui->finalHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long stepDownTimeMSecs = stepDownTimeMult * ui->stepsDownDurationLineEdit->text().toFloat() * 1000;


    unsigned long startTimeMSec = 0;
    for(int i = 0; i < numOfCycles; i++){

        qDebug() << "startTimeMSec " << startTimeMSec;

        //ARC 1
        TempArc tempArc1(createPlateuTempArc(initialTemp, startTimeMSec, initialHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc1);



        float lastTemp = tempArc1.finishTemp();
        unsigned long arcStartTime = tempArc1.finishTimeMsec();

        qDebug() << "arcStartTime after  tempArc1 " << arcStartTime;
        qDebug() << "lastTemp after  tempArc1 " << lastTemp;


        //ARC 2
        TempArc tempArc2(createLinearTempArc(lastTemp, targetTemp, stepUpRank, arcStartTime));
        profile.addTempArcInSequence(tempArc2);




        lastTemp = tempArc2.finishTemp();
        arcStartTime = tempArc2.finishTimeMsec();
        //ARC 3
        TempArc tempArc3(createPlateuTempArc(lastTemp, arcStartTime, platueTimeMSecs));
        profile.addTempArcInSequence(tempArc3);




        lastTemp = tempArc3.finishTemp();
        arcStartTime = tempArc3.finishTimeMsec();

        std::vector<TempArc> stepDownArcs = createStepTempArc(lastTemp, finalTemp, stepDownCount, stepDownTimeMSecs,arcStartTime);
        for(int a = 0; a < stepDownArcs.size(); a++){
            profile.addTempArcInSequence(stepDownArcs.at(a));
            lastTemp = stepDownArcs.at(a).finishTemp();
            arcStartTime = stepDownArcs.at(a).finishTimeMsec();
        }




        //ARC 5
        TempArc tempArc5( createPlateuTempArc(lastTemp, arcStartTime, finalHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc5);

        startTimeMSec = tempArc5.finishTimeMsec();


    }
    return profile;
}


Profile ExperimentCreateWidget::createStepUpLinearDownProfile()
{
    Profile profile;
    int initialHoldingTimeIdx = ui->initHoldTimeComboBox->currentData().toInt();
    int initialHoldTimeMult = 1;
    if(initialHoldingTimeIdx == IDX_TIME_MIN){
        initialHoldTimeMult = 60;
    }else if(initialHoldingTimeIdx == IDX_TIME_HOUR){
        initialHoldTimeMult = 3600;
    }

    int platueTimeIdx = ui->plateauTimeComboBox->currentData().toInt();
    int platueTimeMult = 1;
    if(platueTimeIdx == IDX_TIME_MIN){
        platueTimeMult = 60;
    }else if(platueTimeIdx == IDX_TIME_HOUR){
        platueTimeMult = 3600;
    }

    int finalHoldingTimeIdx = ui->finalHoldTimeCombobox->currentData().toInt();
    int finalHoldTimeMult = 1;
    if(finalHoldingTimeIdx == IDX_TIME_MIN){
        finalHoldTimeMult = 60;
    }else if(finalHoldingTimeIdx == IDX_TIME_HOUR){
        finalHoldTimeMult = 3600;
    }

    int stepUpTimeIdx = ui->stepsUpDurationComboBox->currentData().toInt();
    int stepUpTimeMult = 1;
    if(stepUpTimeIdx == IDX_TIME_MIN){
        stepUpTimeMult = 60;
    }else if(stepUpTimeIdx == IDX_TIME_HOUR){
        stepUpTimeMult = 3600;
    }


    float targetTemp = ui->targetTempLineEdit->text().toFloat();
    float stepDownRank = ui->finalRampLineEdit->text().toFloat() / 60.0;  //converting to C/sec


    int numOfCycles = ui->numOfCyclesLineEdit->text().toInt();
    int stepUpCount = ui->numOfStepsUpLineEdit->text().toInt();

    float initialTemp = ui->initTempLineEdit->text().toFloat();
    float finalTemp = ui->finalTempLineEdit->text().toFloat();

    unsigned long initialHoldingTimeMSecs = initialHoldTimeMult * ui->initHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long platueTimeMSecs = platueTimeMult * ui->plateauTimeLineEdit->text().toFloat() * 1000;
    unsigned long finalHoldingTimeMSecs = finalHoldTimeMult * ui->finalHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long stepUpTimeMSecs = stepUpTimeMult * ui->stepsUpDurationLineEdit->text().toFloat() * 1000;


    unsigned long startTimeMSec = 0;
    for(int i = 0; i < numOfCycles; i++){

        qDebug() << "startTimeMSec " << startTimeMSec;

        //ARC 1
        TempArc tempArc1(createPlateuTempArc(initialTemp, startTimeMSec, initialHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc1);



        float lastTemp = tempArc1.finishTemp();
        unsigned long arcStartTime = tempArc1.finishTimeMsec();

        qDebug() << "arcStartTime after  tempArc1 " << arcStartTime;
        qDebug() << "lastTemp after  tempArc1 " << lastTemp;


        std::vector<TempArc> stepUpArcs = createStepTempArc(lastTemp, targetTemp, stepUpCount, stepUpTimeMSecs,arcStartTime);
        for(int a = 0; a < stepUpArcs.size(); a++){
            profile.addTempArcInSequence(stepUpArcs.at(a));
            lastTemp = stepUpArcs.at(a).finishTemp();
            arcStartTime = stepUpArcs.at(a).finishTimeMsec();
        }


        //ARC 3
        TempArc tempArc3(createPlateuTempArc(lastTemp, arcStartTime, platueTimeMSecs));
        profile.addTempArcInSequence(tempArc3);




        lastTemp = tempArc3.finishTemp();
        arcStartTime = tempArc3.finishTimeMsec();
        //ARC 4
        TempArc tempArc4(createLinearTempArc(lastTemp, finalTemp, stepDownRank, arcStartTime));
        profile.addTempArcInSequence(tempArc4);



        lastTemp = tempArc4.finishTemp();
        arcStartTime = tempArc4.finishTimeMsec();
        //ARC 5
        TempArc tempArc5( createPlateuTempArc(lastTemp, arcStartTime, finalHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc5);

        startTimeMSec = tempArc5.finishTimeMsec();


    }
    return profile;
}


Profile ExperimentCreateWidget::createStepUpStepDownProfile()
{
    Profile profile;
    int initialHoldingTimeIdx = ui->initHoldTimeComboBox->currentData().toInt();
    int initialHoldTimeMult = 1;
    if(initialHoldingTimeIdx == IDX_TIME_MIN){
        initialHoldTimeMult = 60;
    }else if(initialHoldingTimeIdx == IDX_TIME_HOUR){
        initialHoldTimeMult = 3600;
    }

    int platueTimeIdx = ui->plateauTimeComboBox->currentData().toInt();
    int platueTimeMult = 1;
    if(platueTimeIdx == IDX_TIME_MIN){
        platueTimeMult = 60;
    }else if(platueTimeIdx == IDX_TIME_HOUR){
        platueTimeMult = 3600;
    }

    int finalHoldingTimeIdx = ui->finalHoldTimeCombobox->currentData().toInt();
    int finalHoldTimeMult = 1;
    if(finalHoldingTimeIdx == IDX_TIME_MIN){
        finalHoldTimeMult = 60;
    }else if(finalHoldingTimeIdx == IDX_TIME_HOUR){
        finalHoldTimeMult = 3600;
    }

    int stepUpTimeIdx = ui->stepsUpDurationComboBox->currentData().toInt();
    int stepUpTimeMult = 1;
    if(stepUpTimeIdx == IDX_TIME_MIN){
        stepUpTimeMult = 60;
    }else if(stepUpTimeIdx == IDX_TIME_HOUR){
        stepUpTimeMult = 3600;
    }

    int stepDownTimeIdx = ui->stepsDownDurationComboBox->currentData().toInt();
    int stepDownTimeMult = 1;
    if(stepDownTimeIdx == IDX_TIME_MIN){
        stepDownTimeMult = 60;
    }else if(stepDownTimeIdx == IDX_TIME_HOUR){
        stepDownTimeMult = 3600;
    }

    float targetTemp = ui->targetTempLineEdit->text().toFloat();

    int numOfCycles = ui->numOfCyclesLineEdit->text().toInt();
    int stepUpCount = ui->numOfStepsUpLineEdit->text().toInt();
    int stepDownCount = ui->numOfStepsDownLineEdit->text().toInt();

    float initialTemp = ui->initTempLineEdit->text().toFloat();
    float finalTemp = ui->finalTempLineEdit->text().toFloat();

    unsigned long initialHoldingTimeMSecs = initialHoldTimeMult * ui->initHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long platueTimeMSecs = platueTimeMult * ui->plateauTimeLineEdit->text().toFloat() * 1000;
    unsigned long finalHoldingTimeMSecs = finalHoldTimeMult * ui->finalHoldTimeLineEdit->text().toFloat() * 1000;
    unsigned long stepUpTimeMSecs = stepUpTimeMult * ui->stepsUpDurationLineEdit->text().toFloat() * 1000;
    unsigned long stepDownTimeMSecs = stepDownTimeMult * ui->stepsDownDurationLineEdit->text().toFloat() * 1000;


    unsigned long startTimeMSec = 0;
    for(int i = 0; i < numOfCycles; i++){

        qDebug() << "startTimeMSec " << startTimeMSec;

        //ARC 1
        TempArc tempArc1(createPlateuTempArc(initialTemp, startTimeMSec, initialHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc1);



        float lastTemp = tempArc1.finishTemp();
        unsigned long arcStartTime = tempArc1.finishTimeMsec();



        std::vector<TempArc> stepUpArcs = createStepTempArc(lastTemp, targetTemp, stepUpCount, stepUpTimeMSecs,arcStartTime);
        for(int a = 0; a < stepUpArcs.size(); a++){
            profile.addTempArcInSequence(stepUpArcs.at(a));
            lastTemp = stepUpArcs.at(a).finishTemp();
            arcStartTime = stepUpArcs.at(a).finishTimeMsec();
        }


        //ARC 3
        TempArc tempArc3(createPlateuTempArc(lastTemp, arcStartTime, platueTimeMSecs));
        profile.addTempArcInSequence(tempArc3);




        lastTemp = tempArc3.finishTemp();
        arcStartTime = tempArc3.finishTimeMsec();

        std::vector<TempArc> stepDownArcs = createStepTempArc(lastTemp, finalTemp, stepDownCount, stepDownTimeMSecs,arcStartTime);
        for(int a = 0; a < stepDownArcs.size(); a++){
            profile.addTempArcInSequence(stepDownArcs.at(a));
            lastTemp = stepDownArcs.at(a).finishTemp();
            arcStartTime = stepDownArcs.at(a).finishTimeMsec();
        }




        //ARC 5
        TempArc tempArc5( createPlateuTempArc(lastTemp, arcStartTime, finalHoldingTimeMSecs));
        profile.addTempArcInSequence(tempArc5);

        startTimeMSec = tempArc5.finishTimeMsec();


    }
    return profile;
}
