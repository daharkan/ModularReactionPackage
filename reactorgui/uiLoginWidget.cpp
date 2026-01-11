#include "uiLoginWidget.h"
#include "ui_uiLoginWidget.h"

LoginWidget::LoginWidget(bool isMachineConnected, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , m_isMachineConnected(isMachineConnected)
{
    ui->setupUi(this);

    if(!m_isMachineConnected){
        ui->machineComboBox->hide();
        ui->machneLabel->hide();
    }

    connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginWidget::loginPressed);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

User LoginWidget::currentUser() const
{
    return m_currentUser;
}

void LoginWidget::loginPressed()
{
    QString username = ui->userLineEdit->text().trimmed();
    QString password = ui->passLineEdit->text().trimmed();
    if(username.contains("a") && password.contains("a")){
        User user;
        user.setUsername(username.toStdString());
        user.setPassword(password.toStdString());
        if (username.compare("root", Qt::CaseInsensitive) == 0) {
            user.setRole(ROLE_ROOT);
        } else if (username.compare("admin", Qt::CaseInsensitive) == 0) {
            user.setRole(ROLE_ADMIN);
        } else {
            user.setRole(ROLE_OPERATOR);
        }
        m_currentUser = user;
        emit sgn_loginSucceed();
    }else{
        ui->loginMessageLabel->setText(tr("Login Failed. Username or password is wrong. Please try again."));
    }
}
