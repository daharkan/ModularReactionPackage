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
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellWidget
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QFrame *line_2;
    QGridLayout *graphLayput;
    QGridLayout *gridLayout;
    QLabel *label_11;
    QLabel *label_5;
    QFrame *line;
    QLabel *currentInnerTempLabel;
    QLabel *currentExtTempLabel;
    QLabel *motorAmpLabel;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer;
    QLabel *targetRPMLabel;
    QLabel *label;
    QLabel *targetTempLabel;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *currentRPMLabel;
    QLabel *label_4;
    QLabel *expStateLabel;

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

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_11 = new QLabel(CellWidget);
        label_11->setObjectName("label_11");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_11, 3, 5, 1, 1);

        label_5 = new QLabel(CellWidget);
        label_5->setObjectName("label_5");
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_5, 1, 5, 1, 1);

        line = new QFrame(CellWidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 1, 4, 4, 1);

        currentInnerTempLabel = new QLabel(CellWidget);
        currentInnerTempLabel->setObjectName("currentInnerTempLabel");

        gridLayout->addWidget(currentInnerTempLabel, 2, 3, 1, 1);

        currentExtTempLabel = new QLabel(CellWidget);
        currentExtTempLabel->setObjectName("currentExtTempLabel");
        sizePolicy.setHeightForWidth(currentExtTempLabel->sizePolicy().hasHeightForWidth());
        currentExtTempLabel->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        currentExtTempLabel->setFont(font);

        gridLayout->addWidget(currentExtTempLabel, 1, 3, 1, 1);

        motorAmpLabel = new QLabel(CellWidget);
        motorAmpLabel->setObjectName("motorAmpLabel");
        sizePolicy.setHeightForWidth(motorAmpLabel->sizePolicy().hasHeightForWidth());
        motorAmpLabel->setSizePolicy(sizePolicy);
        motorAmpLabel->setFont(font);

        gridLayout->addWidget(motorAmpLabel, 3, 6, 1, 1);

        label_8 = new QLabel(CellWidget);
        label_8->setObjectName("label_8");
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_8, 2, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        targetRPMLabel = new QLabel(CellWidget);
        targetRPMLabel->setObjectName("targetRPMLabel");
        sizePolicy.setHeightForWidth(targetRPMLabel->sizePolicy().hasHeightForWidth());
        targetRPMLabel->setSizePolicy(sizePolicy);
        targetRPMLabel->setFont(font);

        gridLayout->addWidget(targetRPMLabel, 2, 6, 1, 1);

        label = new QLabel(CellWidget);
        label->setObjectName("label");
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 3, 2, 1, 1);

        targetTempLabel = new QLabel(CellWidget);
        targetTempLabel->setObjectName("targetTempLabel");
        sizePolicy.setHeightForWidth(targetTempLabel->sizePolicy().hasHeightForWidth());
        targetTempLabel->setSizePolicy(sizePolicy);
        targetTempLabel->setFont(font);

        gridLayout->addWidget(targetTempLabel, 3, 3, 1, 1);

        label_7 = new QLabel(CellWidget);
        label_7->setObjectName("label_7");
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 0, 2, 1, 5);

        label_3 = new QLabel(CellWidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 2, 1, 1);

        label_2 = new QLabel(CellWidget);
        label_2->setObjectName("label_2");
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 1, 2, 1, 1);

        currentRPMLabel = new QLabel(CellWidget);
        currentRPMLabel->setObjectName("currentRPMLabel");
        sizePolicy.setHeightForWidth(currentRPMLabel->sizePolicy().hasHeightForWidth());
        currentRPMLabel->setSizePolicy(sizePolicy);
        currentRPMLabel->setFont(font);

        gridLayout->addWidget(currentRPMLabel, 1, 6, 1, 1);

        label_4 = new QLabel(CellWidget);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        expStateLabel = new QLabel(CellWidget);
        expStateLabel->setObjectName("expStateLabel");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        expStateLabel->setFont(font1);
        expStateLabel->setStyleSheet(QString::fromUtf8("color: rgb(26, 95, 180);"));
        expStateLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(expStateLabel, 1, 0, 3, 1);


        gridLayout_2->addLayout(gridLayout, 1, 1, 1, 2);


        retranslateUi(CellWidget);

        QMetaObject::connectSlotsByName(CellWidget);
    } // setupUi

    void retranslateUi(QWidget *CellWidget)
    {
        CellWidget->setWindowTitle(QCoreApplication::translate("CellWidget", "Form", nullptr));
        label_11->setText(QCoreApplication::translate("CellWidget", "Motor Amp.:", nullptr));
        label_5->setText(QCoreApplication::translate("CellWidget", "Current RPM:", nullptr));
        currentInnerTempLabel->setText(QString());
        currentExtTempLabel->setText(QString());
        motorAmpLabel->setText(QString());
        label_8->setText(QCoreApplication::translate("CellWidget", "Target RPM:", nullptr));
        targetRPMLabel->setText(QString());
        label->setText(QCoreApplication::translate("CellWidget", "Target Temperature:", nullptr));
        targetTempLabel->setText(QString());
        label_7->setText(QCoreApplication::translate("CellWidget", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700; color:#a51d2d;\">Cell Status</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("CellWidget", "Current InnerTemperature:", nullptr));
        label_2->setText(QCoreApplication::translate("CellWidget", "Current Ext. Temperature:", nullptr));
        currentRPMLabel->setText(QString());
        label_4->setText(QCoreApplication::translate("CellWidget", "Experiment State", nullptr));
        expStateLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CellWidget: public Ui_CellWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICELLWIDGET_H
