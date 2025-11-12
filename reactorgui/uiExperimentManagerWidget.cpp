#include "uiExperimentManagerWidget.h"
#include "ui_uiExperimentManagerWidget.h"


ExperimentManagerWidget::ExperimentManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentManagerWidget)
{
    ui->setupUi(this);
    m_experimentListWidget = new ExperimentListWidget(this);
    showListWidget();

    connect(ui->createPushButton, &QPushButton::clicked, this, &ExperimentManagerWidget::createClicked);
    connect(ui->listExperimentsPushButton, &QPushButton::clicked, this, &ExperimentManagerWidget::showListWidget);

}

ExperimentManagerWidget::~ExperimentManagerWidget()
{
    delete ui;
}

void ExperimentManagerWidget::createClicked()
{
    clearMainLayout();
    ExperimentCreateWidget *createWid = new ExperimentCreateWidget(this);
    ui->mainGridLayout->addWidget(createWid);
    ui->listExperimentsPushButton->setVisible(true);
    ui->createPushButton->setVisible(false);
    ui->delExperimentPushButton->setVisible(false);
    ui->edtExperimentPushButton->setVisible(false);
}



void ExperimentManagerWidget::clearMainLayout() {
    QLayoutItem *item;
    while ((item = ui->mainGridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->hide();
        }
        delete item;
    }
}

void ExperimentManagerWidget::showListWidget()
{
    clearMainLayout();
    ui->mainGridLayout->addWidget(m_experimentListWidget);
    m_experimentListWidget->show();
    ui->listExperimentsPushButton->setVisible(false);
    ui->createPushButton->setVisible(true);
    ui->edtExperimentPushButton->setVisible(true);
    ui->delExperimentPushButton->setVisible(true);
}
