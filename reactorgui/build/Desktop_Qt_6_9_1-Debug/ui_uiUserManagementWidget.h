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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserManagementWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *changePasswordGroupBox;
    QGridLayout *changePasswordLayout;
    QLabel *currentPasswordLabel;
    QLineEdit *currentPasswordLineEdit;
    QLabel *newPasswordLabel;
    QLineEdit *newPasswordLineEdit;
    QLabel *confirmPasswordLabel;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *changePasswordPushButton;
    QGroupBox *adminGroupBox;
    QVBoxLayout *adminLayout;
    QGridLayout *createUserLayout;
    QLabel *newUsernameLabel;
    QLineEdit *newUsernameLineEdit;
    QLabel *initialPasswordLabel;
    QLineEdit *initialPasswordLineEdit;
    QLabel *roleLabel;
    QComboBox *roleComboBox;
    QPushButton *createUserPushButton;
    QTableWidget *usersTableWidget;
    QHBoxLayout *adminButtonsLayout;
    QPushButton *refreshUsersPushButton;
    QSpacerItem *adminButtonsSpacer;
    QPushButton *deleteUserPushButton;

    void setupUi(QWidget *UserManagementWidget)
    {
        if (UserManagementWidget->objectName().isEmpty())
            UserManagementWidget->setObjectName("UserManagementWidget");
        UserManagementWidget->resize(689, 420);
        verticalLayout = new QVBoxLayout(UserManagementWidget);
        verticalLayout->setObjectName("verticalLayout");
        changePasswordGroupBox = new QGroupBox(UserManagementWidget);
        changePasswordGroupBox->setObjectName("changePasswordGroupBox");
        changePasswordLayout = new QGridLayout(changePasswordGroupBox);
        changePasswordLayout->setObjectName("changePasswordLayout");
        currentPasswordLabel = new QLabel(changePasswordGroupBox);
        currentPasswordLabel->setObjectName("currentPasswordLabel");

        changePasswordLayout->addWidget(currentPasswordLabel, 0, 0, 1, 1);

        currentPasswordLineEdit = new QLineEdit(changePasswordGroupBox);
        currentPasswordLineEdit->setObjectName("currentPasswordLineEdit");
        currentPasswordLineEdit->setEchoMode(QLineEdit::Password);

        changePasswordLayout->addWidget(currentPasswordLineEdit, 0, 1, 1, 1);

        newPasswordLabel = new QLabel(changePasswordGroupBox);
        newPasswordLabel->setObjectName("newPasswordLabel");

        changePasswordLayout->addWidget(newPasswordLabel, 1, 0, 1, 1);

        newPasswordLineEdit = new QLineEdit(changePasswordGroupBox);
        newPasswordLineEdit->setObjectName("newPasswordLineEdit");
        newPasswordLineEdit->setEchoMode(QLineEdit::Password);

        changePasswordLayout->addWidget(newPasswordLineEdit, 1, 1, 1, 1);

        confirmPasswordLabel = new QLabel(changePasswordGroupBox);
        confirmPasswordLabel->setObjectName("confirmPasswordLabel");

        changePasswordLayout->addWidget(confirmPasswordLabel, 2, 0, 1, 1);

        confirmPasswordLineEdit = new QLineEdit(changePasswordGroupBox);
        confirmPasswordLineEdit->setObjectName("confirmPasswordLineEdit");
        confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

        changePasswordLayout->addWidget(confirmPasswordLineEdit, 2, 1, 1, 1);

        changePasswordPushButton = new QPushButton(changePasswordGroupBox);
        changePasswordPushButton->setObjectName("changePasswordPushButton");

        changePasswordLayout->addWidget(changePasswordPushButton, 3, 1, 1, 1, Qt::AlignRight);


        verticalLayout->addWidget(changePasswordGroupBox);

        adminGroupBox = new QGroupBox(UserManagementWidget);
        adminGroupBox->setObjectName("adminGroupBox");
        adminLayout = new QVBoxLayout(adminGroupBox);
        adminLayout->setObjectName("adminLayout");
        createUserLayout = new QGridLayout();
        createUserLayout->setObjectName("createUserLayout");
        newUsernameLabel = new QLabel(adminGroupBox);
        newUsernameLabel->setObjectName("newUsernameLabel");

        createUserLayout->addWidget(newUsernameLabel, 0, 0, 1, 1);

        newUsernameLineEdit = new QLineEdit(adminGroupBox);
        newUsernameLineEdit->setObjectName("newUsernameLineEdit");

        createUserLayout->addWidget(newUsernameLineEdit, 0, 1, 1, 1);

        initialPasswordLabel = new QLabel(adminGroupBox);
        initialPasswordLabel->setObjectName("initialPasswordLabel");

        createUserLayout->addWidget(initialPasswordLabel, 1, 0, 1, 1);

        initialPasswordLineEdit = new QLineEdit(adminGroupBox);
        initialPasswordLineEdit->setObjectName("initialPasswordLineEdit");
        initialPasswordLineEdit->setEchoMode(QLineEdit::Password);

        createUserLayout->addWidget(initialPasswordLineEdit, 1, 1, 1, 1);

        roleLabel = new QLabel(adminGroupBox);
        roleLabel->setObjectName("roleLabel");

        createUserLayout->addWidget(roleLabel, 2, 0, 1, 1);

        roleComboBox = new QComboBox(adminGroupBox);
        roleComboBox->setObjectName("roleComboBox");

        createUserLayout->addWidget(roleComboBox, 2, 1, 1, 1);

        createUserPushButton = new QPushButton(adminGroupBox);
        createUserPushButton->setObjectName("createUserPushButton");

        createUserLayout->addWidget(createUserPushButton, 3, 1, 1, 1, Qt::AlignRight);


        adminLayout->addLayout(createUserLayout);

        usersTableWidget = new QTableWidget(adminGroupBox);
        usersTableWidget->setObjectName("usersTableWidget");

        adminLayout->addWidget(usersTableWidget);

        adminButtonsLayout = new QHBoxLayout();
        adminButtonsLayout->setObjectName("adminButtonsLayout");
        refreshUsersPushButton = new QPushButton(adminGroupBox);
        refreshUsersPushButton->setObjectName("refreshUsersPushButton");

        adminButtonsLayout->addWidget(refreshUsersPushButton);

        adminButtonsSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        adminButtonsLayout->addItem(adminButtonsSpacer);

        deleteUserPushButton = new QPushButton(adminGroupBox);
        deleteUserPushButton->setObjectName("deleteUserPushButton");

        adminButtonsLayout->addWidget(deleteUserPushButton);


        adminLayout->addLayout(adminButtonsLayout);


        verticalLayout->addWidget(adminGroupBox);


        retranslateUi(UserManagementWidget);

        QMetaObject::connectSlotsByName(UserManagementWidget);
    } // setupUi

    void retranslateUi(QWidget *UserManagementWidget)
    {
        UserManagementWidget->setWindowTitle(QCoreApplication::translate("UserManagementWidget", "Form", nullptr));
        changePasswordGroupBox->setTitle(QCoreApplication::translate("UserManagementWidget", "Change Password", nullptr));
        currentPasswordLabel->setText(QCoreApplication::translate("UserManagementWidget", "Current Password:", nullptr));
        newPasswordLabel->setText(QCoreApplication::translate("UserManagementWidget", "New Password:", nullptr));
        confirmPasswordLabel->setText(QCoreApplication::translate("UserManagementWidget", "Confirm Password:", nullptr));
        changePasswordPushButton->setText(QCoreApplication::translate("UserManagementWidget", "Change Password", nullptr));
        adminGroupBox->setTitle(QCoreApplication::translate("UserManagementWidget", "Admin Tools", nullptr));
        newUsernameLabel->setText(QCoreApplication::translate("UserManagementWidget", "Username:", nullptr));
        initialPasswordLabel->setText(QCoreApplication::translate("UserManagementWidget", "Initial Password:", nullptr));
        roleLabel->setText(QCoreApplication::translate("UserManagementWidget", "Role:", nullptr));
        createUserPushButton->setText(QCoreApplication::translate("UserManagementWidget", "Create User", nullptr));
        refreshUsersPushButton->setText(QCoreApplication::translate("UserManagementWidget", "Refresh", nullptr));
        deleteUserPushButton->setText(QCoreApplication::translate("UserManagementWidget", "Delete User", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserManagementWidget: public Ui_UserManagementWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIUSERMANAGEMENTWIDGET_H
