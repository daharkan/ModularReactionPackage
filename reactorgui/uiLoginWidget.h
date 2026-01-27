#ifndef UILOGINWIDGET_H
#define UILOGINWIDGET_H

#include <QWidget>
#include "User.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(bool isMachineConnected, QWidget *parent = nullptr);
    ~LoginWidget();
    User currentUser() const;
    std::string selectedMachineId() const;

private:
    Ui::LoginWidget *ui;
    bool m_isMachineConnected;
    User m_currentUser;
    void refreshMachineList();

private slots:
    void loginPressed();

signals:
    void sgn_loginSucceed();
    void sgn_loginFailed();
};

#endif // UILOGINWIDGET_H
