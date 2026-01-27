/********************************************************************************
** Form generated from reading UI file 'uiMenuWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIMENUWIDGET_H
#define UI_UIMENUWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuWidget
{
public:
    QHBoxLayout *mainLayout;
    QPushButton *backButton;
    QSpacerItem *horizontalSpacer_2;
    QLabel *titleLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *reactorButton;
    QPushButton *experimentButton;
    QPushButton *userButton;
    QPushButton *optionsButton;

    void setupUi(QWidget *MenuWidget)
    {
        if (MenuWidget->objectName().isEmpty())
            MenuWidget->setObjectName("MenuWidget");
        MenuWidget->resize(1187, 102);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MenuWidget->sizePolicy().hasHeightForWidth());
        MenuWidget->setSizePolicy(sizePolicy);
        MenuWidget->setMinimumSize(QSize(0, 72));
        mainLayout = new QHBoxLayout(MenuWidget);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(12, 8, 12, 8);
        backButton = new QPushButton(MenuWidget);
        backButton->setObjectName("backButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/previous.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        backButton->setIcon(icon);
        backButton->setIconSize(QSize(24, 24));

        mainLayout->addWidget(backButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        mainLayout->addItem(horizontalSpacer_2);

        titleLabel = new QLabel(MenuWidget);
        titleLabel->setObjectName("titleLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titleLabel->sizePolicy().hasHeightForWidth());
        titleLabel->setSizePolicy(sizePolicy1);
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(titleLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        mainLayout->addItem(horizontalSpacer);

        reactorButton = new QPushButton(MenuWidget);
        reactorButton->setObjectName("reactorButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/grid.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        reactorButton->setIcon(icon1);
        reactorButton->setIconSize(QSize(24, 24));

        mainLayout->addWidget(reactorButton);

        experimentButton = new QPushButton(MenuWidget);
        experimentButton->setObjectName("experimentButton");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/bar chart.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        experimentButton->setIcon(icon2);
        experimentButton->setIconSize(QSize(24, 24));

        mainLayout->addWidget(experimentButton);

        userButton = new QPushButton(MenuWidget);
        userButton->setObjectName("userButton");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/icons/users.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        userButton->setIcon(icon3);
        userButton->setIconSize(QSize(24, 24));

        mainLayout->addWidget(userButton);

        optionsButton = new QPushButton(MenuWidget);
        optionsButton->setObjectName("optionsButton");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/icons/settings.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        optionsButton->setIcon(icon4);
        optionsButton->setIconSize(QSize(24, 24));

        mainLayout->addWidget(optionsButton);


        retranslateUi(MenuWidget);

        QMetaObject::connectSlotsByName(MenuWidget);
    } // setupUi

    void retranslateUi(QWidget *MenuWidget)
    {
        MenuWidget->setWindowTitle(QCoreApplication::translate("MenuWidget", "MenuWidget", nullptr));
        backButton->setText(QCoreApplication::translate("MenuWidget", "Back", nullptr));
        titleLabel->setText(QCoreApplication::translate("MenuWidget", "Title", nullptr));
        reactorButton->setText(QCoreApplication::translate("MenuWidget", "Reactor", nullptr));
        experimentButton->setText(QCoreApplication::translate("MenuWidget", "Experiments", nullptr));
        userButton->setText(QCoreApplication::translate("MenuWidget", "Users", nullptr));
        optionsButton->setText(QCoreApplication::translate("MenuWidget", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MenuWidget: public Ui_MenuWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIMENUWIDGET_H
