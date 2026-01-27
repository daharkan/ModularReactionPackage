#include "uiMenuWidget.h"
#include "ui_uiMenuWidget.h"

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    ui->titleLabel->setText(QString());

    connect(ui->backButton, &QPushButton::clicked, this, &MenuWidget::sgn_backClicked);
    connect(ui->reactorButton, &QPushButton::clicked, this, &MenuWidget::sgn_reactorClicked);
    connect(ui->experimentButton, &QPushButton::clicked, this, &MenuWidget::sgn_experimentClicked);
    connect(ui->userButton, &QPushButton::clicked, this, &MenuWidget::sgn_userClicked);
    connect(ui->optionsButton, &QPushButton::clicked, this, &MenuWidget::sgn_optionsClicked);
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::setTitle(const QString &title)
{
    ui->titleLabel->setText(title);
}

void MenuWidget::setBackEnabled(bool enabled)
{
    ui->backButton->setEnabled(enabled);
}
