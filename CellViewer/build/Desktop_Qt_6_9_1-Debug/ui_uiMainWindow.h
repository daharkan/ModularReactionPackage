/********************************************************************************
** Form generated from reading UI file 'uiMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIMAINWINDOW_H
#define UI_UIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QGridLayout *cellLayout;
    QGridLayout *gridLayout;
    QLineEdit *pidKiLineEdit;
    QLabel *heaterDutyCLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *pidOutputLabel;
    QLabel *label_10;
    QLabel *label_4;
    QLineEdit *pidKpLineEdit;
    QLabel *label_3;
    QLineEdit *pidKdLineEdit;
    QLabel *label;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_8;
    QPushButton *updatePIDParametersButton;
    QLabel *peltierDutyCLabel;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *assignExp1Button;
    QPushButton *assignExp2Button;
    QPushButton *assignExp3Button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 1, 1, 1);

        cellLayout = new QGridLayout();
        cellLayout->setObjectName("cellLayout");
        cellLayout->setContentsMargins(0, -1, -1, -1);

        gridLayout_2->addLayout(cellLayout, 2, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        pidKiLineEdit = new QLineEdit(centralwidget);
        pidKiLineEdit->setObjectName("pidKiLineEdit");

        gridLayout->addWidget(pidKiLineEdit, 2, 5, 1, 1);

        heaterDutyCLabel = new QLabel(centralwidget);
        heaterDutyCLabel->setObjectName("heaterDutyCLabel");
        QFont font;
        font.setBold(true);
        heaterDutyCLabel->setFont(font);

        gridLayout->addWidget(heaterDutyCLabel, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 3, 1, 1);

        pidOutputLabel = new QLabel(centralwidget);
        pidOutputLabel->setObjectName("pidOutputLabel");
        pidOutputLabel->setFont(font);

        gridLayout->addWidget(pidOutputLabel, 3, 2, 1, 1);

        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");

        gridLayout->addWidget(label_10, 3, 1, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 4, 1, 2);

        pidKpLineEdit = new QLineEdit(centralwidget);
        pidKpLineEdit->setObjectName("pidKpLineEdit");

        gridLayout->addWidget(pidKpLineEdit, 1, 5, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 3, 4, 1, 1);

        pidKdLineEdit = new QLineEdit(centralwidget);
        pidKdLineEdit->setObjectName("pidKdLineEdit");

        gridLayout->addWidget(pidKdLineEdit, 3, 5, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 4, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 4, 1, 1);

        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");

        gridLayout->addWidget(label_8, 2, 1, 1, 1);

        updatePIDParametersButton = new QPushButton(centralwidget);
        updatePIDParametersButton->setObjectName("updatePIDParametersButton");
        updatePIDParametersButton->setFont(font);

        gridLayout->addWidget(updatePIDParametersButton, 4, 5, 1, 1);

        peltierDutyCLabel = new QLabel(centralwidget);
        peltierDutyCLabel->setObjectName("peltierDutyCLabel");
        peltierDutyCLabel->setFont(font);

        gridLayout->addWidget(peltierDutyCLabel, 2, 2, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 0, 1, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        assignExp1Button = new QPushButton(centralwidget);
        assignExp1Button->setObjectName("assignExp1Button");

        horizontalLayout->addWidget(assignExp1Button);

        assignExp2Button = new QPushButton(centralwidget);
        assignExp2Button->setObjectName("assignExp2Button");

        horizontalLayout->addWidget(assignExp2Button);

        assignExp3Button = new QPushButton(centralwidget);
        assignExp3Button->setObjectName("assignExp3Button");

        horizontalLayout->addWidget(assignExp3Button);


        gridLayout_2->addLayout(horizontalLayout, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        heaterDutyCLabel->setText(QString());
        pidOutputLabel->setText(QString());
        label_10->setText(QCoreApplication::translate("MainWindow", "PID Output:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:16pt; font-weight:700; color:#26a269;\">PID Parameters</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Kd", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Kp", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Heater Duty Cycle:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Ki", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Peltier Duty Cycle:", nullptr));
        updatePIDParametersButton->setText(QCoreApplication::translate("MainWindow", "Update PID Parameters", nullptr));
        peltierDutyCLabel->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700; color:#26a269;\">Duty Cycle Values</span></p></body></html>", nullptr));
        assignExp1Button->setText(QCoreApplication::translate("MainWindow", "Assign Exp 1", nullptr));
        assignExp2Button->setText(QCoreApplication::translate("MainWindow", "Assign Exp 2", nullptr));
        assignExp3Button->setText(QCoreApplication::translate("MainWindow", "Assign Exp 3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIMAINWINDOW_H
