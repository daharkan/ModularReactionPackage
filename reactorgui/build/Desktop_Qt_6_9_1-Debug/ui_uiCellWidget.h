/********************************************************************************
** Form generated from reading UI file 'uiCellWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICELLWIDGET_H
#define UI_UICELLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellWidget
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QFrame *line_2;
    QGridLayout *graphLayput;
    QHBoxLayout *gridLayout;
    QGroupBox *stateGroupBox;
    QVBoxLayout *stateLayout;
    QLabel *expStateLabel;
    QGroupBox *experimentInfoGroupBox;
    QGridLayout *experimentInfoLayout;
    QLabel *experimentNameLabel;
    QLabel *experimentNameValueLabel;
    QLabel *assignedByLabel;
    QLabel *assignedByValueLabel;
    QLabel *assignedAtLabel;
    QLabel *assignedAtValueLabel;
    QLabel *experimentProgressLabel;
    QLabel *experimentProgressValueLabel;
    QPushButton *assignExperimentButton;
    QGroupBox *metricsGroupBox;
    QGridLayout *metricsLayout;
    QLabel *label_2;
    QLabel *currentExtTempLabel;
    QLabel *label_3;
    QLabel *currentInnerTempLabel;
    QLabel *label;
    QLabel *targetTempLabel;
    QLabel *label_5;
    QLabel *currentRPMLabel;
    QLabel *label_8;
    QLabel *targetRPMLabel;
    QLabel *label_11;
    QLabel *motorAmpLabel;

    void setupUi(QWidget *CellWidget)
    {
        if (CellWidget->objectName().isEmpty())
            CellWidget->setObjectName("CellWidget");
        CellWidget->resize(965, 578);
        gridLayout_2 = new QGridLayout(CellWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 4, 1, 1, 1);

        line_2 = new QFrame(CellWidget);
        line_2->setObjectName("line_2");
        line_2->setLineWidth(3);
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_2->addWidget(line_2, 0, 1, 1, 2);

        graphLayput = new QGridLayout();
        graphLayput->setObjectName("graphLayput");

        gridLayout_2->addLayout(graphLayput, 2, 1, 1, 2);

        gridLayout = new QHBoxLayout();
        gridLayout->setSpacing(12);
        gridLayout->setObjectName("gridLayout");
        stateGroupBox = new QGroupBox(CellWidget);
        stateGroupBox->setObjectName("stateGroupBox");
        stateLayout = new QVBoxLayout(stateGroupBox);
        stateLayout->setObjectName("stateLayout");
        expStateLabel = new QLabel(stateGroupBox);
        expStateLabel->setObjectName("expStateLabel");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        expStateLabel->setFont(font);
        expStateLabel->setStyleSheet(QString::fromUtf8("color: #213268;"));
        expStateLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        stateLayout->addWidget(expStateLabel);


        gridLayout->addWidget(stateGroupBox);

        experimentInfoGroupBox = new QGroupBox(CellWidget);
        experimentInfoGroupBox->setObjectName("experimentInfoGroupBox");
        experimentInfoLayout = new QGridLayout(experimentInfoGroupBox);
        experimentInfoLayout->setObjectName("experimentInfoLayout");
        experimentNameLabel = new QLabel(experimentInfoGroupBox);
        experimentNameLabel->setObjectName("experimentNameLabel");

        experimentInfoLayout->addWidget(experimentNameLabel, 0, 0, 1, 1);

        experimentNameValueLabel = new QLabel(experimentInfoGroupBox);
        experimentNameValueLabel->setObjectName("experimentNameValueLabel");
        QFont font1;
        font1.setBold(true);
        experimentNameValueLabel->setFont(font1);

        experimentInfoLayout->addWidget(experimentNameValueLabel, 0, 1, 1, 1);

        assignedByLabel = new QLabel(experimentInfoGroupBox);
        assignedByLabel->setObjectName("assignedByLabel");

        experimentInfoLayout->addWidget(assignedByLabel, 1, 0, 1, 1);

        assignedByValueLabel = new QLabel(experimentInfoGroupBox);
        assignedByValueLabel->setObjectName("assignedByValueLabel");
        assignedByValueLabel->setFont(font1);

        experimentInfoLayout->addWidget(assignedByValueLabel, 1, 1, 1, 1);

        assignedAtLabel = new QLabel(experimentInfoGroupBox);
        assignedAtLabel->setObjectName("assignedAtLabel");

        experimentInfoLayout->addWidget(assignedAtLabel, 2, 0, 1, 1);

        assignedAtValueLabel = new QLabel(experimentInfoGroupBox);
        assignedAtValueLabel->setObjectName("assignedAtValueLabel");

        experimentInfoLayout->addWidget(assignedAtValueLabel, 2, 1, 1, 1);

        experimentProgressLabel = new QLabel(experimentInfoGroupBox);
        experimentProgressLabel->setObjectName("experimentProgressLabel");

        experimentInfoLayout->addWidget(experimentProgressLabel, 3, 0, 1, 1);

        experimentProgressValueLabel = new QLabel(experimentInfoGroupBox);
        experimentProgressValueLabel->setObjectName("experimentProgressValueLabel");

        experimentInfoLayout->addWidget(experimentProgressValueLabel, 3, 1, 1, 1);

        assignExperimentButton = new QPushButton(experimentInfoGroupBox);
        assignExperimentButton->setObjectName("assignExperimentButton");
        assignExperimentButton->setMinimumSize(QSize(0, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/plus.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        assignExperimentButton->setIcon(icon);
        assignExperimentButton->setIconSize(QSize(22, 22));

        experimentInfoLayout->addWidget(assignExperimentButton, 4, 0, 1, 2);


        gridLayout->addWidget(experimentInfoGroupBox);

        metricsGroupBox = new QGroupBox(CellWidget);
        metricsGroupBox->setObjectName("metricsGroupBox");
        metricsLayout = new QGridLayout(metricsGroupBox);
        metricsLayout->setObjectName("metricsLayout");
        label_2 = new QLabel(metricsGroupBox);
        label_2->setObjectName("label_2");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        metricsLayout->addWidget(label_2, 0, 0, 1, 1);

        currentExtTempLabel = new QLabel(metricsGroupBox);
        currentExtTempLabel->setObjectName("currentExtTempLabel");
        sizePolicy.setHeightForWidth(currentExtTempLabel->sizePolicy().hasHeightForWidth());
        currentExtTempLabel->setSizePolicy(sizePolicy);
        currentExtTempLabel->setFont(font1);

        metricsLayout->addWidget(currentExtTempLabel, 0, 1, 1, 1);

        label_3 = new QLabel(metricsGroupBox);
        label_3->setObjectName("label_3");

        metricsLayout->addWidget(label_3, 1, 0, 1, 1);

        currentInnerTempLabel = new QLabel(metricsGroupBox);
        currentInnerTempLabel->setObjectName("currentInnerTempLabel");
        currentInnerTempLabel->setFont(font1);

        metricsLayout->addWidget(currentInnerTempLabel, 1, 1, 1, 1);

        label = new QLabel(metricsGroupBox);
        label->setObjectName("label");
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        metricsLayout->addWidget(label, 2, 0, 1, 1);

        targetTempLabel = new QLabel(metricsGroupBox);
        targetTempLabel->setObjectName("targetTempLabel");
        sizePolicy.setHeightForWidth(targetTempLabel->sizePolicy().hasHeightForWidth());
        targetTempLabel->setSizePolicy(sizePolicy);
        targetTempLabel->setFont(font1);

        metricsLayout->addWidget(targetTempLabel, 2, 1, 1, 1);

        label_5 = new QLabel(metricsGroupBox);
        label_5->setObjectName("label_5");
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        metricsLayout->addWidget(label_5, 3, 0, 1, 1);

        currentRPMLabel = new QLabel(metricsGroupBox);
        currentRPMLabel->setObjectName("currentRPMLabel");
        sizePolicy.setHeightForWidth(currentRPMLabel->sizePolicy().hasHeightForWidth());
        currentRPMLabel->setSizePolicy(sizePolicy);
        currentRPMLabel->setFont(font1);

        metricsLayout->addWidget(currentRPMLabel, 3, 1, 1, 1);

        label_8 = new QLabel(metricsGroupBox);
        label_8->setObjectName("label_8");
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        metricsLayout->addWidget(label_8, 4, 0, 1, 1);

        targetRPMLabel = new QLabel(metricsGroupBox);
        targetRPMLabel->setObjectName("targetRPMLabel");
        sizePolicy.setHeightForWidth(targetRPMLabel->sizePolicy().hasHeightForWidth());
        targetRPMLabel->setSizePolicy(sizePolicy);
        targetRPMLabel->setFont(font1);

        metricsLayout->addWidget(targetRPMLabel, 4, 1, 1, 1);

        label_11 = new QLabel(metricsGroupBox);
        label_11->setObjectName("label_11");
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);

        metricsLayout->addWidget(label_11, 5, 0, 1, 1);

        motorAmpLabel = new QLabel(metricsGroupBox);
        motorAmpLabel->setObjectName("motorAmpLabel");
        sizePolicy.setHeightForWidth(motorAmpLabel->sizePolicy().hasHeightForWidth());
        motorAmpLabel->setSizePolicy(sizePolicy);
        motorAmpLabel->setFont(font1);

        metricsLayout->addWidget(motorAmpLabel, 5, 1, 1, 1);


        gridLayout->addWidget(metricsGroupBox);


        gridLayout_2->addLayout(gridLayout, 1, 1, 1, 2);


        retranslateUi(CellWidget);

        QMetaObject::connectSlotsByName(CellWidget);
    } // setupUi

    void retranslateUi(QWidget *CellWidget)
    {
        CellWidget->setWindowTitle(QCoreApplication::translate("CellWidget", "Form", nullptr));
        stateGroupBox->setTitle(QCoreApplication::translate("CellWidget", "Experiment State", nullptr));
        expStateLabel->setText(QString());
        experimentInfoGroupBox->setTitle(QCoreApplication::translate("CellWidget", "Experiment Info", nullptr));
        experimentNameLabel->setText(QCoreApplication::translate("CellWidget", "Experiment:", nullptr));
        experimentNameValueLabel->setText(QCoreApplication::translate("CellWidget", "--", nullptr));
        assignedByLabel->setText(QCoreApplication::translate("CellWidget", "Assigned By:", nullptr));
        assignedByValueLabel->setText(QCoreApplication::translate("CellWidget", "--", nullptr));
        assignedAtLabel->setText(QCoreApplication::translate("CellWidget", "Assigned At:", nullptr));
        assignedAtValueLabel->setText(QCoreApplication::translate("CellWidget", "--", nullptr));
        experimentProgressLabel->setText(QCoreApplication::translate("CellWidget", "Progress:", nullptr));
        experimentProgressValueLabel->setText(QCoreApplication::translate("CellWidget", "--", nullptr));
        assignExperimentButton->setText(QCoreApplication::translate("CellWidget", "Assign Experiment", nullptr));
        metricsGroupBox->setTitle(QCoreApplication::translate("CellWidget", "Live Readings", nullptr));
        label_2->setText(QCoreApplication::translate("CellWidget", "Current Ext. Temperature:", nullptr));
        currentExtTempLabel->setText(QString());
        label_3->setText(QCoreApplication::translate("CellWidget", "Current Inner Temperature:", nullptr));
        currentInnerTempLabel->setText(QString());
        label->setText(QCoreApplication::translate("CellWidget", "Target Temperature:", nullptr));
        targetTempLabel->setText(QString());
        label_5->setText(QCoreApplication::translate("CellWidget", "Current RPM:", nullptr));
        currentRPMLabel->setText(QString());
        label_8->setText(QCoreApplication::translate("CellWidget", "Target RPM:", nullptr));
        targetRPMLabel->setText(QString());
        label_11->setText(QCoreApplication::translate("CellWidget", "Motor Amp.:", nullptr));
        motorAmpLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CellWidget: public Ui_CellWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICELLWIDGET_H
