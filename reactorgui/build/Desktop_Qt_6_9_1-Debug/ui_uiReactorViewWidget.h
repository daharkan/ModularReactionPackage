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

    void setupUi(QWidget *ReactorViewWidget)
    {
        if (ReactorViewWidget->objectName().isEmpty())
            ReactorViewWidget->setObjectName("ReactorViewWidget");
        ReactorViewWidget->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ReactorViewWidget->sizePolicy().hasHeightForWidth());
        ReactorViewWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ReactorViewWidget);
        gridLayout->setObjectName("gridLayout");
        flowLayout = new QVBoxLayout();
        flowLayout->setObjectName("flowLayout");
        flowLayout->setAlignment(Qt::AlignRight|Qt::AlignTop);
        flowRateLayout = new QHBoxLayout();
        flowRateLayout->setObjectName("flowRateLayout");
        flowRateLabel = new QLabel(ReactorViewWidget);
        flowRateLabel->setObjectName("flowRateLabel");

        flowRateLayout->addWidget(flowRateLabel);

        flowRateValueLabel = new QLabel(ReactorViewWidget);
        flowRateValueLabel->setObjectName("flowRateValueLabel");

        flowRateLayout->addWidget(flowRateValueLabel);


        flowLayout->addLayout(flowRateLayout);

        flowTempLayout = new QHBoxLayout();
        flowTempLayout->setObjectName("flowTempLayout");
        flowTempLabel = new QLabel(ReactorViewWidget);
        flowTempLabel->setObjectName("flowTempLabel");

        flowTempLayout->addWidget(flowTempLabel);

        flowTempValueLabel = new QLabel(ReactorViewWidget);
        flowTempValueLabel->setObjectName("flowTempValueLabel");

        flowTempLayout->addWidget(flowTempValueLabel);


        flowLayout->addLayout(flowTempLayout);


        gridLayout->addLayout(flowLayout, 0, 0, 1, 1);


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
    } // retranslateUi

};

namespace Ui {
    class ReactorViewWidget: public Ui_ReactorViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIREACTORVIEWWIDGET_H
