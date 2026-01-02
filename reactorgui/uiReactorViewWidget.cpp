#include "uiReactorViewWidget.h"
#include "ui_uiReactorViewWidget.h"
#include "RedisDBManager.h"

ReactorViewWidget::ReactorViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReactorViewWidget)
{
    ui->setupUi(this);

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    m_flowTimer = new QTimer(this);
    connect(m_flowTimer, &QTimer::timeout, this, &ReactorViewWidget::updateFlowStatus);
    m_flowTimer->start(1000);
}

ReactorViewWidget::~ReactorViewWidget()
{
    delete ui;
}

void ReactorViewWidget::updateFlowStatus()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        return;
    }

    FlowStatus flowStatus = RedisDBManager::getInstance()->getFlowStatus();

    if (flowStatus.timestamp() == 0) {
        ui->flowRateValueLabel->setText("--");
        ui->flowTempValueLabel->setText("--");
        return;
    }

    ui->flowRateValueLabel->setText(QString::number(flowStatus.flowRateLpm(), 'f', 2));
    ui->flowTempValueLabel->setText(QString::number(flowStatus.flowTemp(), 'f', 1));
}
