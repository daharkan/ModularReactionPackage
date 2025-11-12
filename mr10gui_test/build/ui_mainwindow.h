/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QTableWidget *tableWidget;
    QSpacerItem *horizontalSpacer;
    QLineEdit *targetTempLineEdit;
    QLabel *label_3;
    QPushButton *startUpdateButton;
    QLineEdit *targetRPMLineEdit;
    QLabel *label;
    QComboBox *cellNoComboBox;
    QPushButton *senPushButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1098, 597);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 3, 1, 1, 1);

        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 11)
            tableWidget->setColumnCount(11);
        if (tableWidget->rowCount() < 6)
            tableWidget->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(5, __qtablewidgetitem5);
        tableWidget->setObjectName("tableWidget");

        gridLayout->addWidget(tableWidget, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        targetTempLineEdit = new QLineEdit(centralwidget);
        targetTempLineEdit->setObjectName("targetTempLineEdit");

        gridLayout->addWidget(targetTempLineEdit, 3, 2, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 4, 1, 1, 1);

        startUpdateButton = new QPushButton(centralwidget);
        startUpdateButton->setObjectName("startUpdateButton");

        gridLayout->addWidget(startUpdateButton, 2, 0, 1, 1);

        targetRPMLineEdit = new QLineEdit(centralwidget);
        targetRPMLineEdit->setObjectName("targetRPMLineEdit");

        gridLayout->addWidget(targetRPMLineEdit, 4, 2, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 2, 1, 1, 1);

        cellNoComboBox = new QComboBox(centralwidget);
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->addItem(QString());
        cellNoComboBox->setObjectName("cellNoComboBox");

        gridLayout->addWidget(cellNoComboBox, 2, 2, 1, 1);

        senPushButton = new QPushButton(centralwidget);
        senPushButton->setObjectName("senPushButton");

        gridLayout->addWidget(senPushButton, 5, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Target Temp:", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Cell ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Inner Temp", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Ext. Temp", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "RPM", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Target RPM", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Target Temp", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Target RPM:", nullptr));
        startUpdateButton->setText(QCoreApplication::translate("MainWindow", "Start Updating", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Cell No:", nullptr));
        cellNoComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        cellNoComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));
        cellNoComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        cellNoComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "4", nullptr));
        cellNoComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "5", nullptr));
        cellNoComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "6", nullptr));
        cellNoComboBox->setItemText(6, QCoreApplication::translate("MainWindow", "7", nullptr));
        cellNoComboBox->setItemText(7, QCoreApplication::translate("MainWindow", "8", nullptr));
        cellNoComboBox->setItemText(8, QCoreApplication::translate("MainWindow", "9", nullptr));
        cellNoComboBox->setItemText(9, QCoreApplication::translate("MainWindow", "10", nullptr));

        senPushButton->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
