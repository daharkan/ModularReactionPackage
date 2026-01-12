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
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellOverviewWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *slotLabel;
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
        slotLabel = new QLabel(CellOverviewWidget);
        slotLabel->setObjectName("slotLabel");
        QFont font;
        font.setBold(true);
        slotLabel->setFont(font);

        verticalLayout->addWidget(slotLabel);

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


        verticalLayout->addLayout(metricsLayout);


        retranslateUi(CellOverviewWidget);

        QMetaObject::connectSlotsByName(CellOverviewWidget);
    } // setupUi

    void retranslateUi(QWidget *CellOverviewWidget)
    {
        CellOverviewWidget->setWindowTitle(QCoreApplication::translate("CellOverviewWidget", "Form", nullptr));
        slotLabel->setText(QCoreApplication::translate("CellOverviewWidget", "LHS-1", nullptr));
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
    } // retranslateUi

};

namespace Ui {
    class CellOverviewWidget: public Ui_CellOverviewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICELLOVERVIEWWIDGET_H
