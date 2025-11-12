#include "uiOptionsWidget.h"
#include "ui_uiOptionsWidget.h"

OptionsWidget::OptionsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}
