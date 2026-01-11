/********************************************************************************
** Form generated from reading UI file 'uiHomeWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIHOMEWIDGET_H
#define UI_UIHOMEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HomeWidget
{
public:
    QGridLayout *gridLayout;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QPushButton *reactorPushButton;
    QPushButton *experimentManagerPushButton;
    QPushButton *optionsPushButton;
    QPushButton *userManagementPushButton;
    QPushButton *closePushButton;
    QLabel *label;

    void setupUi(QWidget *HomeWidget)
    {
        if (HomeWidget->objectName().isEmpty())
            HomeWidget->setObjectName("HomeWidget");
        HomeWidget->resize(969, 654);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(HomeWidget->sizePolicy().hasHeightForWidth());
        HomeWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(HomeWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        line = new QFrame(HomeWidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        reactorPushButton = new QPushButton(HomeWidget);
        reactorPushButton->setObjectName("reactorPushButton");
        QFont font;
        font.setBold(true);
        reactorPushButton->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/icons/grid.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        reactorPushButton->setIcon(icon);
        reactorPushButton->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(reactorPushButton);

        experimentManagerPushButton = new QPushButton(HomeWidget);
        experimentManagerPushButton->setObjectName("experimentManagerPushButton");
        experimentManagerPushButton->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/bar chart.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        experimentManagerPushButton->setIcon(icon1);
        experimentManagerPushButton->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(experimentManagerPushButton);

        optionsPushButton = new QPushButton(HomeWidget);
        optionsPushButton->setObjectName("optionsPushButton");
        optionsPushButton->setFont(font);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/settings.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        optionsPushButton->setIcon(icon2);
        optionsPushButton->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(optionsPushButton);

        userManagementPushButton = new QPushButton(HomeWidget);
        userManagementPushButton->setObjectName("userManagementPushButton");
        userManagementPushButton->setFont(font);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/icons/users.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        userManagementPushButton->setIcon(icon3);
        userManagementPushButton->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(userManagementPushButton);

        closePushButton = new QPushButton(HomeWidget);
        closePushButton->setObjectName("closePushButton");
        closePushButton->setFont(font);
        closePushButton->setStyleSheet(QString::fromUtf8("image: url(:/resources/icons/power on.svg);"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/icons/power on.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        closePushButton->setIcon(icon4);
        closePushButton->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(closePushButton);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        label = new QLabel(HomeWidget);
        label->setObjectName("label");
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(951, 581));
        label->setPixmap(QPixmap(QString::fromUtf8(":/resources/icons/TAD_Logo.png")));
        label->setScaledContents(true);

        gridLayout->addWidget(label, 1, 0, 1, 1);


        retranslateUi(HomeWidget);

        QMetaObject::connectSlotsByName(HomeWidget);
    } // setupUi

    void retranslateUi(QWidget *HomeWidget)
    {
        HomeWidget->setWindowTitle(QCoreApplication::translate("HomeWidget", "Form", nullptr));
        reactorPushButton->setText(QCoreApplication::translate("HomeWidget", "Reactor", nullptr));
        experimentManagerPushButton->setText(QCoreApplication::translate("HomeWidget", "Experiment Manager", nullptr));
        optionsPushButton->setText(QCoreApplication::translate("HomeWidget", "Options", nullptr));
        userManagementPushButton->setText(QCoreApplication::translate("HomeWidget", "User Management", nullptr));
        closePushButton->setText(QString());
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class HomeWidget: public Ui_HomeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIHOMEWIDGET_H
