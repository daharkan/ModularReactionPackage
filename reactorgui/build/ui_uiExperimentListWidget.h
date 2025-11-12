/********************************************************************************
** Form generated from reading UI file 'uiExperimentListWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIEXPERIMENTLISTWIDGET_H
#define UI_UIEXPERIMENTLISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExperimentListWidget
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QTableWidget *experimentsTableWidget;

    void setupUi(QWidget *ExperimentListWidget)
    {
        if (ExperimentListWidget->objectName().isEmpty())
            ExperimentListWidget->setObjectName("ExperimentListWidget");
        ExperimentListWidget->resize(811, 400);
        gridLayout = new QGridLayout(ExperimentListWidget);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        experimentsTableWidget = new QTableWidget(ExperimentListWidget);
        experimentsTableWidget->setObjectName("experimentsTableWidget");
        experimentsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        experimentsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        experimentsTableWidget->setSortingEnabled(true);

        gridLayout->addWidget(experimentsTableWidget, 1, 0, 1, 1);


        retranslateUi(ExperimentListWidget);

        QMetaObject::connectSlotsByName(ExperimentListWidget);
    } // setupUi

    void retranslateUi(QWidget *ExperimentListWidget)
    {
        ExperimentListWidget->setWindowTitle(QCoreApplication::translate("ExperimentListWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExperimentListWidget: public Ui_ExperimentListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIEXPERIMENTLISTWIDGET_H
