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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TSMainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *upperHorizontalLayout;
    QLabel *pageHeadlineLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *backPushButton;
    QPushButton *homePushButton;
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
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        upperHorizontalLayout = new QHBoxLayout();
        upperHorizontalLayout->setObjectName("upperHorizontalLayout");
        upperHorizontalLayout->setContentsMargins(-1, 0, -1, -1);
        pageHeadlineLabel = new QLabel(centralwidget);
        pageHeadlineLabel->setObjectName("pageHeadlineLabel");

        upperHorizontalLayout->addWidget(pageHeadlineLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        upperHorizontalLayout->addItem(horizontalSpacer);

        backPushButton = new QPushButton(centralwidget);
        backPushButton->setObjectName("backPushButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/undo.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        backPushButton->setIcon(icon);
        backPushButton->setIconSize(QSize(32, 32));

        upperHorizontalLayout->addWidget(backPushButton);

        homePushButton = new QPushButton(centralwidget);
        homePushButton->setObjectName("homePushButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/home.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        homePushButton->setIcon(icon1);
        homePushButton->setIconSize(QSize(32, 32));

        upperHorizontalLayout->addWidget(homePushButton);


        gridLayout->addLayout(upperHorizontalLayout, 0, 0, 1, 2);

        mainStackedWidget = new QStackedWidget(centralwidget);
        mainStackedWidget->setObjectName("mainStackedWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mainStackedWidget->sizePolicy().hasHeightForWidth());
        mainStackedWidget->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(mainStackedWidget, 1, 0, 1, 2);

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
        pageHeadlineLabel->setText(QString());
        backPushButton->setText(QString());
        homePushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TSMainWindow: public Ui_TSMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UITSMAINWINDOW_H
