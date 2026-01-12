/********************************************************************************
** Form generated from reading UI file 'uiOptionsWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIOPTIONSWIDGET_H
#define UI_UIOPTIONSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionsWidget
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QGroupBox *appearanceGroupBox;
    QFormLayout *appearanceFormLayout;
    QLabel *menuPositionLabel;
    QComboBox *menuPositionComboBox;
    QLabel *themeLabel;
    QComboBox *themesComboBox;
    QHBoxLayout *actionLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *applyPrefsPushButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *OptionsWidget)
    {
        if (OptionsWidget->objectName().isEmpty())
            OptionsWidget->setObjectName("OptionsWidget");
        OptionsWidget->resize(448, 332);
        mainLayout = new QVBoxLayout(OptionsWidget);
        mainLayout->setObjectName("mainLayout");
        titleLabel = new QLabel(OptionsWidget);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        titleLabel->setFont(font);

        mainLayout->addWidget(titleLabel);

        appearanceGroupBox = new QGroupBox(OptionsWidget);
        appearanceGroupBox->setObjectName("appearanceGroupBox");
        appearanceFormLayout = new QFormLayout(appearanceGroupBox);
        appearanceFormLayout->setObjectName("appearanceFormLayout");
        menuPositionLabel = new QLabel(appearanceGroupBox);
        menuPositionLabel->setObjectName("menuPositionLabel");

        appearanceFormLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, menuPositionLabel);

        menuPositionComboBox = new QComboBox(appearanceGroupBox);
        menuPositionComboBox->setObjectName("menuPositionComboBox");

        appearanceFormLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, menuPositionComboBox);

        themeLabel = new QLabel(appearanceGroupBox);
        themeLabel->setObjectName("themeLabel");

        appearanceFormLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, themeLabel);

        themesComboBox = new QComboBox(appearanceGroupBox);
        themesComboBox->setObjectName("themesComboBox");

        appearanceFormLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, themesComboBox);


        mainLayout->addWidget(appearanceGroupBox);

        actionLayout = new QHBoxLayout();
        actionLayout->setObjectName("actionLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionLayout->addItem(horizontalSpacer);

        applyPrefsPushButton = new QPushButton(OptionsWidget);
        applyPrefsPushButton->setObjectName("applyPrefsPushButton");

        actionLayout->addWidget(applyPrefsPushButton);


        mainLayout->addLayout(actionLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer);


        retranslateUi(OptionsWidget);

        QMetaObject::connectSlotsByName(OptionsWidget);
    } // setupUi

    void retranslateUi(QWidget *OptionsWidget)
    {
        OptionsWidget->setWindowTitle(QCoreApplication::translate("OptionsWidget", "Form", nullptr));
        titleLabel->setText(QCoreApplication::translate("OptionsWidget", "Options", nullptr));
        appearanceGroupBox->setTitle(QCoreApplication::translate("OptionsWidget", "Appearance", nullptr));
        menuPositionLabel->setText(QCoreApplication::translate("OptionsWidget", "Menu Position:", nullptr));
        themeLabel->setText(QCoreApplication::translate("OptionsWidget", "Theme:", nullptr));
        applyPrefsPushButton->setText(QCoreApplication::translate("OptionsWidget", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionsWidget: public Ui_OptionsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIOPTIONSWIDGET_H
