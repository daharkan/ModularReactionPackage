#include "uiUserManagementWidget.h"
#include "ui_uiUserManagementWidget.h"
#include "RedisDBManager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <vector>

namespace {
QString roleName(UserRole role)
{
    switch (role) {
    case ROLE_ROOT:
        return "Root";
    case ROLE_ADMIN:
        return "Admin";
    case ROLE_OPERATOR:
        return "Operator";
    default:
        return "Unknown";
    }
}
} // namespace

UserManagementWidget::UserManagementWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserManagementWidget)
{
    ui->setupUi(this);

    ui->usersTableWidget->setColumnCount(2);
    ui->usersTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Username") << tr("Role"));
    ui->usersTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->usersTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usersTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->createUserPushButton, &QPushButton::clicked, this, &UserManagementWidget::handleCreateUser);
    connect(ui->deleteUserPushButton, &QPushButton::clicked, this, &UserManagementWidget::handleDeleteUser);
    connect(ui->refreshUsersPushButton, &QPushButton::clicked, this, &UserManagementWidget::handleRefreshUsers);
    connect(ui->changePasswordPushButton, &QPushButton::clicked, this, &UserManagementWidget::handleChangePassword);

    updateRoleOptions();
}

UserManagementWidget::~UserManagementWidget()
{
    delete ui;
}

void UserManagementWidget::setCurrentUser(const User &user)
{
    m_currentUser = user;
    bool isAdmin = m_currentUser.role() == ROLE_ADMIN || m_currentUser.role() == ROLE_ROOT;
    ui->adminGroupBox->setVisible(isAdmin);
    refreshUsers();
}

void UserManagementWidget::refreshUsers()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    std::vector<User> users = RedisDBManager::getInstance()->getUsers();
    ui->usersTableWidget->setRowCount(static_cast<int>(users.size()));

    for (int row = 0; row < static_cast<int>(users.size()); ++row) {
        const User &user = users.at(row);
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(user.username()));
        nameItem->setData(Qt::UserRole, QString::fromStdString(user.username()));
        ui->usersTableWidget->setItem(row, 0, nameItem);

        QTableWidgetItem *roleItem = new QTableWidgetItem(roleName(user.role()));
        ui->usersTableWidget->setItem(row, 1, roleItem);
    }
}

void UserManagementWidget::updateRoleOptions()
{
    ui->roleComboBox->clear();
    ui->roleComboBox->addItem(tr("Operator"), ROLE_OPERATOR);
    ui->roleComboBox->addItem(tr("Admin"), ROLE_ADMIN);
}

void UserManagementWidget::handleCreateUser()
{
    if (!(m_currentUser.role() == ROLE_ADMIN || m_currentUser.role() == ROLE_ROOT)) {
        return;
    }

    QString username = ui->newUsernameLineEdit->text().trimmed();
    QString password = ui->initialPasswordLineEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, tr("Create User"), tr("Username and password are required."));
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    if (RedisDBManager::getInstance()->getUser(username.toStdString()).has_value()) {
        QMessageBox::warning(this, tr("Create User"), tr("User already exists."));
        return;
    }

    User user;
    user.setUsername(username.toStdString());
    user.setPassword(password.toStdString());
    user.setRole(static_cast<UserRole>(ui->roleComboBox->currentData().toInt()));
    RedisDBManager::getInstance()->pushUser(user);

    ui->newUsernameLineEdit->clear();
    ui->initialPasswordLineEdit->clear();
    refreshUsers();
}

void UserManagementWidget::handleDeleteUser()
{
    if (!(m_currentUser.role() == ROLE_ADMIN || m_currentUser.role() == ROLE_ROOT)) {
        return;
    }

    QList<QTableWidgetItem*> items = ui->usersTableWidget->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QString username = ui->usersTableWidget->item(items.first()->row(), 0)->data(Qt::UserRole).toString();
    if (username.isEmpty()) {
        return;
    }
    if (username == QString::fromStdString(m_currentUser.username())) {
        QMessageBox::warning(this, tr("Delete User"), tr("You cannot delete your own account."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Delete User"),
        tr("Delete user '%1'?").arg(username),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    RedisDBManager::getInstance()->deleteUser(username.toStdString());
    refreshUsers();
}

void UserManagementWidget::handleRefreshUsers()
{
    refreshUsers();
}

void UserManagementWidget::handleChangePassword()
{
    QString currentPassword = ui->currentPasswordLineEdit->text();
    QString newPassword = ui->newPasswordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (currentPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, tr("Change Password"), tr("All password fields are required."));
        return;
    }
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, tr("Change Password"), tr("New password entries do not match."));
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    auto existing = RedisDBManager::getInstance()->getUser(m_currentUser.username());
    if (!existing.has_value()) {
        QMessageBox::warning(this, tr("Change Password"), tr("User not found."));
        return;
    }

    if (existing->password() != currentPassword.toStdString()) {
        QMessageBox::warning(this, tr("Change Password"), tr("Current password is incorrect."));
        return;
    }

    User updated = existing.value();
    updated.setPassword(newPassword.toStdString());
    RedisDBManager::getInstance()->pushUser(updated);
    m_currentUser = updated;

    ui->currentPasswordLineEdit->clear();
    ui->newPasswordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();
    QMessageBox::information(this, tr("Change Password"), tr("Password updated."));
}
