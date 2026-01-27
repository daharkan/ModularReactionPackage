/********************************************************************************
** Form generated from reading UI file 'uiExperimentManagerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIEXPERIMENTMANAGERWIDGET_H
#define UI_UIEXPERIMENTMANAGERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExperimentManagerWidget
{
public:
    QGridLayout *gridLayout;
    QGridLayout *mainGridLayout;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *listExperimentsPushButton;
    QSpacerItem *verticalSpacer;
    QPushButton *createPushButton;
    QPushButton *edtExperimentPushButton;
    QPushButton *delExperimentPushButton;

    void setupUi(QWidget *ExperimentManagerWidget)
    {
        if (ExperimentManagerWidget->objectName().isEmpty())
            ExperimentManagerWidget->setObjectName("ExperimentManagerWidget");
        ExperimentManagerWidget->resize(1375, 628);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ExperimentManagerWidget->sizePolicy().hasHeightForWidth());
        ExperimentManagerWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ExperimentManagerWidget);
        gridLayout->setObjectName("gridLayout");
        mainGridLayout = new QGridLayout();
        mainGridLayout->setObjectName("mainGridLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        mainGridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);


        gridLayout->addLayout(mainGridLayout, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, -1, 0, -1);
        listExperimentsPushButton = new QPushButton(ExperimentManagerWidget);
        listExperimentsPushButton->setObjectName("listExperimentsPushButton");

        verticalLayout->addWidget(listExperimentsPushButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        createPushButton = new QPushButton(ExperimentManagerWidget);
        createPushButton->setObjectName("createPushButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(createPushButton->sizePolicy().hasHeightForWidth());
        createPushButton->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(createPushButton);

        edtExperimentPushButton = new QPushButton(ExperimentManagerWidget);
        edtExperimentPushButton->setObjectName("edtExperimentPushButton");

        verticalLayout->addWidget(edtExperimentPushButton);

        delExperimentPushButton = new QPushButton(ExperimentManagerWidget);
        delExperimentPushButton->setObjectName("delExperimentPushButton");

        verticalLayout->addWidget(delExperimentPushButton);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);


        retranslateUi(ExperimentManagerWidget);

        QMetaObject::connectSlotsByName(ExperimentManagerWidget);
    } // setupUi

    void retranslateUi(QWidget *ExperimentManagerWidget)
    {
        ExperimentManagerWidget->setWindowTitle(QCoreApplication::translate("ExperimentManagerWidget", "Form", nullptr));
        listExperimentsPushButton->setText(QCoreApplication::translate("ExperimentManagerWidget", "Experiment List", nullptr));
        createPushButton->setText(QCoreApplication::translate("ExperimentManagerWidget", "Create Experiment", nullptr));
        edtExperimentPushButton->setText(QCoreApplication::translate("ExperimentManagerWidget", "Edit Experiment", nullptr));
        delExperimentPushButton->setText(QCoreApplication::translate("ExperimentManagerWidget", "Delete Experiment", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExperimentManagerWidget: public Ui_ExperimentManagerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIEXPERIMENTMANAGERWIDGET_H
