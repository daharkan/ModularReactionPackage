#include "uiReactorViewWidget.h"
#include "ui_uiReactorViewWidget.h"

ReactorViewWidget::ReactorViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReactorViewWidget)
{
    ui->setupUi(this);
}

ReactorViewWidget::~ReactorViewWidget()
{
    delete ui;
}
