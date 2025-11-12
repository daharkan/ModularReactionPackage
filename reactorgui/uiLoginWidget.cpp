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

void LoginWidget::loginPressed()
{
    if(ui->userLineEdit->text().contains("a") && ui->userLineEdit->text().contains("a")){
        emit sgn_loginSucceed();
    }else{
        ui->loginMessageLabel->setText(tr("Login Failed. Username or password is wrong. Please try again."));
    }
}
