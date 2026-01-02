/********************************************************************************
** Form generated from reading UI file 'uiReactorViewWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIREACTORVIEWWIDGET_H
#define UI_UIREACTORVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReactorViewWidget
{
public:

    void setupUi(QWidget *ReactorViewWidget)
    {
        if (ReactorViewWidget->objectName().isEmpty())
            ReactorViewWidget->setObjectName("ReactorViewWidget");
        ReactorViewWidget->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ReactorViewWidget->sizePolicy().hasHeightForWidth());
        ReactorViewWidget->setSizePolicy(sizePolicy);

        retranslateUi(ReactorViewWidget);

        QMetaObject::connectSlotsByName(ReactorViewWidget);
    } // setupUi

    void retranslateUi(QWidget *ReactorViewWidget)
    {
        ReactorViewWidget->setWindowTitle(QCoreApplication::translate("ReactorViewWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReactorViewWidget: public Ui_ReactorViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIREACTORVIEWWIDGET_H
