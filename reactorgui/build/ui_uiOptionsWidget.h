/********************************************************************************
** Form generated from reading UI file 'uiOptionsWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIOPTIONSWIDGET_H
#define UI_UIOPTIONSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionsWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *applyPrefsPushButton;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QComboBox *themesComboBox;

    void setupUi(QWidget *OptionsWidget)
    {
        if (OptionsWidget->objectName().isEmpty())
            OptionsWidget->setObjectName("OptionsWidget");
        OptionsWidget->resize(448, 332);
        gridLayout = new QGridLayout(OptionsWidget);
        gridLayout->setObjectName("gridLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 3, 1, 1);

        applyPrefsPushButton = new QPushButton(OptionsWidget);
        applyPrefsPushButton->setObjectName("applyPrefsPushButton");

        gridLayout->addWidget(applyPrefsPushButton, 3, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 2, 1, 1);

        label_2 = new QLabel(OptionsWidget);
        label_2->setObjectName("label_2");
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 0, 0, 1, 3);

        label_3 = new QLabel(OptionsWidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label = new QLabel(OptionsWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 2, 0, 1, 1);

        themesComboBox = new QComboBox(OptionsWidget);
        themesComboBox->setObjectName("themesComboBox");

        gridLayout->addWidget(themesComboBox, 2, 1, 1, 2);


        retranslateUi(OptionsWidget);

        QMetaObject::connectSlotsByName(OptionsWidget);
    } // setupUi

    void retranslateUi(QWidget *OptionsWidget)
    {
        OptionsWidget->setWindowTitle(QCoreApplication::translate("OptionsWidget", "Form", nullptr));
        applyPrefsPushButton->setText(QCoreApplication::translate("OptionsWidget", "Apply", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        label->setText(QCoreApplication::translate("OptionsWidget", "Themes:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionsWidget: public Ui_OptionsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIOPTIONSWIDGET_H
