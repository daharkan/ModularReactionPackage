#include "uiHomeWidget.h"
#include "ui_uiHomeWidget.h"

#include <QMessageBox>

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWidget)
{
    ui->setupUi(this);

    connect(ui->reactorPushButton, &QPushButton::clicked, this, &HomeWidget::reactorClicked);
    connect(ui->experimentManagerPushButton, &QPushButton::clicked, this, &HomeWidget::expManagerClicked);
    connect(ui->userManagementPushButton, &QPushButton::clicked, this, &HomeWidget::userManagerClicked);
    connect(ui->optionsPushButton, &QPushButton::clicked, this, &HomeWidget::optionsClicked);
    connect(ui->closePushButton, &QPushButton::clicked, this, &HomeWidget::closeClicked);

}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::reactorClicked()
{
    emit sgn_openReactorWidget();
}

void HomeWidget::expManagerClicked()
{
    emit sgn_openExpManagerWidget();
}

void HomeWidget::userManagerClicked()
{
    emit sgn_openUserManagerWidget();
}

void HomeWidget::optionsClicked()
{
    emit sgn_openOptionsWidget();
}

void HomeWidget::closeClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, tr("Close"), tr("Are you sure you want to close the software?"),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit sgn_closeApp();

    }
}
