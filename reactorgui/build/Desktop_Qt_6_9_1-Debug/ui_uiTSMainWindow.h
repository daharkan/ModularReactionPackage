/********************************************************************************
** Form generated from reading UI file 'uiTSMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UITSMAINWINDOW_H
#define UI_UITSMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TSMainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QWidget *menuContainer;
    QHBoxLayout *menuContainerLayout;
    QStackedWidget *mainStackedWidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TSMainWindow)
    {
        if (TSMainWindow->objectName().isEmpty())
            TSMainWindow->setObjectName("TSMainWindow");
        TSMainWindow->resize(610, 437);
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TSMainWindow->sizePolicy().hasHeightForWidth());
        TSMainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(TSMainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        menuContainer = new QWidget(centralwidget);
        menuContainer->setObjectName("menuContainer");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(menuContainer->sizePolicy().hasHeightForWidth());
        menuContainer->setSizePolicy(sizePolicy1);
        menuContainerLayout = new QHBoxLayout(menuContainer);
        menuContainerLayout->setObjectName("menuContainerLayout");
        menuContainerLayout->setContentsMargins(0, 0, 0, 0);

        mainLayout->addWidget(menuContainer);

        mainStackedWidget = new QStackedWidget(centralwidget);
        mainStackedWidget->setObjectName("mainStackedWidget");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mainStackedWidget->sizePolicy().hasHeightForWidth());
        mainStackedWidget->setSizePolicy(sizePolicy2);

        mainLayout->addWidget(mainStackedWidget);

        TSMainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(TSMainWindow);
        statusbar->setObjectName("statusbar");
        TSMainWindow->setStatusBar(statusbar);

        retranslateUi(TSMainWindow);

        mainStackedWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(TSMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TSMainWindow)
    {
        TSMainWindow->setWindowTitle(QCoreApplication::translate("TSMainWindow", "TSMainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TSMainWindow: public Ui_TSMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UITSMAINWINDOW_H
