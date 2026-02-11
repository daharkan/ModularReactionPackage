/********************************************************************************
** Form generated from reading UI file 'uiCellOverviewWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICELLOVERVIEWWIDGET_H
#define UI_UICELLOVERVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellOverviewWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *headerLayout;
    QLabel *slotLabel;
    QSpacerItem *headerSpacer;
    QLabel *stateLabel;
    QGridLayout *detailsLayout;
    QLabel *experimentNameLabel;
    QLabel *experimentNameValueLabel;
    QLabel *assignedByLabel;
    QLabel *assignedByValueLabel;
    QLabel *assignedAtLabel;
    QLabel *assignedAtValueLabel;
    QLabel *finishesAtLabel;
    QLabel *finishesAtValueLabel;
    QGridLayout *metricsLayout;
    QLabel *rpmLabel;
    QLabel *rpmValueLabel;
    QLabel *tempLabel;
    QLabel *tempValueLabel;
    QLabel *heaterDutyLabel;
    QLabel *heaterDutyValueLabel;
    QLabel *peltierDutyLabel;
    QLabel *peltierDutyValueLabel;

    void setupUi(QWidget *CellOverviewWidget)
    {
        if (CellOverviewWidget->objectName().isEmpty())
            CellOverviewWidget->setObjectName("CellOverviewWidget");
        CellOverviewWidget->resize(485, 253);
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CellOverviewWidget->sizePolicy().hasHeightForWidth());
        CellOverviewWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(CellOverviewWidget);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(8, 8, 8, 8);
        headerLayout = new QHBoxLayout();
        headerLayout->setSpacing(6);
        headerLayout->setObjectName("headerLayout");
        slotLabel = new QLabel(CellOverviewWidget);
        slotLabel->setObjectName("slotLabel");
        QFont font;
        font.setBold(true);
        slotLabel->setFont(font);

        headerLayout->addWidget(slotLabel);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        stateLabel = new QLabel(CellOverviewWidget);
        stateLabel->setObjectName("stateLabel");
        stateLabel->setFont(font);
        stateLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        headerLayout->addWidget(stateLabel);


        verticalLayout->addLayout(headerLayout);

        detailsLayout = new QGridLayout();
        detailsLayout->setObjectName("detailsLayout");
        detailsLayout->setHorizontalSpacing(6);
        detailsLayout->setVerticalSpacing(2);
        experimentNameLabel = new QLabel(CellOverviewWidget);
        experimentNameLabel->setObjectName("experimentNameLabel");
        experimentNameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        detailsLayout->addWidget(experimentNameLabel, 0, 0, 1, 1);

        experimentNameValueLabel = new QLabel(CellOverviewWidget);
        experimentNameValueLabel->setObjectName("experimentNameValueLabel");

        detailsLayout->addWidget(experimentNameValueLabel, 0, 1, 1, 1);

        assignedByLabel = new QLabel(CellOverviewWidget);
        assignedByLabel->setObjectName("assignedByLabel");
        assignedByLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        detailsLayout->addWidget(assignedByLabel, 1, 0, 1, 1);

        assignedByValueLabel = new QLabel(CellOverviewWidget);
        assignedByValueLabel->setObjectName("assignedByValueLabel");

        detailsLayout->addWidget(assignedByValueLabel, 1, 1, 1, 1);

        assignedAtLabel = new QLabel(CellOverviewWidget);
        assignedAtLabel->setObjectName("assignedAtLabel");
        assignedAtLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        detailsLayout->addWidget(assignedAtLabel, 2, 0, 1, 1);

        assignedAtValueLabel = new QLabel(CellOverviewWidget);
        assignedAtValueLabel->setObjectName("assignedAtValueLabel");

        detailsLayout->addWidget(assignedAtValueLabel, 2, 1, 1, 1);

        finishesAtLabel = new QLabel(CellOverviewWidget);
        finishesAtLabel->setObjectName("finishesAtLabel");
        finishesAtLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        detailsLayout->addWidget(finishesAtLabel, 3, 0, 1, 1);

        finishesAtValueLabel = new QLabel(CellOverviewWidget);
        finishesAtValueLabel->setObjectName("finishesAtValueLabel");

        detailsLayout->addWidget(finishesAtValueLabel, 3, 1, 1, 1);


        verticalLayout->addLayout(detailsLayout);

        metricsLayout = new QGridLayout();
        metricsLayout->setObjectName("metricsLayout");
        metricsLayout->setHorizontalSpacing(10);
        metricsLayout->setVerticalSpacing(2);
        rpmLabel = new QLabel(CellOverviewWidget);
        rpmLabel->setObjectName("rpmLabel");
        rpmLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        metricsLayout->addWidget(rpmLabel, 0, 0, 1, 1);

        rpmValueLabel = new QLabel(CellOverviewWidget);
        rpmValueLabel->setObjectName("rpmValueLabel");

        metricsLayout->addWidget(rpmValueLabel, 0, 1, 1, 1);

        tempLabel = new QLabel(CellOverviewWidget);
        tempLabel->setObjectName("tempLabel");
        tempLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        metricsLayout->addWidget(tempLabel, 0, 2, 1, 1);

        tempValueLabel = new QLabel(CellOverviewWidget);
        tempValueLabel->setObjectName("tempValueLabel");

        metricsLayout->addWidget(tempValueLabel, 0, 3, 1, 1);

        heaterDutyLabel = new QLabel(CellOverviewWidget);
        heaterDutyLabel->setObjectName("heaterDutyLabel");
        heaterDutyLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        metricsLayout->addWidget(heaterDutyLabel, 1, 0, 1, 1);

        heaterDutyValueLabel = new QLabel(CellOverviewWidget);
        heaterDutyValueLabel->setObjectName("heaterDutyValueLabel");

        metricsLayout->addWidget(heaterDutyValueLabel, 1, 1, 1, 1);

        peltierDutyLabel = new QLabel(CellOverviewWidget);
        peltierDutyLabel->setObjectName("peltierDutyLabel");
        peltierDutyLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        metricsLayout->addWidget(peltierDutyLabel, 1, 2, 1, 1);

        peltierDutyValueLabel = new QLabel(CellOverviewWidget);
        peltierDutyValueLabel->setObjectName("peltierDutyValueLabel");

        metricsLayout->addWidget(peltierDutyValueLabel, 1, 3, 1, 1);


        verticalLayout->addLayout(metricsLayout);


        retranslateUi(CellOverviewWidget);

        QMetaObject::connectSlotsByName(CellOverviewWidget);
    } // setupUi

    void retranslateUi(QWidget *CellOverviewWidget)
    {
        CellOverviewWidget->setWindowTitle(QCoreApplication::translate("CellOverviewWidget", "Form", nullptr));
        slotLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Cell 1", nullptr));
        stateLabel->setText(QString());
        experimentNameLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Experiment:", nullptr));
        experimentNameValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        assignedByLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Assigned by:", nullptr));
        assignedByValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        assignedAtLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Assigned at:", nullptr));
        assignedAtValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        finishesAtLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Finishes at:", nullptr));
        finishesAtValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        rpmLabel->setText(QCoreApplication::translate("CellOverviewWidget", "RPM:", nullptr));
        rpmValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        tempLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Temp:", nullptr));
        tempValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        heaterDutyLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Heater %:", nullptr));
        heaterDutyValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
        peltierDutyLabel->setText(QCoreApplication::translate("CellOverviewWidget", "Peltier %:", nullptr));
        peltierDutyValueLabel->setText(QCoreApplication::translate("CellOverviewWidget", "--", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CellOverviewWidget: public Ui_CellOverviewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICELLOVERVIEWWIDGET_H
