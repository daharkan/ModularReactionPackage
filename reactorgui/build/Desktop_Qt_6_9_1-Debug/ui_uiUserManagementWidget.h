/********************************************************************************
** Form generated from reading UI file 'uiUserManagementWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIUSERMANAGEMENTWIDGET_H
#define UI_UIUSERMANAGEMENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserManagementWidget
{
public:

    void setupUi(QWidget *UserManagementWidget)
    {
        if (UserManagementWidget->objectName().isEmpty())
            UserManagementWidget->setObjectName("UserManagementWidget");
        UserManagementWidget->resize(689, 331);

        retranslateUi(UserManagementWidget);

        QMetaObject::connectSlotsByName(UserManagementWidget);
    } // setupUi

    void retranslateUi(QWidget *UserManagementWidget)
    {
        UserManagementWidget->setWindowTitle(QCoreApplication::translate("UserManagementWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserManagementWidget: public Ui_UserManagementWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIUSERMANAGEMENTWIDGET_H
