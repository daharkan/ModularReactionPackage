#include "uiExperimentCreateWidget.h"
#include "qmessagebox.h"
#include "ui_uiExperimentCreateWidget.h"

#include <QButtonGroup>
#include <QTimer>

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

    connect(ui->adv_arcDurationLineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_addArcDurationChanged);
    connect(ui->adv_arcRampLineEdit, &QLineEdit::textEdited, this, &ExperimentCreateWidget::adv_rampChanged);

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


    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->adv_addPlatueRadioButton);
    buttonGroup->addButton(ui->adv_addStepsRadioButton);
    buttonGroup->addButton(ui->adv_addLinearRadioButton);
    buttonGroup->addButton(ui->adv_add2ndDegRadioButton);

}

ExperimentCreateWidget::~ExperimentCreateWidget()
{
    delete ui;
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

TempArc ExperimentCreateWidget::createLinearTempArc(float startTemp, float endTemp, float rank, unsigned long startTimeMSec)
{
    unsigned long linearDurationMSec = abs((endTemp - startTemp) / rank);
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


        float stepRank = abs(stepEndTemp - stepStartTemp) / stepDuration;
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

    float b = ((endTemp-startTemp)/durationSec) - (a * (startTimeSec+durationSec+durationSec));
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




        ui->stackedWidget->setCurrentIndex(PAGE_CREATE_IDX);
        m_lastPageIndex = PAGE_CREATE_IDX;

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

void ExperimentCreateWidget::updateClicked()
{

    int numOfCycles = ui->numOfCyclesLineEdit->text().toInt();
    if(numOfCycles < 1){

        blinkLineEdit(ui->numOfCyclesLineEdit);
        return;
    }



    Profile profile;

    if(m_currentExpType == EXP_BASIC_STANDARD){
        int rank = ui->initRampLineEdit->text().toInt();
         if(rank < 1){
            blinkLineEdit(ui->initRampLineEdit);
            return;
        }
        rank = ui->finalRampLineEdit->text().toInt();
        if(rank < 1){
         blinkLineEdit(ui->finalRampLineEdit);
         return;
        }

        profile = createBasicStandardProfile();
    }

    else if(m_currentExpType == EXP_BASIC_LI_UP_ST_DOWN){
        int rank = ui->initRampLineEdit->text().toInt();
        if(rank < 1){
            blinkLineEdit(ui->initRampLineEdit);
            return;
        }


        int stCount = ui->numOfStepsDownLineEdit->text().toInt();
        if(stCount < 1){
            blinkLineEdit(ui->numOfStepsDownLineEdit);
            return;
        }

        float stDur = ui->stepsDownDurationLineEdit->text().toFloat();
        if(stDur == 0){
            blinkLineEdit(ui->stepsDownDurationLineEdit);
            return;
        }

        profile = createLinearUpStepDownProfile();
    }


    else if(m_currentExpType == EXP_BASIC_ST_UP_LI_DOWN){
        int rank = ui->finalRampLineEdit->text().toInt();
        if(rank < 1){
            blinkLineEdit(ui->finalRampLineEdit);
            return;
        }

        int stCount = ui->numOfStepsUpLineEdit->text().toInt();
        if(stCount < 1){
            blinkLineEdit(ui->numOfStepsUpLineEdit);
            return;
        }

        float stDur = ui->stepsUpDurationLineEdit->text().toFloat();
        if(stDur == 0){
            blinkLineEdit(ui->stepsUpDurationLineEdit);
            return;
        }




        profile = createStepUpLinearDownProfile();
    }

    else if(m_currentExpType == EXP_BASIC_ST_UP_ST_DOWN){
        int stCount = ui->numOfStepsDownLineEdit->text().toInt();
        if(stCount < 1){
            blinkLineEdit(ui->numOfStepsDownLineEdit);
            return;
        }

        float stDur = ui->stepsDownDurationLineEdit->text().toFloat();
        if(stDur == 0){
            blinkLineEdit(ui->stepsDownDurationLineEdit);
            return;
        }

        stCount = ui->numOfStepsUpLineEdit->text().toInt();
        if(stCount < 1){
            blinkLineEdit(ui->numOfStepsUpLineEdit);
            return;
        }

        stDur = ui->stepsUpDurationLineEdit->text().toFloat();
        if(stDur == 0){
            blinkLineEdit(ui->stepsUpDurationLineEdit);
            return;
        }



        profile = createStepUpStepDownProfile();
    }
    TempArc firstTempArc = profile.tempArcsInSeq().at(0);
    TempArc lastTempARc = profile.tempArcsInSeq().at(profile.tempArcsInSeq().size()-1);

    float rpmC = ui->initRPMLineEdit->text().toFloat();
    RPMArc rpmarc(0, 0, rpmC, firstTempArc.startTimeMsec(), lastTempARc.finishTimeMsec()-firstTempArc.startTimeMsec());
    profile.addRPMArcInSequence(rpmarc);

    m_currentExperiment.setProfile(profile);
    m_expGraph->updateTheExperiment(m_currentExperiment);
    m_currentExpType = EXP_UNDEFINED;

}






///ADVANCED EXPERIMENTS----------


void ExperimentCreateWidget::setVisibleAllAdvExperimentItems(bool en)
{
    ui->adv_arcDurationComboBox->setVisible(en);
    ui->adv_arcDurationLabel->setVisible(en);
    ui->adv_arcDurationLineEdit->setVisible(en);
    ui->adv_arcRampLabel->setVisible(en);
    ui->adv_arcRampCLabel->setVisible(en);
    ui->adv_arcRampLineEdit->setVisible(en);
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

    ui->adv_initTempLineEdit->clear();
    ui->adv_finalTempLineEdit->clear();
    ui->adv_arcRampLineEdit->clear();
    ui->adv_arcDurationLineEdit->clear();
    ui->adv_numOfCyclesLineEdit->clear();
    ui->adv_stepCntLieEdit->clear();
    ui->adv_polyALineEdit->clear();

    ui->adv_initTempLineEdit->setEnabled(true);

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

void ExperimentCreateWidget::adv_addPlatueClicked()
{

    setVisibleAllAdvExperimentItems(false);

    ui->adv_initTempCLabel->setVisible(true);
    ui->adv_initTempLabel->setVisible(true);
    ui->adv_initTempLineEdit->setVisible(true);

    ui->adv_arcDurationLineEdit->setVisible(true);
    ui->adv_arcDurationLabel->setVisible(true);
    ui->adv_arcDurationComboBox->setVisible(true);

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_PLATUE;


}


void ExperimentCreateWidget::adv_addLinearClicked()
{
    setVisibleAllAdvExperimentItems(false);

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

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_LIN;
}

void ExperimentCreateWidget::adv_addStepsClicked()
{
    setVisibleAllAdvExperimentItems(false);

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


}

void ExperimentCreateWidget::adv_add2ndDegClicked()
{
    setVisibleAllAdvExperimentItems(false);

    ui->adv_initTempCLabel->setVisible(true);
    ui->adv_initTempLabel->setVisible(true);
    ui->adv_initTempLineEdit->setVisible(true);

    ui->adv_finalTempCLabel->setVisible(true);
    ui->adv_finalTempLabel->setVisible(true);
    ui->adv_finalTempLineEdit->setVisible(true);

    ui->adv_arcDurationLineEdit->setVisible(true);
    ui->adv_arcDurationLabel->setVisible(true);
    ui->adv_arcDurationComboBox->setVisible(true);

    ui->adv_polyALineEdit->setVisible(true);
    ui->adv_polyALabel->setVisible(true);
    ui->adv_polyDescrLabel->setVisible(true);

    adv_forceInitialTemp();
    m_currentExpType = EXP_ADVANCED_2NDDEG;
}


void ExperimentCreateWidget::adv_addArcToExperimentClicked()
{
    Profile profile = m_currentExperiment.profile();

    float initialTemp = ui->adv_initTempLineEdit->text().toFloat();
    unsigned long initTimeMsecs = 0;

    if(profile.tempArcsInSeq().size() > 0){
        initialTemp = profile.tempArcsInSeq().at(profile.tempArcsInSeq().size()-1).finishTemp();
        initTimeMsecs = profile.tempArcsInSeq().at(profile.tempArcsInSeq().size()-1).finishTimeMsec();
    }

    int arcDurationIdx = ui->adv_arcDurationComboBox->currentData().toInt();
    int arcDurationTimeMult = 1;
    if(arcDurationIdx == IDX_TIME_MIN){
        arcDurationTimeMult = 60;
    }else if(arcDurationIdx == IDX_TIME_HOUR){
        arcDurationTimeMult = 3600;
    }
    unsigned long arcDurationMSecs = ui->adv_arcDurationLineEdit->text().toInt() * arcDurationTimeMult * 1000;



    qDebug() << "adv_addArcToExperimentClicked::initTimeMsecs " << initTimeMsecs;






    if(m_currentExpType == EXP_ADVANCED_PLATUE){
        TempArc tempArc(0, 0, initialTemp, initTimeMsecs, arcDurationMSecs);
        profile.addTempArcInSequence(tempArc);
    }


    else if(m_currentExpType == EXP_ADVANCED_LIN){

        float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();
        float ramp = abs(ui->adv_arcRampLineEdit->text().toFloat() / (60*1000.0));
        profile.addTempArcInSequence(createLinearTempArc(initialTemp, finalTemp, ramp, initTimeMsecs));

    }


    else if(m_currentExpType == EXP_ADVANCED_STEPS){

        float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();
        float stepCnt = ui->adv_stepCntLieEdit->text().toInt();
        std::vector<TempArc> stepArcs = createStepTempArc(initialTemp, finalTemp, stepCnt, arcDurationMSecs, initTimeMsecs);
        foreach (TempArc arc, stepArcs) {
            profile.addTempArcInSequence(arc);
        }

    }


    else if(m_currentExpType == EXP_ADVANCED_2NDDEG){

        float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();
        double a = ui->adv_polyALineEdit->text().toDouble();
        qDebug() << "adv_addArcToExperimentClicked::a: " << a;

        TempArc deg2Arc = create2ndDegArc(initialTemp, finalTemp, a, initTimeMsecs, arcDurationMSecs);
        profile.addTempArcInSequence(deg2Arc);
    }



    m_currentExpType = EXP_UNDEFINED;
    setVisibleAllAdvExperimentItems(false);
    m_currentExperiment.setProfile(profile);
    m_expGraph->updateTheExperiment(m_currentExperiment);

}

void ExperimentCreateWidget::adv_delLastArcClicked()
{
    Profile profile = m_currentExperiment.profile();
    if(profile.tempArcsInSeq().size() > 0){
        profile.tempArcsInSeq().erase(profile.tempArcsInSeq().begin()+profile.tempArcsInSeq().size()-1);
        m_currentExperiment.setProfile(profile);
        m_expGraph->updateTheExperiment(m_currentExperiment);
    }
}

void ExperimentCreateWidget::adv_clearProfileClicked()
{
    m_currentExperiment.setProfile(Profile());
    m_expGraph->updateTheExperiment(m_currentExperiment);
}





void ExperimentCreateWidget::adv_addArcDurationChanged()
{
    float initialTemp = ui->adv_initTempLineEdit->text().toFloat();
    float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();

    if(m_currentExpType == EXP_ADVANCED_LIN){


        int arcDurationIdx = ui->adv_arcDurationComboBox->currentData().toInt();
        int arcDurationTimeMult = 1;
        if(arcDurationIdx == IDX_TIME_MIN){
            arcDurationTimeMult = 60;
        }else if(arcDurationIdx == IDX_TIME_HOUR){
            arcDurationTimeMult = 3600;
        }
        unsigned long arcDurationMSecs = ui->adv_arcDurationLineEdit->text().toInt() * arcDurationTimeMult * 1000;
        float arcDurationMin = arcDurationMSecs/(1000.0*60.0);

        float rank = abs(finalTemp-initialTemp)/arcDurationMin;
        ui->adv_arcRampLineEdit->setText(QString::number(rank));
    }
}

void ExperimentCreateWidget::adv_rampChanged()
{

    float initialTemp = ui->adv_initTempLineEdit->text().toFloat();
    float finalTemp = ui->adv_finalTempLineEdit->text().toFloat();

    if(m_currentExpType == EXP_ADVANCED_LIN){
        ui->adv_arcDurationComboBox->setCurrentIndex(IDX_TIME_MIN);

        float rank = ui->adv_arcRampLineEdit->text().toFloat();   //   C/min.

        float durationMin = abs(finalTemp-initialTemp) / rank;
        ui->adv_arcDurationLineEdit->setText(QString::number(durationMin));
    }
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
    float stepUpRank = ui->initRampLineEdit->text().toFloat() / (60*1000.0);  //converting to C/msec
    float stepDownRank = ui->finalRampLineEdit->text().toFloat() / (60*1000.0);  //converting to C/msec


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
    float stepUpRank = ui->initRampLineEdit->text().toFloat() / (60*1000.0);  //converting to C/msec

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
    float stepDownRank = ui->finalRampLineEdit->text().toFloat() / (60*1000.0);  //converting to C/msec


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

