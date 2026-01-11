#ifndef UIUSERMANAGEMENTWIDGET_H
#define UIUSERMANAGEMENTWIDGET_H

#include <QWidget>
#include "User.h"

namespace Ui {
class UserManagementWidget;
}

class UserManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserManagementWidget(QWidget *parent = nullptr);
    ~UserManagementWidget();
    void setCurrentUser(const User &user);

private:
    Ui::UserManagementWidget *ui;
    User m_currentUser;

    void refreshUsers();
    void updateRoleOptions();

private slots:
    void handleCreateUser();
    void handleDeleteUser();
    void handleRefreshUsers();
    void handleChangePassword();
};

#endif // UIUSERMANAGEMENTWIDGET_H
