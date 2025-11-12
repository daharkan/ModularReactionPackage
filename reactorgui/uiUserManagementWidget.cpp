#include "uiUserManagementWidget.h"
#include "ui_uiUserManagementWidget.h"

UserManagementWidget::UserManagementWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserManagementWidget)
{
    ui->setupUi(this);
}

UserManagementWidget::~UserManagementWidget()
{
    delete ui;
}
