/********************************************************************************
** Form generated from reading UI file 'uiExperimentCreateWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
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
    QPushButton *createPushButton;
    QLabel *numOfStepsUpLabel;
    QComboBox *initHoldTimeComboBox;
    QLabel *targetTempCLabel;
    QLabel *targetTempLabel;
    QComboBox *finalHoldTimeCombobox;
    QLineEdit *initHoldTimeLineEdit;
    QLineEdit *finalHoldTimeLineEdit;
    QLabel *initHoldTimeLabel;
    QLineEdit *targetTempLineEdit;
    QLabel *finalHoldTimeLabel;
    QLabel *initRPMRPMLabel;
    QComboBox *stepsUpDurationComboBox;
    QLabel *stepsUpDurationLabel;
    QLineEdit *stepsUpDurationLineEdit;
    QLabel *initRPMLabel;
    QLineEdit *numOfCyclesLineEdit;
    QLineEdit *numOfStepsUpLineEdit;
    QLineEdit *initTempLineEdit;
    QLabel *numOfCyclesLabel;
    QLabel *initTempLabel;
    QLabel *initTempCLabel;
    QLineEdit *initRPMLineEdit;
    QLabel *initRampLabel;
    QLineEdit *initRampLineEdit;
    QLabel *initRampCLabel;
    QLabel *finalTempLabel;
    QLineEdit *finalTempLineEdit;
    QLabel *finalTempCLabel;
    QLabel *numOfStepsDownLabel;
    QLineEdit *numOfStepsDownLineEdit;
    QComboBox *stepsDownDurationComboBox;
    QLineEdit *stepsDownDurationLineEdit;
    QLabel *stepsDownDurationLabel;
    QComboBox *plateauTimeComboBox;
    QLineEdit *plateauTimeLineEdit;
    QLabel *plateauTimeLabel;
    QLabel *finalRampLabel;
    QLineEdit *finalRampLineEdit;
    QLabel *finalRampCLabel;
    QWidget *page_2;
    QGridLayout *gridLayout_3;
    QGridLayout *warningMessageBoxLayout;
    QWidget *page_3;
    QGridLayout *gridLayout_5;
    QFrame *line_2;
    QLabel *adv_initTempCLabel;
    QLineEdit *adv_numOfCyclesLineEdit;
    QLabel *adv_arcDurationLabel;
    QSpacerItem *verticalSpacer;
    QLabel *adv_stepCntLabel;
    QLabel *adv_finalTempLabel;
    QLineEdit *adv_initTempLineEdit;
    QLineEdit *adv_arcRampLineEdit;
    QFrame *line_3;
    QLabel *adv_polyALabel;
    QLineEdit *adv_stepCntLieEdit;
    QLabel *adv_finalTempCLabel;
    QLabel *adv_arcRampLabel;
    QLineEdit *adv_polyALineEdit;
    QLineEdit *adv_finalTempLineEdit;
    QLabel *adv_arcRampCLabel;
    QLabel *adv_initTempLabel;
    QComboBox *adv_arcDurationComboBox;
    QLineEdit *adv_arcDurationLineEdit;
    QVBoxLayout *verticalLayout;
    QRadioButton *adv_addPlatueRadioButton;
    QRadioButton *adv_addLinearRadioButton;
    QRadioButton *adv_addStepsRadioButton;
    QRadioButton *adv_add2ndDegRadioButton;
    QVBoxLayout *verticalLayout_2;
    QPushButton *adv_addArcToExpButton;
    QPushButton *adv_delLastArcButton;
    QPushButton *adv_clearProfileButton;
    QLabel *adv_numOfCyclesLabel;
    QLabel *adv_polyDescrLabel;

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
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 5, 1, 1);

        basicStepUpLinDown = new QPushButton(ExperimentCreateWidget);
        basicStepUpLinDown->setObjectName("basicStepUpLinDown");
        basicStepUpLinDown->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/stepuplineardown.png"), QSize(), QIcon::Normal, QIcon::Off);
        basicStepUpLinDown->setIcon(icon);
        basicStepUpLinDown->setIconSize(QSize(64, 64));
        basicStepUpLinDown->setCheckable(true);

        gridLayout->addWidget(basicStepUpLinDown, 1, 2, 1, 1);

        line = new QFrame(ExperimentCreateWidget);
        line->setObjectName("line");
        line->setLineWidth(3);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 4, 1, 1);

        basicStepUpStepDown = new QPushButton(ExperimentCreateWidget);
        basicStepUpStepDown->setObjectName("basicStepUpStepDown");
        basicStepUpStepDown->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/stepupstepdown.png"), QSize(), QIcon::Normal, QIcon::Off);
        basicStepUpStepDown->setIcon(icon1);
        basicStepUpStepDown->setIconSize(QSize(64, 64));
        basicStepUpStepDown->setCheckable(true);

        gridLayout->addWidget(basicStepUpStepDown, 1, 3, 1, 1);

        advancedProfile = new QPushButton(ExperimentCreateWidget);
        advancedProfile->setObjectName("advancedProfile");
        advancedProfile->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/customGraph.png"), QSize(), QIcon::Normal, QIcon::Off);
        advancedProfile->setIcon(icon2);
        advancedProfile->setIconSize(QSize(64, 64));
        advancedProfile->setCheckable(true);

        gridLayout->addWidget(advancedProfile, 1, 5, 1, 1);

        basicStandard = new QPushButton(ExperimentCreateWidget);
        basicStandard->setObjectName("basicStandard");
        basicStandard->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/icons/standard.png"), QSize(), QIcon::Normal, QIcon::Off);
        basicStandard->setIcon(icon3);
        basicStandard->setIconSize(QSize(64, 64));
        basicStandard->setCheckable(true);
        basicStandard->setChecked(false);

        gridLayout->addWidget(basicStandard, 1, 0, 1, 1);

        basicLinUpStepDown = new QPushButton(ExperimentCreateWidget);
        basicLinUpStepDown->setObjectName("basicLinUpStepDown");
        basicLinUpStepDown->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/icons/linearupstepdown.png"), QSize(), QIcon::Normal, QIcon::Off);
        basicLinUpStepDown->setIcon(icon4);
        basicLinUpStepDown->setIconSize(QSize(64, 64));
        basicLinUpStepDown->setCheckable(true);

        gridLayout->addWidget(basicLinUpStepDown, 1, 1, 1, 1);

        label = new QLabel(ExperimentCreateWidget);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

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
        createPushButton = new QPushButton(page);
        createPushButton->setObjectName("createPushButton");

        gridLayout_4->addWidget(createPushButton, 9, 0, 1, 1);

        numOfStepsUpLabel = new QLabel(page);
        numOfStepsUpLabel->setObjectName("numOfStepsUpLabel");

        gridLayout_4->addWidget(numOfStepsUpLabel, 6, 0, 1, 1);

        initHoldTimeComboBox = new QComboBox(page);
        initHoldTimeComboBox->setObjectName("initHoldTimeComboBox");

        gridLayout_4->addWidget(initHoldTimeComboBox, 2, 2, 1, 1);

        targetTempCLabel = new QLabel(page);
        targetTempCLabel->setObjectName("targetTempCLabel");

        gridLayout_4->addWidget(targetTempCLabel, 3, 2, 1, 1);

        targetTempLabel = new QLabel(page);
        targetTempLabel->setObjectName("targetTempLabel");

        gridLayout_4->addWidget(targetTempLabel, 3, 0, 1, 1);

        finalHoldTimeCombobox = new QComboBox(page);
        finalHoldTimeCombobox->setObjectName("finalHoldTimeCombobox");

        gridLayout_4->addWidget(finalHoldTimeCombobox, 5, 5, 1, 1);

        initHoldTimeLineEdit = new QLineEdit(page);
        initHoldTimeLineEdit->setObjectName("initHoldTimeLineEdit");

        gridLayout_4->addWidget(initHoldTimeLineEdit, 2, 1, 1, 1);

        finalHoldTimeLineEdit = new QLineEdit(page);
        finalHoldTimeLineEdit->setObjectName("finalHoldTimeLineEdit");

        gridLayout_4->addWidget(finalHoldTimeLineEdit, 5, 4, 1, 1);

        initHoldTimeLabel = new QLabel(page);
        initHoldTimeLabel->setObjectName("initHoldTimeLabel");

        gridLayout_4->addWidget(initHoldTimeLabel, 2, 0, 1, 1);

        targetTempLineEdit = new QLineEdit(page);
        targetTempLineEdit->setObjectName("targetTempLineEdit");

        gridLayout_4->addWidget(targetTempLineEdit, 3, 1, 1, 1);

        finalHoldTimeLabel = new QLabel(page);
        finalHoldTimeLabel->setObjectName("finalHoldTimeLabel");

        gridLayout_4->addWidget(finalHoldTimeLabel, 5, 3, 1, 1);

        initRPMRPMLabel = new QLabel(page);
        initRPMRPMLabel->setObjectName("initRPMRPMLabel");

        gridLayout_4->addWidget(initRPMRPMLabel, 0, 2, 1, 1);

        stepsUpDurationComboBox = new QComboBox(page);
        stepsUpDurationComboBox->setObjectName("stepsUpDurationComboBox");

        gridLayout_4->addWidget(stepsUpDurationComboBox, 5, 2, 1, 1);

        stepsUpDurationLabel = new QLabel(page);
        stepsUpDurationLabel->setObjectName("stepsUpDurationLabel");

        gridLayout_4->addWidget(stepsUpDurationLabel, 5, 0, 1, 1);

        stepsUpDurationLineEdit = new QLineEdit(page);
        stepsUpDurationLineEdit->setObjectName("stepsUpDurationLineEdit");

        gridLayout_4->addWidget(stepsUpDurationLineEdit, 5, 1, 1, 1);

        initRPMLabel = new QLabel(page);
        initRPMLabel->setObjectName("initRPMLabel");

        gridLayout_4->addWidget(initRPMLabel, 0, 0, 1, 1);

        numOfCyclesLineEdit = new QLineEdit(page);
        numOfCyclesLineEdit->setObjectName("numOfCyclesLineEdit");

        gridLayout_4->addWidget(numOfCyclesLineEdit, 6, 4, 1, 1);

        numOfStepsUpLineEdit = new QLineEdit(page);
        numOfStepsUpLineEdit->setObjectName("numOfStepsUpLineEdit");

        gridLayout_4->addWidget(numOfStepsUpLineEdit, 6, 1, 1, 1);

        initTempLineEdit = new QLineEdit(page);
        initTempLineEdit->setObjectName("initTempLineEdit");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(initTempLineEdit->sizePolicy().hasHeightForWidth());
        initTempLineEdit->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(initTempLineEdit, 1, 1, 1, 1);

        numOfCyclesLabel = new QLabel(page);
        numOfCyclesLabel->setObjectName("numOfCyclesLabel");

        gridLayout_4->addWidget(numOfCyclesLabel, 6, 3, 1, 1);

        initTempLabel = new QLabel(page);
        initTempLabel->setObjectName("initTempLabel");

        gridLayout_4->addWidget(initTempLabel, 1, 0, 1, 1);

        initTempCLabel = new QLabel(page);
        initTempCLabel->setObjectName("initTempCLabel");

        gridLayout_4->addWidget(initTempCLabel, 1, 2, 1, 1);

        initRPMLineEdit = new QLineEdit(page);
        initRPMLineEdit->setObjectName("initRPMLineEdit");

        gridLayout_4->addWidget(initRPMLineEdit, 0, 1, 1, 1);

        initRampLabel = new QLabel(page);
        initRampLabel->setObjectName("initRampLabel");

        gridLayout_4->addWidget(initRampLabel, 4, 0, 1, 1);

        initRampLineEdit = new QLineEdit(page);
        initRampLineEdit->setObjectName("initRampLineEdit");

        gridLayout_4->addWidget(initRampLineEdit, 4, 1, 1, 1);

        initRampCLabel = new QLabel(page);
        initRampCLabel->setObjectName("initRampCLabel");

        gridLayout_4->addWidget(initRampCLabel, 4, 2, 1, 1);

        finalTempLabel = new QLabel(page);
        finalTempLabel->setObjectName("finalTempLabel");

        gridLayout_4->addWidget(finalTempLabel, 3, 3, 1, 1);

        finalTempLineEdit = new QLineEdit(page);
        finalTempLineEdit->setObjectName("finalTempLineEdit");

        gridLayout_4->addWidget(finalTempLineEdit, 3, 4, 1, 1);

        finalTempCLabel = new QLabel(page);
        finalTempCLabel->setObjectName("finalTempCLabel");

        gridLayout_4->addWidget(finalTempCLabel, 3, 5, 1, 1);

        numOfStepsDownLabel = new QLabel(page);
        numOfStepsDownLabel->setObjectName("numOfStepsDownLabel");

        gridLayout_4->addWidget(numOfStepsDownLabel, 2, 3, 1, 1);

        numOfStepsDownLineEdit = new QLineEdit(page);
        numOfStepsDownLineEdit->setObjectName("numOfStepsDownLineEdit");

        gridLayout_4->addWidget(numOfStepsDownLineEdit, 2, 4, 1, 1);

        stepsDownDurationComboBox = new QComboBox(page);
        stepsDownDurationComboBox->setObjectName("stepsDownDurationComboBox");

        gridLayout_4->addWidget(stepsDownDurationComboBox, 1, 5, 1, 1);

        stepsDownDurationLineEdit = new QLineEdit(page);
        stepsDownDurationLineEdit->setObjectName("stepsDownDurationLineEdit");

        gridLayout_4->addWidget(stepsDownDurationLineEdit, 1, 4, 1, 1);

        stepsDownDurationLabel = new QLabel(page);
        stepsDownDurationLabel->setObjectName("stepsDownDurationLabel");

        gridLayout_4->addWidget(stepsDownDurationLabel, 1, 3, 1, 1);

        plateauTimeComboBox = new QComboBox(page);
        plateauTimeComboBox->setObjectName("plateauTimeComboBox");

        gridLayout_4->addWidget(plateauTimeComboBox, 0, 5, 1, 1);

        plateauTimeLineEdit = new QLineEdit(page);
        plateauTimeLineEdit->setObjectName("plateauTimeLineEdit");

        gridLayout_4->addWidget(plateauTimeLineEdit, 0, 4, 1, 1);

        plateauTimeLabel = new QLabel(page);
        plateauTimeLabel->setObjectName("plateauTimeLabel");

        gridLayout_4->addWidget(plateauTimeLabel, 0, 3, 1, 1);

        finalRampLabel = new QLabel(page);
        finalRampLabel->setObjectName("finalRampLabel");

        gridLayout_4->addWidget(finalRampLabel, 4, 3, 1, 1);

        finalRampLineEdit = new QLineEdit(page);
        finalRampLineEdit->setObjectName("finalRampLineEdit");

        gridLayout_4->addWidget(finalRampLineEdit, 4, 4, 1, 1);

        finalRampCLabel = new QLabel(page);
        finalRampCLabel->setObjectName("finalRampCLabel");

        gridLayout_4->addWidget(finalRampCLabel, 4, 5, 1, 1);

        gridLayout_4->setRowStretch(0, 1);
        gridLayout_4->setRowStretch(1, 1);
        gridLayout_4->setRowStretch(2, 1);
        gridLayout_4->setRowStretch(3, 1);
        gridLayout_4->setRowStretch(4, 1);
        gridLayout_4->setRowStretch(5, 1);
        gridLayout_4->setRowStretch(6, 1);
        gridLayout_4->setRowStretch(7, 1);
        gridLayout_4->setRowStretch(8, 1);
        gridLayout_4->setRowStretch(9, 1);
        gridLayout_4->setColumnStretch(0, 1);
        gridLayout_4->setColumnStretch(1, 1);
        gridLayout_4->setColumnStretch(2, 1);
        gridLayout_4->setColumnStretch(3, 1);
        gridLayout_4->setColumnStretch(4, 1);
        gridLayout_4->setColumnStretch(5, 1);

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
        line_2 = new QFrame(page_3);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_5->addWidget(line_2, 0, 3, 5, 1);

        adv_initTempCLabel = new QLabel(page_3);
        adv_initTempCLabel->setObjectName("adv_initTempCLabel");

        gridLayout_5->addWidget(adv_initTempCLabel, 0, 2, 1, 1);

        adv_numOfCyclesLineEdit = new QLineEdit(page_3);
        adv_numOfCyclesLineEdit->setObjectName("adv_numOfCyclesLineEdit");

        gridLayout_5->addWidget(adv_numOfCyclesLineEdit, 10, 6, 1, 1);

        adv_arcDurationLabel = new QLabel(page_3);
        adv_arcDurationLabel->setObjectName("adv_arcDurationLabel");

        gridLayout_5->addWidget(adv_arcDurationLabel, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 9, 0, 1, 1);

        adv_stepCntLabel = new QLabel(page_3);
        adv_stepCntLabel->setObjectName("adv_stepCntLabel");

        gridLayout_5->addWidget(adv_stepCntLabel, 4, 0, 1, 1);

        adv_finalTempLabel = new QLabel(page_3);
        adv_finalTempLabel->setObjectName("adv_finalTempLabel");

        gridLayout_5->addWidget(adv_finalTempLabel, 0, 4, 1, 1);

        adv_initTempLineEdit = new QLineEdit(page_3);
        adv_initTempLineEdit->setObjectName("adv_initTempLineEdit");

        gridLayout_5->addWidget(adv_initTempLineEdit, 0, 1, 1, 1);

        adv_arcRampLineEdit = new QLineEdit(page_3);
        adv_arcRampLineEdit->setObjectName("adv_arcRampLineEdit");

        gridLayout_5->addWidget(adv_arcRampLineEdit, 3, 5, 1, 1);

        line_3 = new QFrame(page_3);
        line_3->setObjectName("line_3");
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_5->addWidget(line_3, 5, 0, 1, 7);

        adv_polyALabel = new QLabel(page_3);
        adv_polyALabel->setObjectName("adv_polyALabel");
        adv_polyALabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        adv_polyALabel->setWordWrap(true);

        gridLayout_5->addWidget(adv_polyALabel, 6, 0, 1, 2);

        adv_stepCntLieEdit = new QLineEdit(page_3);
        adv_stepCntLieEdit->setObjectName("adv_stepCntLieEdit");

        gridLayout_5->addWidget(adv_stepCntLieEdit, 4, 1, 1, 1);

        adv_finalTempCLabel = new QLabel(page_3);
        adv_finalTempCLabel->setObjectName("adv_finalTempCLabel");

        gridLayout_5->addWidget(adv_finalTempCLabel, 0, 6, 1, 1);

        adv_arcRampLabel = new QLabel(page_3);
        adv_arcRampLabel->setObjectName("adv_arcRampLabel");

        gridLayout_5->addWidget(adv_arcRampLabel, 3, 4, 1, 1);

        adv_polyALineEdit = new QLineEdit(page_3);
        adv_polyALineEdit->setObjectName("adv_polyALineEdit");

        gridLayout_5->addWidget(adv_polyALineEdit, 6, 2, 1, 1);

        adv_finalTempLineEdit = new QLineEdit(page_3);
        adv_finalTempLineEdit->setObjectName("adv_finalTempLineEdit");

        gridLayout_5->addWidget(adv_finalTempLineEdit, 0, 5, 1, 1);

        adv_arcRampCLabel = new QLabel(page_3);
        adv_arcRampCLabel->setObjectName("adv_arcRampCLabel");

        gridLayout_5->addWidget(adv_arcRampCLabel, 3, 6, 1, 1);

        adv_initTempLabel = new QLabel(page_3);
        adv_initTempLabel->setObjectName("adv_initTempLabel");

        gridLayout_5->addWidget(adv_initTempLabel, 0, 0, 1, 1);

        adv_arcDurationComboBox = new QComboBox(page_3);
        adv_arcDurationComboBox->setObjectName("adv_arcDurationComboBox");

        gridLayout_5->addWidget(adv_arcDurationComboBox, 3, 2, 1, 1);

        adv_arcDurationLineEdit = new QLineEdit(page_3);
        adv_arcDurationLineEdit->setObjectName("adv_arcDurationLineEdit");

        gridLayout_5->addWidget(adv_arcDurationLineEdit, 3, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        adv_addPlatueRadioButton = new QRadioButton(page_3);
        adv_addPlatueRadioButton->setObjectName("adv_addPlatueRadioButton");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/icons/platue.png"), QSize(), QIcon::Normal, QIcon::Off);
        adv_addPlatueRadioButton->setIcon(icon5);
        adv_addPlatueRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_addPlatueRadioButton);

        adv_addLinearRadioButton = new QRadioButton(page_3);
        adv_addLinearRadioButton->setObjectName("adv_addLinearRadioButton");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/icons/linears.png"), QSize(), QIcon::Normal, QIcon::Off);
        adv_addLinearRadioButton->setIcon(icon6);
        adv_addLinearRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_addLinearRadioButton);

        adv_addStepsRadioButton = new QRadioButton(page_3);
        adv_addStepsRadioButton->setObjectName("adv_addStepsRadioButton");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/icons/steps.png"), QSize(), QIcon::Normal, QIcon::Off);
        adv_addStepsRadioButton->setIcon(icon7);
        adv_addStepsRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_addStepsRadioButton);

        adv_add2ndDegRadioButton = new QRadioButton(page_3);
        adv_add2ndDegRadioButton->setObjectName("adv_add2ndDegRadioButton");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/icons/secondDegs.png"), QSize(), QIcon::Normal, QIcon::Off);
        adv_add2ndDegRadioButton->setIcon(icon8);
        adv_add2ndDegRadioButton->setIconSize(QSize(64, 64));

        verticalLayout->addWidget(adv_add2ndDegRadioButton);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        adv_addArcToExpButton = new QPushButton(page_3);
        adv_addArcToExpButton->setObjectName("adv_addArcToExpButton");
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resources/icons/plus.svg"), QSize(), QIcon::Normal, QIcon::Off);
        adv_addArcToExpButton->setIcon(icon9);
        adv_addArcToExpButton->setIconSize(QSize(32, 32));
        adv_addArcToExpButton->setAutoDefault(true);

        verticalLayout_2->addWidget(adv_addArcToExpButton);

        adv_delLastArcButton = new QPushButton(page_3);
        adv_delLastArcButton->setObjectName("adv_delLastArcButton");
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resources/icons/minus.svg"), QSize(), QIcon::Normal, QIcon::Off);
        adv_delLastArcButton->setIcon(icon10);

        verticalLayout_2->addWidget(adv_delLastArcButton);

        adv_clearProfileButton = new QPushButton(page_3);
        adv_clearProfileButton->setObjectName("adv_clearProfileButton");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resources/icons/block.svg"), QSize(), QIcon::Normal, QIcon::Off);
        adv_clearProfileButton->setIcon(icon11);

        verticalLayout_2->addWidget(adv_clearProfileButton);


        verticalLayout->addLayout(verticalLayout_2);


        gridLayout_5->addLayout(verticalLayout, 0, 7, 13, 1);

        adv_numOfCyclesLabel = new QLabel(page_3);
        adv_numOfCyclesLabel->setObjectName("adv_numOfCyclesLabel");

        gridLayout_5->addWidget(adv_numOfCyclesLabel, 10, 5, 1, 1);

        adv_polyDescrLabel = new QLabel(page_3);
        adv_polyDescrLabel->setObjectName("adv_polyDescrLabel");
        adv_polyDescrLabel->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(adv_polyDescrLabel, 6, 4, 1, 3);

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
        createPushButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Create", nullptr));
        numOfStepsUpLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of Steps:", nullptr));
        targetTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        targetTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Target Temperature:", nullptr));
        initHoldTimeLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial Holding Time:", nullptr));
        finalHoldTimeLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final holding time:", nullptr));
        initRPMRPMLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "RPM", nullptr));
        stepsUpDurationLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Total Step Up Duration:", nullptr));
        initRPMLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial Stir speed:", nullptr));
        numOfCyclesLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of cycles:", nullptr));
        initTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial temperature:", nullptr));
        initTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        initRampLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial Ramp:", nullptr));
        initRampCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C/min", nullptr));
        finalTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final Temperature:", nullptr));
        finalTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        numOfStepsDownLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of Steps:", nullptr));
        stepsDownDurationLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Total Step Down Duration:", nullptr));
        plateauTimeLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Plateau Time:", nullptr));
        finalRampLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final Ramp:", nullptr));
        finalRampCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C/min", nullptr));
        adv_initTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        adv_arcDurationLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Arc Duration:", nullptr));
        adv_stepCntLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Step Count:", nullptr));
        adv_finalTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Final Temperature:", nullptr));
        adv_polyALabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Polynom Coefficent  a:", nullptr));
        adv_finalTempCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C", nullptr));
        adv_arcRampLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Arc Ramp:", nullptr));
        adv_arcRampCLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "\302\260C/min", nullptr));
        adv_initTempLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Initial temperature:", nullptr));
        adv_addPlatueRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add Platue", nullptr));
        adv_addLinearRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add Linear", nullptr));
        adv_addStepsRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add Steps", nullptr));
        adv_add2ndDegRadioButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add 2nd Deg.", nullptr));
        adv_addArcToExpButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Add", nullptr));
        adv_delLastArcButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Delete Last Arc", nullptr));
        adv_clearProfileButton->setText(QCoreApplication::translate("ExperimentCreateWidget", "Clear Profile", nullptr));
        adv_numOfCyclesLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "Number of Cycles:", nullptr));
        adv_polyDescrLabel->setText(QCoreApplication::translate("ExperimentCreateWidget", "<html><head/><body><p align=\"center\"><span style=\" font-weight:700; color:#bf4040;\">(a.x\302\262 + b.x + c)</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExperimentCreateWidget: public Ui_ExperimentCreateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIEXPERIMENTCREATEWIDGET_H
