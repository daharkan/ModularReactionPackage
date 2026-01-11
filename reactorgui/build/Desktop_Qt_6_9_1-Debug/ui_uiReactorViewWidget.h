/********************************************************************************
** Form generated from reading UI file 'uiReactorViewWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIREACTORVIEWWIDGET_H
#define UI_UIREACTORVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReactorViewWidget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *flowLayout;
    QHBoxLayout *flowRateLayout;
    QLabel *flowRateLabel;
    QLabel *flowRateValueLabel;
    QHBoxLayout *flowTempLayout;
    QLabel *flowTempLabel;
    QLabel *flowTempValueLabel;
    QHBoxLayout *cellsLayout;
    QGroupBox *lhsGroupBox;
    QVBoxLayout *lhsCellsLayout;
    QGroupBox *rhsGroupBox;
    QVBoxLayout *rhsCellsLayout;

    void setupUi(QWidget *ReactorViewWidget)
    {
        if (ReactorViewWidget->objectName().isEmpty())
            ReactorViewWidget->setObjectName("ReactorViewWidget");
        ReactorViewWidget->resize(739, 395);
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ReactorViewWidget->sizePolicy().hasHeightForWidth());
        ReactorViewWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ReactorViewWidget);
        gridLayout->setObjectName("gridLayout");
        flowLayout = new QVBoxLayout();
        flowLayout->setObjectName("flowLayout");
        flowRateLayout = new QHBoxLayout();
        flowRateLayout->setObjectName("flowRateLayout");
        flowRateLabel = new QLabel(ReactorViewWidget);
        flowRateLabel->setObjectName("flowRateLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(flowRateLabel->sizePolicy().hasHeightForWidth());
        flowRateLabel->setSizePolicy(sizePolicy1);

        flowRateLayout->addWidget(flowRateLabel);

        flowRateValueLabel = new QLabel(ReactorViewWidget);
        flowRateValueLabel->setObjectName("flowRateValueLabel");
        sizePolicy1.setHeightForWidth(flowRateValueLabel->sizePolicy().hasHeightForWidth());
        flowRateValueLabel->setSizePolicy(sizePolicy1);

        flowRateLayout->addWidget(flowRateValueLabel);


        flowLayout->addLayout(flowRateLayout);

        flowTempLayout = new QHBoxLayout();
        flowTempLayout->setObjectName("flowTempLayout");
        flowTempLabel = new QLabel(ReactorViewWidget);
        flowTempLabel->setObjectName("flowTempLabel");
        sizePolicy1.setHeightForWidth(flowTempLabel->sizePolicy().hasHeightForWidth());
        flowTempLabel->setSizePolicy(sizePolicy1);

        flowTempLayout->addWidget(flowTempLabel);

        flowTempValueLabel = new QLabel(ReactorViewWidget);
        flowTempValueLabel->setObjectName("flowTempValueLabel");
        sizePolicy1.setHeightForWidth(flowTempValueLabel->sizePolicy().hasHeightForWidth());
        flowTempValueLabel->setSizePolicy(sizePolicy1);

        flowTempLayout->addWidget(flowTempValueLabel);


        flowLayout->addLayout(flowTempLayout);


        gridLayout->addLayout(flowLayout, 0, 0, 1, 1);

        cellsLayout = new QHBoxLayout();
        cellsLayout->setObjectName("cellsLayout");
        lhsGroupBox = new QGroupBox(ReactorViewWidget);
        lhsGroupBox->setObjectName("lhsGroupBox");
        sizePolicy1.setHeightForWidth(lhsGroupBox->sizePolicy().hasHeightForWidth());
        lhsGroupBox->setSizePolicy(sizePolicy1);
        lhsCellsLayout = new QVBoxLayout(lhsGroupBox);
        lhsCellsLayout->setObjectName("lhsCellsLayout");

        cellsLayout->addWidget(lhsGroupBox);

        rhsGroupBox = new QGroupBox(ReactorViewWidget);
        rhsGroupBox->setObjectName("rhsGroupBox");
        sizePolicy1.setHeightForWidth(rhsGroupBox->sizePolicy().hasHeightForWidth());
        rhsGroupBox->setSizePolicy(sizePolicy1);
        rhsCellsLayout = new QVBoxLayout(rhsGroupBox);
        rhsCellsLayout->setObjectName("rhsCellsLayout");

        cellsLayout->addWidget(rhsGroupBox);


        gridLayout->addLayout(cellsLayout, 1, 0, 1, 1);


        retranslateUi(ReactorViewWidget);

        QMetaObject::connectSlotsByName(ReactorViewWidget);
    } // setupUi

    void retranslateUi(QWidget *ReactorViewWidget)
    {
        ReactorViewWidget->setWindowTitle(QCoreApplication::translate("ReactorViewWidget", "Form", nullptr));
        flowRateLabel->setText(QCoreApplication::translate("ReactorViewWidget", "Flow (L/min):", nullptr));
        flowRateValueLabel->setText(QCoreApplication::translate("ReactorViewWidget", "--", nullptr));
        flowTempLabel->setText(QCoreApplication::translate("ReactorViewWidget", "Flow Temp (mV):", nullptr));
        flowTempValueLabel->setText(QCoreApplication::translate("ReactorViewWidget", "--", nullptr));
        lhsGroupBox->setTitle(QCoreApplication::translate("ReactorViewWidget", "LHS Cells", nullptr));
        rhsGroupBox->setTitle(QCoreApplication::translate("ReactorViewWidget", "RHS Cells", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReactorViewWidget: public Ui_ReactorViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIREACTORVIEWWIDGET_H
