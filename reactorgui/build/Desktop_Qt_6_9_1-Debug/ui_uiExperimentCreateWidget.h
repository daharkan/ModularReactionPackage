/********************************************************************************
** Form generated from reading UI file 'uiExperimentCreateWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIEXPERIMENTCREATEWIDGET_H
#define UI_UIEXPERIMENTCREATEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExperimentCreateWidget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QPushButton *basicStepUpLinDown;
    QFrame *line;
    QPushButton *basicStepUpStepDown;
    QPushButton *advancedProfile;
    QPushButton *basicStandard;
    QPushButton *basicLinUpStepDown;
    QLabel *label;
    QGridLayout *expGraphLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_4;
    QGroupBox *basicStartGroupBox;
    QGridLayout *basicStartLayout;
    QLabel *initTempLabel;
    QLineEdit *initTempLineEdit;
    QLabel *initTempCLabel;
    QLabel *initHoldTimeLabel;
    QLineEdit *initHoldTimeLineEdit;
    QComboBox *initHoldTimeComboBox;
    QLabel *initRPMLabel;
    QLineEdit *initRPMLineEdit;
    QLabel *initRPMRPMLabel;
    QGroupBox *basicTargetGroupBox;
    QGridLayout *basicTargetLayout;
    QLabel *targetTempLabel;
    QLineEdit *targetTempLineEdit;
    QLabel *targetTempCLabel;
    QLabel *initRampLabel;
    QLineEdit *initRampLineEdit;
    QLabel *initRampCLabel;
    QGroupBox *basicRampGroupBox;
    QGridLayout *basicRampLayout;
    QLabel *plateauTimeLabel;
    QLineEdit *plateauTimeLineEdit;
    QComboBox *plateauTimeComboBox;
    QLabel *finalTempLabel;
    QLineEdit *finalTempLineEdit;
    QLabel *finalTempCLabel;
    QLabel *finalRampLabel;
    QLineEdit *finalRampLineEdit;
    QLabel *finalRampCLabel;
    QLabel *finalHoldTimeLabel;
    QLineEdit *finalHoldTimeLineEdit;
    QComboBox *finalHoldTimeCombobox;
    QGroupBox *basicStepsGroupBox;
    QGridLayout *basicStepsLayout;
    QLabel *numOfStepsUpLabel;
    QLineEdit *numOfStepsUpLineEdit;
    QLabel *stepsUpDurationLabel;
    QLineEdit *stepsUpDurationLineEdit;
    QComboBox *stepsUpDurationComboBox;
    QLabel *numOfStepsDownLabel;
    QLineEdit *numOfStepsDownLineEdit;
    QLabel *stepsDownDurationLabel;
    QLineEdit *stepsDownDurationLineEdit;
    QComboBox *stepsDownDurationComboBox;
    QHBoxLayout *basicCyclesLayout;
    QLabel *numOfCyclesLabel;
    QLineEdit *numOfCyclesLineEdit;
    QSpacerItem *basicCyclesSpacer;
    QHBoxLayout *basicActionLayout;
    QSpacerItem *basicActionSpacer;
    QPushButton *createPushButton;
    QWidget *page_2;
    QGridLayout *gridLayout_3;
    QGridLayout *warningMessageBoxLayout;
    QWidget *page_3;
    QGridLayout *gridLayout_5;
    QLineEdit *adv_finalTempLineEdit;
    QLabel *adv_polyDescrLabel;
    QLabel *adv_arcDurationLabel;
    QFrame *line_2;
    QComboBox *adv_arcDurationComboBox;
    QLineEdit *adv_stepCntLieEdit;
    QLineEdit *adv_arcDurationLineEdit;
    QLabel *adv_finalTempLabel;
    QLabel *adv_arcRampCLabel;
    QLabel *adv_initTempCLabel;
    QLabel *adv_stepCntLabel;
    QLabel *adv_polyALabel;
    QSpacerItem *verticalSpacer;
    QLineEdit *adv_polyALineEdit;
    QLabel *adv_initTempLabel;
    QFrame *line_3;
    QLabel *adv_numOfCyclesLabel;
    QLineEdit *adv_arcRampLineEdit;
    QLabel *adv_finalTempCLabel;
    QVBoxLayout *verticalLayout;
    QRadioButton *adv_addPlatueRadioButton;
    QRadioButton *adv_addLinearRadioButton;
    QRadioButton *adv_addStepsRadioButton;
    QRadioButton *adv_add2ndDegRadioButton;
    QVBoxLayout *verticalLayout_2;
    QPushButton *adv_addArcToExpButton;
    QPushButton *adv_delLastArcButton;
    QPushButton *adv_clearProfileButton;
    QHBoxLayout *advActionLayout;
    QSpacerItem *advActionSpacer;
    QPushButton *adv_createPushButton;
    QLineEdit *adv_numOfCyclesLineEdit;
    QLabel *adv_arcRampLabel;
    QLineEdit *adv_initTempLineEdit;

    void setupUi(QWidget *ExperimentCreateWidget)
    {
        if (ExperimentCreateWidget->objectName().isEmpty())
            ExperimentCreateWidget->setObjectName("ExperimentCreateWidget");
        ExperimentCreateWidget->resize(858, 568);
        gridLayout_2 = new QGridLayout(ExperimentCreateWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_2 = new QLabel(ExperimentCreateWidget);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_2, 0, 5, 1, 1);

        basicStepUpLinDown = new QPushButton(ExperimentCreateWidget);
        basicStepUpLinDown->setObjectName("basicStepUpLinDown");
        basicStepUpLinDown->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/stepuplineardown.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        basicStepUpLinDown->setIcon(icon);
        basicStepUpLinDown->setIconSize(QSize(64, 64));
        basicStepUpLinDown->setCheckable(true);

        gridLayout->addWidget(basicStepUpLinDown, 1, 2, 1, 1);

        line = new QFrame(ExperimentCreateWidget);
        line->setObjectName("line");
        line->setLineWidth(3);
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 1, 4, 1, 1);

        basicStepUpStepDown = new QPushButton(ExperimentCreateWidget);
        basicStepUpStepDown->setObjectName("basicStepUpStepDown");
        basicStepUpStepDown->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/stepupstepdown.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        basicStepUpStepDown->setIcon(icon1);
        basicStepUpStepDown->setIconSize(QSize(64, 64));
        basicStepUpStepDown->setCheckable(true);

        gridLayout->addWidget(basicStepUpStepDown, 1, 3, 1, 1);

        advancedProfile = new QPushButton(ExperimentCreateWidget);
        advancedProfile->setObjectName("advancedProfile");
        advancedProfile->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/customGraph.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        advancedProfile->setIcon(icon2);
        advancedProfile->setIconSize(QSize(64, 64));
        advancedProfile->setCheckable(true);

        gridLayout->addWidget(advancedProfile, 1, 5, 1, 1);

        basicStandard = new QPushButton(ExperimentCreateWidget);
        basicStandard->setObjectName("basicStandard");
        basicStandard->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/icons/standard.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        basicStandard->setIcon(icon3);
        basicStandard->setIconSize(QSize(64, 64));
        basicStandard->setCheckable(true);
        basicStandard->setChecked(false);

        gridLayout->addWidget(basicStandard, 1, 0, 1, 1);

        basicLinUpStepDown = new QPushButton(ExperimentCreateWidget);
        basicLinUpStepDown->setObjectName("basicLinUpStepDown");
        basicLinUpStepDown->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/icons/linearupstepdown.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        basicLinUpStepDown->setIcon(icon4);
        basicLinUpStepDown->setIconSize(QSize(64, 64));
        basicLinUpStepDown->setCheckable(true);

        gridLayout->addWidget(basicLinUpStepDown, 1, 1, 1, 1);

        label = new QLabel(ExperimentCreateWidget);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 4);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        expGraphLayout = new QGridLayout();
        expGraphLayout->setObjectName("expGraphLayout");

        gridLayout_2->addLayout(expGraphLayout, 1, 0, 1, 1);

        stackedWidget = new QStackedWidget(ExperimentCreateWidget);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        gridLayout_6 = new QGridLayout(page);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        basicStartGroupBox = new QGroupBox(page);
        basicStartGroupBox->setObjectName("basicStartGroupBox");
        basicStartLayout = new QGridLayout(basicStartGroupBox);
        basicStartLayout->setObjectName("basicStartLayout");
        initTempLabel = new QLabel(basicStartGroupBox);
        initTempLabel->setObjectName("initTempLabel");

        basicStartLayout->addWidget(initTempLabel, 0, 0, 1, 1);

        initTempLineEdit = new QLineEdit(basicStartGroupBox);
        initTempLineEdit->setObjectName("initTempLineEdit");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(initTempLineEdit->sizePolicy().hasHeightForWidth());
        initTempLineEdit->setSizePolicy(sizePolicy);

        basicStartLayout->addWidget(initTempLineEdit, 0, 1, 1, 1);

        initTempCLabel = new QLabel(basicStartGroupBox);
        initTempCLabel->setObjectName("initTempCLabel");

        basicStartLayout->addWidget(initTempCLabel, 0, 2, 1, 1);

        initHoldTimeLabel = new QLabel(basicStartGroupBox);
        initHoldTimeLabel->setObjectName("initHoldTimeLabel");

        basicStartLayout->addWidget(initHoldTimeLabel, 1, 0, 1, 1);

        initHoldTimeLineEdit = new QLineEdit(basicStartGroupBox);
        initHoldTimeLineEdit->setObjectName("initHoldTimeLineEdit");

        basicStartLayout->addWidget(initHoldTimeLineEdit, 1, 1, 1, 1);

        initHoldTimeComboBox = new QComboBox(basicStartGroupBox);
        initHoldTimeComboBox->setObjectName("initHoldTimeComboBox");

        basicStartLayout->addWidget(initHoldTimeComboBox, 1, 2, 1, 1);

        initRPMLabel = new QLabel(basicStartGroupBox);
        initRPMLabel->setObjectName("initRPMLabel");

        basicStartLayout->addWidget(initRPMLabel, 2, 0, 1, 1);

        initRPMLineEdit = new QLineEdit(basicStartGroupBox);
        initRPMLineEdit->setObjectName("initRPMLineEdit");

        basicStartLayout->addWidget(initRPMLineEdit, 2, 1, 1, 1);

        initRPMRPMLabel = new QLabel(basicStartGroupBox);
        initRPMRPMLabel->setObjectName("initRPMRPMLabel");

        basicStartLayout->addWidget(initRPMRPMLabel, 2, 2, 1, 1);


        gridLayout_4->addWidget(basicStartGroupBox, 0, 0, 1, 1);

        basicTargetGroupBox = new QGroupBox(page);
        basicTargetGroupBox->setObjectName("basicTargetGroupBox");
        basicTargetLayout = new QGridLayout(basicTargetGroupBox);
        basicTargetLayout->setObjectName("basicTargetLayout");
        targetTempLabel = new QLabel(basicTargetGroupBox);
        targetTempLabel->setObjectName("targetTempLabel");

        basicTargetLayout->addWidget(targetTempLabel, 0, 0, 1, 1);

        targetTempLineEdit = new QLineEdit(basicTargetGroupBox);
        targetTempLineEdit->setObjectName("targetTempLineEdit");

        basicTargetLayout->addWidget(targetTempLineEdit, 0, 1, 1, 1);

        targetTempCLabel = new QLabel(basicTargetGroupBox);
        targetTempCLabel->setObjectName("targetTempCLabel");

        basicTargetLayout->addWidget(targetTempCLabel, 0, 2, 1, 1);

        initRampLabel = new QLabel(basicTargetGroupBox);
        initRampLabel->setObjectName("initRampLabel");

        basicTargetLayout->addWidget(initRampLabel, 1, 0, 1, 1);

        initRampLineEdit = new QLineEdit(basicTargetGroupBox);
        initRampLineEdit->setObjectName("initRampLineEdit");

        basicTargetLayout->addWidget(initRampLineEdit, 1, 1, 1, 1);

        initRampCLabel = new QLabel(basicTargetGroupBox);
        initRampCLabel->setObjectName("initRampCLabel");

        basicTargetLayout->addWidget(initRampCLabel, 1, 2, 1, 1);


        gridLayout_4->addWidget(basicTargetGroupBox, 0, 1, 1, 1);

        basicRampGroupBox = new QGroupBox(page);
        basicRampGroupBox->setObjectName("basicRampGroupBox");
        basicRampLayout = new QGridLayout(basicRampGroupBox);
        basicRampLayout->setObjectName("basicRampLayout");
        plateauTimeLabel = new QLabel(basicRampGroupBox);
        plateauTimeLabel->setObjectName("plateauTimeLabel");

        basicRampLayout->addWidget(plateauTimeLabel, 0, 0, 1, 1);

        plateauTimeLineEdit = new QLineEdit(basicRampGroupBox);
        plateauTimeLineEdit->setObjectName("plateauTimeLineEdit");

        basicRampLayout->addWidget(plateauTimeLineEdit, 0, 1, 1, 1);

        plateauTimeComboBox = new QComboBox(basicRampGroupBox);
        plateauTimeComboBox->setObjectName("plateauTimeComboBox");

        basicRampLayout->addWidget(plateauTimeComboBox, 0, 2, 1, 1);

        finalTempLabel = new QLabel(basicRampGroupBox);
        finalTempLabel->setObjectName("finalTempLabel");

        basicRampLayout->addWidget(finalTempLabel, 1, 0, 1, 1);

        finalTempLineEdit = new QLineEdit(basicRampGroupBox);
        finalTempLineEdit->setObjectName("finalTempLineEdit");

        basicRampLayout->addWidget(finalTempLineEdit, 1, 1, 1, 1);

        finalTempCLabel = new QLabel(basicRampGroupBox);
        finalTempCLabel->setObjectName("finalTempCLabel");

        basicRampLayout->addWidget(finalTempCLabel, 1, 2, 1, 1);

        finalRampLabel = new QLabel(basicRampGroupBox);
        finalRampLabel->setObjectName("finalRampLabel");

        basicRampLayout->addWidget(finalRampLabel, 2, 0, 1, 1);

        finalRampLineEdit = new QLineEdit(basicRampGroupBox);
        finalRampLineEdit->setObjectName("finalRampLineEdit");

        basicRampLayout->addWidget(finalRampLineEdit, 2, 1, 1, 1);

        finalRampCLabel = new QLabel(basicRampGroupBox);
        finalRampCLabel->setObjectName("finalRampCLabel");

        basicRampLayout->addWidget(finalRampCLabel, 2, 2, 1, 1);

        finalHoldTimeLabel = new QLabel(basicRampGroupBox);
        finalHoldTimeLabel->setObjectName("finalHoldTimeLabel");

        basicRampLayout->addWidget(finalHoldTimeLabel, 3, 0, 1, 1);

        finalHoldTimeLineEdit = new QLineEdit(basicRampGroupBox);
        finalHoldTimeLineEdit->setObjectName("finalHoldTimeLineEdit");

        basicRampLayout->addWidget(finalHoldTimeLineEdit, 3, 1, 1, 1);

        finalHoldTimeCombobox = new QComboBox(basicRampGroupBox);
        finalHoldTimeCombobox->setObjectName("finalHoldTimeCombobox");

        basicRampLayout->addWidget(finalHoldTimeCombobox, 3, 2, 1, 1);


        gridLayout_4->addWidget(basicRampGroupBox, 1, 0, 1, 1);

        basicStepsGroupBox = new QGroupBox(page);
        basicStepsGroupBox->setObjectName("basicStepsGroupBox");
        basicStepsLayout = new QGridLayout(basicStepsGroupBox);
        basicStepsLayout->setObjectName("basicStepsLayout");
        numOfStepsUpLabel = new QLabel(basicStepsGroupBox);
        numOfStepsUpLabel->setObjectName("numOfStepsUpLabel");

        basicStepsLayout->addWidget(numOfStepsUpLabel, 0, 0, 1, 1);

        numOfStepsUpLineEdit = new QLineEdit(basicStepsGroupBox);
        numOfStepsUpLineEdit->setObjectName("numOfStepsUpLineEdit");

        basicStepsLayout->addWidget(numOfStepsUpLineEdit, 0, 1, 1, 1);

        stepsUpDurationLabel = new QLabel(basicStepsGroupBox);
        stepsUpDurationLabel->setObjectName("stepsUpDurationLabel");

        basicStepsLayout->addWidget(stepsUpDurationLabel, 1, 0, 1, 1);

        stepsUpDurationLineEdit = new QLineEdit(basicStepsGroupBox);
        stepsUpDurationLineEdit->setObjectName("stepsUpDurationLineEdit");

        basicStepsLayout->addWidget(stepsUpDurationLineEdit, 1, 1, 1, 1);

        stepsUpDurationComboBox = new QComboBox(basicStepsGroupBox);
        stepsUpDurationComboBox->setObjectName("stepsUpDurationComboBox");

        basicStepsLayout->addWidget(stepsUpDurationComboBox, 1, 2, 1, 1);

        numOfStepsDownLabel = new QLabel(basicStepsGroupBox);
        numOfStepsDownLabel->setObjectName("numOfStepsDownLabel");

        basicStepsLayout->addWidget(numOfStepsDownLabel, 2, 0, 1, 1);

        numOfStepsDownLineEdit = new QLineEdit(basicStepsGroupBox);
        numOfStepsDownLineEdit->setObjectName("numOfStepsDownLineEdit");

        basicStepsLayout->addWidget(numOfStepsDownLineEdit, 2, 1, 1, 1);

        stepsDownDurationLabel = new QLabel(basicStepsGroupBox);
        stepsDownDurationLabel->setObjectName("stepsDownDurationLabel");

        basicStepsLayout->addWidget(stepsDownDurationLabel, 3, 0, 1, 1);

        stepsDownDurationLineEdit = new QLineEdit(basicStepsGroupBox);
        stepsDownDurationLineEdit->setObjectName("stepsDownDurationLineEdit");

        basicStepsLayout->addWidget(stepsDownDurationLineEdit, 3, 1, 1, 1);

        stepsDownDurationComboBox = new QComboBox(basicStepsGroupBox);
        stepsDownDurationComboBox->setObjectName("stepsDownDurationComboBox");

        basicStepsLayout->addWidget(stepsDownDurationComboBox, 3, 2, 1, 1);


        gridLayout_4->addWidget(basicStepsGroupBox, 1, 1, 1, 1);

        basicCyclesLayout = new QHBoxLayout();
        basicCyclesLayout->setObjectName("basicCyclesLayout");
        numOfCyclesLabel = new QLabel(page);
        numOfCyclesLabel->setObjectName("numOfCyclesLabel");

        basicCyclesLayout->addWidget(numOfCyclesLabel);

        numOfCyclesLineEdit = new QLineEdit(page);
        numOfCyclesLineEdit->setObjectName("numOfCyclesLineEdit");

        basicCyclesLayout->addWidget(numOfCyclesLineEdit);

        basicCyclesSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        basicCyclesLayout->addItem(basicCyclesSpacer);


        gridLayout_4->addLayout(basicCyclesLayout, 2, 0, 1, 2);

        basicActionLayout = new QHBoxLayout();
        basicActionLayout->setObjectName("basicActionLayout");
        basicActionSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        basicActionLayout->addItem(basicActionSpacer);

        createPushButton = new QPushButton(page);
        createPushButton->setObjectName("createPushButton");

        basicActionLayout->addWidget(createPushButton);


        gridLayout_4->addLayout(basicActionLayout, 3, 0, 1, 2);

        gridLayout_4->setRowStretch(0, 1);
        gridLayout_4->setRowStretch(1, 1);
        gridLayout_4->setColumnStretch(0, 1);
        gridLayout_4->setColumnStretch(1, 1);

        gridLayout_6->addLayout(gridLayout_4, 0, 1, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        gridLayout_3 = new QGridLayout(page_2);
        gridLayout_3->setObjectName("gridLayout_3");
        warningMessageBoxLayout = new QGridLayout();
        warningMessageBoxLayout->setObjectName("warningMessageBoxLayout");

        gridLayout_3->addLayout(warningMessageBoxLayout, 0, 0, 1, 1);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        gridLayout_5 = new QGridLayout(page_3);
        gridLayout_5->setObjectName("gridLayout_5");
        adv_finalTempLineEdit = new QLineEdit(page_3);
        adv_finalTempLineEdit->setObjectName("adv_finalTempLineEdit");

        gridLayout_5->addWidget(adv_finalTempLineEdit, 0, 5, 1, 1);

        adv_polyDescrLabel = new QLabel(page_3);
        adv_polyDescrLabel->setObjectName("adv_polyDescrLabel");
        adv_polyDescrLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_5->addWidget(adv_polyDescrLabel, 6, 4, 1, 3);

        adv_arcDurationLabel = new QLabel(page_3);
        adv_arcDurationLabel->setObjectName("adv_arcDurationLabel");

        gridLayout_5->addWidget(adv_arcDurationLabel, 3, 0, 1, 1);

        line_2 = new QFrame(page_3);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::VLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_5->addWidget(line_2, 0, 3, 5, 1);

        adv_arcDurationComboBox = new QComboBox(page_3);
        adv_arcDurationComboBox->setObjectName("adv_arcDurationComboBox");

        gridLayout_5->addWidget(adv_arcDurationComboBox, 3, 2, 1, 1);

        adv_stepCntLieEdit = new QLineEdit(page_3);
        adv_stepCntLieEdit->setObjectName("adv_stepCntLieEdit");

        gridLayout_5->addWidget(adv_stepCntLieEdit, 4, 1, 1, 1);

        adv_arcDurationLineEdit = new QLineEdit(page_3);
        adv_arcDurationLineEdit->setObjectName("adv_arcDurationLineEdit");

        gridLayout_5->addWidget(adv_arcDurationLineEdit, 3, 1, 1, 1);

        adv_finalTempLabel = new QLabel(page_3);
        adv_finalTempLabel->setObjectName("adv_finalTempLabel");

        gridLayout_5->addWidget(adv_finalTempLabel, 0, 4, 1, 1);

        adv_arcRampCLabel = new QLabel(page_3);
        adv_arcRampCLabel->setObjectName("adv_arcRampCLabel");

        gridLayout_5->addWidget(adv_arcRampCLabel, 3, 6, 1, 1);

        adv_initTempCLabel = new QLabel(page_3);
        adv_initTempCLabel->setObjectName("adv_initTempCLabel");

        gridLayout_5->addWidget(adv_initTempCLabel, 0, 2, 1, 1);

        adv_stepCntLabel = new QLabel(page_3);
        adv_stepCntLabel->setObjectName("adv_stepCntLabel");

        gridLayout_5->addWidget(adv_stepCntLabel, 4, 0, 1, 1);

        adv_polyALabel = new QLabel(page_3);
        adv_polyALabel->setObjectName("adv_polyALabel");
        adv_polyALabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        adv_polyALabel->setWordWrap(true);

        gridLayout_5->addWidget(adv_polyALabel, 6, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 9, 0, 1, 1);

        adv_polyALineEdit = new QLineEdit(page_3);
        adv_polyALineEdit->setObjectName("adv_polyALineEdit");

        gridLayout_5->addWidget(adv_polyALineEdit, 6, 2, 1, 1);

        adv_initTempLabel = new QLabel(page_3);
        adv_initTempLabel->setObjectName("adv_initTempLabel");

        gridLayout_5->addWidget(adv_initTempLabel, 0, 0, 1, 1);

        line_3 = new QFrame(page_3);
        line_3->setObjectName("line_3");
        line_3->setFrameShape(QFrame::Shape::HLine);
        line_3->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_5->addWidget(line_3, 5, 0, 1, 7);

        adv_numOfCyclesLabel = new QLabel(page_3);
        adv_numOfCyclesLabel->setObjectName("adv_numOfCyclesLabel");

        gridLayout_5->addWidget(adv_numOfCyclesLabel, 10, 5, 1, 1);

        adv_arcRampLineEdit = new QLineEdit(page_3);
        adv_arcRampLineEdit->setObjectName("adv_arcRampLineEdit");

        gridLayout_5->addWidget(adv_arcRampLineEdit, 3, 5, 1, 1);

        adv_finalTempCLabel = new QLabel(page_3);
        adv_finalTempCLabel->setObjectName("adv_finalTempCLabel");

        gridLayout_5->addWidget(adv_finalTempCLabel, 0, 6, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        adv_addPlatueRadioButton = new QRadioButton(page_3);
        adv_addPlatueRadioButton->setObjectName("adv_addPlatueRadioButton");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/icons/platue.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_addPlatueRadioButton->setIcon(icon5);
        adv_addPlatueRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_addPlatueRadioButton);

        adv_addLinearRadioButton = new QRadioButton(page_3);
        adv_addLinearRadioButton->setObjectName("adv_addLinearRadioButton");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/icons/linears.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_addLinearRadioButton->setIcon(icon6);
        adv_addLinearRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_addLinearRadioButton);

        adv_addStepsRadioButton = new QRadioButton(page_3);
        adv_addStepsRadioButton->setObjectName("adv_addStepsRadioButton");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/icons/steps.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_addStepsRadioButton->setIcon(icon7);
        adv_addStepsRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_addStepsRadioButton);

        adv_add2ndDegRadioButton = new QRadioButton(page_3);
        adv_add2ndDegRadioButton->setObjectName("adv_add2ndDegRadioButton");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/icons/secondDegs.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_add2ndDegRadioButton->setIcon(icon8);
        adv_add2ndDegRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_add2ndDegRadioButton);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        adv_addArcToExpButton = new QPushButton(page_3);
        adv_addArcToExpButton->setObjectName("adv_addArcToExpButton");
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resources/icons/plus.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_addArcToExpButton->setIcon(icon9);
        adv_addArcToExpButton->setIconSize(QSize(32, 32));
        adv_addArcToExpButton->setAutoDefault(true);

        verticalLayout_2->addWidget(adv_addArcToExpButton);

        adv_delLastArcButton = new QPushButton(page_3);
        adv_delLastArcButton->setObjectName("adv_delLastArcButton");
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resources/icons/minus.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_delLastArcButton->setIcon(icon10);

        verticalLayout_2->addWidget(adv_delLastArcButton);

        adv_clearProfileButton = new QPushButton(page_3);
        adv_clearProfileButton->setObjectName("adv_clearProfileButton");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resources/icons/block.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        adv_clearProfileButton->setIcon(icon11);

        verticalLayout_2->addWidget(adv_clearProfileButton);


        verticalLayout->addLayout(verticalLayout_2);


        gridLayout_5->addLayout(verticalLayout, 0, 7, 13, 1);

        advActionLayout = new QHBoxLayout();
        advActionLayout->setObjectName("advActionLayout");
        advActionSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        advActionLayout->addItem(advActionSpacer);

        adv_createPushButton = new QPushButton(page_3);
        adv_createPushButton->setObjectName("adv_createPushButton");

        advActionLayout->addWidget(adv_createPushButton);


        gridLayout_5->addLayout(advActionLayout, 11, 0, 1, 7);

        adv_numOfCyclesLineEdit = new QLineEdit(page_3);
        adv_numOfCyclesLineEdit->setObjectName("adv_numOfCyclesLineEdit");

        gridLayout_5->addWidget(adv_numOfCyclesLineEdit, 10, 6, 1, 1);

        adv_arcRampLabel = new QLabel(page_3);
        adv_arcRampLabel->setObjectName("adv_arcRampLabel");

        gridLayout_5->addWidget(adv_arcRampLabel, 3, 4, 1, 1);

        adv_initTempLineEdit = new QLineEdit(page_3);
        adv_initTempLineEdit->setObjectName("adv_initTempLineEdit");

        gridLayout_5->addWidget(adv_initTempLineEdit, 0, 1, 1, 1);

        stackedWidget->addWidget(page_3);

        gridLayout_2->addWidget(stackedWidget, 2, 0, 1, 1);


        retranslateUi(ExperimentCreateWidget);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(ExperimentCreateWidget);
    } // setupUi

    void retranslateUi(QWidget *ExperimentCreateWidget)
    {
        ExperimentCreateWidget->setWindowTitle(QCoreApplication::translate("ExperimentCreateWidget", "Form", nullptr));
        label_2->setText(QCoreApplication::translate("ExperimentCreateWidget", "Advanced", nullptr));
        basicStepUpLinDown->setText(QString());
        basicStepUpStepDown->setText(QString());
        advancedProfile->setText(QString());
        basicStandard->setText(QString());
        basicLinUpStepDown->setText(QString());
        label->setText(QCoreApplication::translate("ExperimentCreateWidget", "Basic", nullptr));
        basicStartGroupBox->setTitle(QCoreApplication::translate("ExperimentCreateWidget", "Starts", nullptr));
        initTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial temperature:", nullptr));
        initTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        initHoldTimeLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial Holding Time:", nullptr));
        initRPMLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial Stir speed:", nullptr));
        initRPMRPMLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "RPM", nullptr));
        basicTargetGroupBox->setTitle(QCoreApplication::translate("ExperimentCreateWidget", "Targets", nullptr));
        targetTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Target Temperature:", nullptr));
        targetTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        initRampLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial Ramp:", nullptr));
        initRampCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C/min", nullptr));
        basicRampGroupBox->setTitle(QCoreApplication::translate("ExperimentCreateWidget", "Ramps & Plateaus", nullptr));
        plateauTimeLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Plateau Time:", nullptr));
        finalTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final Temperature:", nullptr));
        finalTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        finalRampLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final Ramp:", nullptr));
        finalRampCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C/min", nullptr));
        finalHoldTimeLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final holding time:", nullptr));
        basicStepsGroupBox->setTitle(QCoreApplication::translate("ExperimentCreateWidget", "Steps", nullptr));
        numOfStepsUpLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of Steps:", nullptr));
        stepsUpDurationLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Total Step Up Duration:", nullptr));
        numOfStepsDownLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of Steps:", nullptr));
        stepsDownDurationLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Total Step Down Duration:", nullptr));
        numOfCyclesLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of cycles:", nullptr));
        createPushButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Create", nullptr));
        adv_polyDescrLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "<html><head/><body><p align=\"center\"><span style=\" font-weight:700; color:#bf4040;\">(a.x\302\262 + b.x + c)</span></p></body></html>", nullptr));
        adv_arcDurationLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Arc Duration:", nullptr));
        adv_finalTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final Temperature:", nullptr));
        adv_arcRampCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C/min", nullptr));
        adv_initTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        adv_stepCntLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Step Count:", nullptr));
        adv_polyALabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Polynom Coefficent  a:", nullptr));
        adv_initTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial temperature:", nullptr));
        adv_numOfCyclesLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of Cycles:", nullptr));
        adv_finalTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        adv_addPlatueRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add Platue", nullptr));
        adv_addLinearRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add Linear", nullptr));
        adv_addStepsRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add Steps", nullptr));
        adv_add2ndDegRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add 2nd Deg.", nullptr));
        adv_addArcToExpButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add", nullptr));
        adv_delLastArcButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Delete Last Arc", nullptr));
        adv_clearProfileButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Clear Profile", nullptr));
        adv_createPushButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Create", nullptr));
        adv_arcRampLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Arc Ramp:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExperimentCreateWidget: public Ui_ExperimentCreateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIEXPERIMENTCREATEWIDGET_H
