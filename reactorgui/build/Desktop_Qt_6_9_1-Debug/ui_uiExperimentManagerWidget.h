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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExperimentManagerWidget
{
public:
    QGridLayout *gridLayout;
    QGridLayout *mainGridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *listExperimentsPushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *createPushButton;
    QPushButton *edtExperimentPushButton;
    QPushButton *delExperimentPushButton;

    void setupUi(QWidget *ExperimentManagerWidget)
    {
        if (ExperimentManagerWidget->objectName().isEmpty())
            ExperimentManagerWidget->setObjectName("ExperimentManagerWidget");
        ExperimentManagerWidget->resize(817, 482);
        gridLayout = new QGridLayout(ExperimentManagerWidget);
        gridLayout->setObjectName("gridLayout");
        mainGridLayout = new QGridLayout();
        mainGridLayout->setObjectName("mainGridLayout");

        gridLayout->addLayout(mainGridLayout, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        listExperimentsPushButton = new QPushButton(ExperimentManagerWidget);
        listExperimentsPushButton->setObjectName("listExperimentsPushButton");

        horizontalLayout->addWidget(listExperimentsPushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        createPushButton = new QPushButton(ExperimentManagerWidget);
        createPushButton->setObjectName("createPushButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(createPushButton->sizePolicy().hasHeightForWidth());
        createPushButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(createPushButton);

        edtExperimentPushButton = new QPushButton(ExperimentManagerWidget);
        edtExperimentPushButton->setObjectName("edtExperimentPushButton");

        horizontalLayout->addWidget(edtExperimentPushButton);

        delExperimentPushButton = new QPushButton(ExperimentManagerWidget);
        delExperimentPushButton->setObjectName("delExperimentPushButton");

        horizontalLayout->addWidget(delExperimentPushButton);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(ExperimentManagerWidget);

        QMetaObject::connectSlotsByName(ExperimentManagerWidget);
    } // setupUi

    void retranslateUi(QWidget *ExperimentManagerWidget)
    {
        ExperimentManagerWidget->setWindowTitle(QCoreApplication::translate("ExperimentManagerWidget", "Form", nullptr));
        listExperimentsPushButton->setText(QCoreApplication::translate("ExperimentManagerWidget", "List Experiments", nullptr));
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
