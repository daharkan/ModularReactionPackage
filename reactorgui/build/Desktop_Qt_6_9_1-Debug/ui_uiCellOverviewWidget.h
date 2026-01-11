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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellOverviewWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *slotLabel;
    QHBoxLayout *experimentLayout;
    QLabel *experimentNameLabel;
    QLabel *experimentNameValueLabel;
    QHBoxLayout *assignedByLayout;
    QLabel *assignedByLabel;
    QLabel *assignedByValueLabel;
    QHBoxLayout *assignedAtLayout;
    QLabel *assignedAtLabel;
    QLabel *assignedAtValueLabel;
    QHBoxLayout *rpmTempLayout;
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
        slotLabel = new QLabel(CellOverviewWidget);
        slotLabel->setObjectName("slotLabel");
        QFont font;
        font.setBold(true);
        slotLabel->setFont(font);

        verticalLayout->addWidget(slotLabel);

        experimentLayout = new QHBoxLayout();
        experimentLayout->setObjectName("experimentLayout");
        experimentNameLabel = new QLabel(CellOverviewWidget);
        experimentNameLabel->setObjectName("experimentNameLabel");

        experimentLayout->addWidget(experimentNameLabel);

        experimentNameValueLabel = new QLabel(CellOverviewWidget);
        experimentNameValueLabel->setObjectName("experimentNameValueLabel");

        experimentLayout->addWidget(experimentNameValueLabel);


        verticalLayout->addLayout(experimentLayout);

        assignedByLayout = new QHBoxLayout();
        assignedByLayout->setObjectName("assignedByLayout");
        assignedByLabel = new QLabel(CellOverviewWidget);
        assignedByLabel->setObjectName("assignedByLabel");

        assignedByLayout->addWidget(assignedByLabel);

        assignedByValueLabel = new QLabel(CellOverviewWidget);
        assignedByValueLabel->setObjectName("assignedByValueLabel");

        assignedByLayout->addWidget(assignedByValueLabel);


        verticalLayout->addLayout(assignedByLayout);

        assignedAtLayout = new QHBoxLayout();
        assignedAtLayout->setObjectName("assignedAtLayout");
        assignedAtLabel = new QLabel(CellOverviewWidget);
        assignedAtLabel->setObjectName("assignedAtLabel");

        assignedAtLayout->addWidget(assignedAtLabel);

        assignedAtValueLabel = new QLabel(CellOverviewWidget);
        assignedAtValueLabel->setObjectName("assignedAtValueLabel");

        assignedAtLayout->addWidget(assignedAtValueLabel);


        verticalLayout->addLayout(assignedAtLayout);

        rpmTempLayout = new QHBoxLayout();
        rpmTempLayout->setObjectName("rpmTempLayout");
        rpmLabel = new QLabel(CellOverviewWidget);
        rpmLabel->setObjectName("rpmLabel");

        rpmTempLayout->addWidget(rpmLabel);

        rpmValueLabel = new QLabel(CellOverviewWidget);
        rpmValueLabel->setObjectName("rpmValueLabel");

        rpmTempLayout->addWidget(rpmValueLabel);

        tempLabel = new QLabel(CellOverviewWidget);
        tempLabel->setObjectName("tempLabel");

        rpmTempLayout->addWidget(tempLabel);

        tempValueLabel = new QLabel(CellOverviewWidget);
        tempValueLabel->setObjectName("tempValueLabel");

        rpmTempLayout->addWidget(tempValueLabel);


        verticalLayout->addLayout(rpmTempLayout);


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
