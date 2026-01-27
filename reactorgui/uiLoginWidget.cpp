#include "uiLoginWidget.h"
#include "ui_uiLoginWidget.h"
#include "RedisDBManager.h"

#include <vector>

namespace {
const char *kNoMachinesText = "No machines";
}

LoginWidget::LoginWidget(bool isMachineConnected, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , m_isMachineConnected(isMachineConnected)
{
    ui->setupUi(this);

    if(!m_isMachineConnected){
        ui->machineComboBox->hide();
        ui->machneLabel->hide();
    } else {
        refreshMachineList();
    }

    connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginWidget::loginPressed);
    connect(ui->userLineEdit, &QLineEdit::returnPressed, this, &LoginWidget::loginPressed);
    connect(ui->passLineEdit, &QLineEdit::returnPressed, this, &LoginWidget::loginPressed);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

User LoginWidget::currentUser() const
{
    return m_currentUser;
}

std::string LoginWidget::selectedMachineId() const
{
    if (!ui->machineComboBox->isEnabled()) {
        return {};
    }
    QString selection = ui->machineComboBox->currentText().trimmed();
    if (selection.isEmpty() || selection == QString::fromUtf8(kNoMachinesText)) {
        return {};
    }
    return selection.toStdString();
}

void LoginWidget::refreshMachineList()
{
    ui->machineComboBox->clear();
    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }

    std::vector<std::string> machineIds;
    if (RedisDBManager::getInstance()->isConnected()) {
        machineIds = RedisDBManager::getInstance()->getMachineIds();
    }

    if (machineIds.empty()) {
        ui->machineComboBox->addItem(QString::fromUtf8(kNoMachinesText));
        ui->machineComboBox->setEnabled(false);
        return;
    }

    ui->machineComboBox->setEnabled(true);
    for (const auto &id : machineIds) {
        ui->machineComboBox->addItem(QString::fromStdString(id));
    }
}

void LoginWidget::loginPressed()
{
    QString username = ui->userLineEdit->text().trimmed();
    QString password = ui->passLineEdit->text().trimmed();
    if (username.isEmpty() || password.isEmpty()) {
        ui->loginMessageLabel->setText(tr("Login Failed. Username or password is missing."));
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }

    auto userOpt = RedisDBManager::getInstance()->getUser(username.toStdString());
    if (!userOpt.has_value()) {
        std::vector<User> existingUsers = RedisDBManager::getInstance()->getUsers();
        QString lower = username.toLower();
        if (existingUsers.empty() && (lower == "admin" || lower == "root")) {
            User user;
            user.setUsername(username.toStdString());
            user.setPassword(password.toStdString());
            user.setRole(lower == "root" ? ROLE_ROOT : ROLE_ADMIN);
            RedisDBManager::getInstance()->pushUser(user);
            m_currentUser = user;
            emit sgn_loginSucceed();
            return;
        }

        ui->loginMessageLabel->setText(tr("Login Failed. Username or password is wrong. Please try again."));
        return;
    }

    User user = userOpt.value();
    if (user.password() != password.toStdString()) {
        ui->loginMessageLabel->setText(tr("Login Failed. Username or password is wrong. Please try again."));
        return;
    }

    m_currentUser = user;
    emit sgn_loginSucceed();
}
