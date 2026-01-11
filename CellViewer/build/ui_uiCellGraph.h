/********************************************************************************
** Form generated from reading UI file 'uiCellGraph.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UICELLGRAPH_H
#define UI_UICELLGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellGraph
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *graphLayout;

    void setupUi(QWidget *CellGraph)
    {
        if (CellGraph->objectName().isEmpty())
            CellGraph->setObjectName("CellGraph");
        CellGraph->resize(617, 338);
        gridLayout_2 = new QGridLayout(CellGraph);
        gridLayout_2->setObjectName("gridLayout_2");
        graphLayout = new QGridLayout();
        graphLayout->setObjectName("graphLayout");

        gridLayout_2->addLayout(graphLayout, 0, 0, 1, 1);


        retranslateUi(CellGraph);

        QMetaObject::connectSlotsByName(CellGraph);
    } // setupUi

    void retranslateUi(QWidget *CellGraph)
    {
        CellGraph->setWindowTitle(QCoreApplication::translate("CellGraph", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CellGraph: public Ui_CellGraph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UICELLGRAPH_H
