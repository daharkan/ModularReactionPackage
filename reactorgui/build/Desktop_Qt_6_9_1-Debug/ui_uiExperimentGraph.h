/********************************************************************************
** Form generated from reading UI file 'uiExperimentGraph.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIEXPERIMENTGRAPH_H
#define UI_UIEXPERIMENTGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExperimentGraph
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *graphLayout;

    void setupUi(QWidget *ExperimentGraph)
    {
        if (ExperimentGraph->objectName().isEmpty())
            ExperimentGraph->setObjectName("ExperimentGraph");
        ExperimentGraph->resize(617, 338);
        gridLayout_2 = new QGridLayout(ExperimentGraph);
        gridLayout_2->setObjectName("gridLayout_2");
        graphLayout = new QGridLayout();
        graphLayout->setObjectName("graphLayout");

        gridLayout_2->addLayout(graphLayout, 0, 0, 1, 1);


        retranslateUi(ExperimentGraph);

        QMetaObject::connectSlotsByName(ExperimentGraph);
    } // setupUi

    void retranslateUi(QWidget *ExperimentGraph)
    {
        ExperimentGraph->setWindowTitle(QCoreApplication::translate("ExperimentGraph", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExperimentGraph: public Ui_ExperimentGraph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIEXPERIMENTGRAPH_H
