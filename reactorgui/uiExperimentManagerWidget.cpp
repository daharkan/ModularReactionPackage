#include "uiExperimentManagerWidget.h"
#include "ui_uiExperimentManagerWidget.h"
#include "RedisDBManager.h"
#include <QMessageBox>


ExperimentManagerWidget::ExperimentManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentManagerWidget)
{
    ui->setupUi(this);
    m_experimentListWidget = new ExperimentListWidget(this);
    showListWidget();

    connect(ui->createPushButton, &QPushButton::clicked, this, &ExperimentManagerWidget::createClicked);
    connect(ui->listExperimentsPushButton, &QPushButton::clicked, this, &ExperimentManagerWidget::showListWidget);
    connect(ui->edtExperimentPushButton, &QPushButton::clicked, this, &ExperimentManagerWidget::editClicked);
    connect(ui->delExperimentPushButton, &QPushButton::clicked, this, &ExperimentManagerWidget::deleteClicked);
    connect(m_experimentListWidget, &ExperimentListWidget::sgn_selectionChanged, this, &ExperimentManagerWidget::handleSelectionChanged);
    connect(m_experimentListWidget, &ExperimentListWidget::sgn_experimentActivated, this, &ExperimentManagerWidget::editClicked);

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }

}

ExperimentManagerWidget::~ExperimentManagerWidget()
{
    delete ui;
}

void ExperimentManagerWidget::setCurrentUser(const User &user)
{
    m_currentUser = user;
    updateActionButtons();
}

void ExperimentManagerWidget::createClicked()
{
    clearMainLayout();
    ExperimentCreateWidget *createWid = new ExperimentCreateWidget(this);
    createWid->setCurrentUser(m_currentUser);
    ui->mainGridLayout->addWidget(createWid);
    ui->listExperimentsPushButton->setVisible(true);
    ui->createPushButton->setVisible(false);
    ui->delExperimentPushButton->setVisible(false);
    ui->edtExperimentPushButton->setVisible(false);

    connect(createWid, &ExperimentCreateWidget::sgn_experimentSaved, this, &ExperimentManagerWidget::handleExperimentSaved);
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
    m_experimentListWidget->reloadExperiments();
    ui->listExperimentsPushButton->setVisible(false);
    ui->createPushButton->setVisible(true);
    ui->edtExperimentPushButton->setVisible(true);
    ui->delExperimentPushButton->setVisible(true);
    updateActionButtons();
}

void ExperimentManagerWidget::editClicked()
{
    if (!m_experimentListWidget->hasSelection()) {
        return;
    }

    Experiment selected = m_experimentListWidget->selectedExperiment();
    if (selected.experimentId().empty()) {
        return;
    }

    clearMainLayout();
    ExperimentCreateWidget *createWid = new ExperimentCreateWidget(this);
    createWid->setCurrentUser(m_currentUser);

    ExperimentCreateWidget::Mode mode = canModifyExperiment(selected) ? ExperimentCreateWidget::Mode::Edit : ExperimentCreateWidget::Mode::Show;
    createWid->loadExperiment(selected, mode);
    ui->mainGridLayout->addWidget(createWid);
    ui->listExperimentsPushButton->setVisible(true);
    ui->createPushButton->setVisible(false);
    ui->delExperimentPushButton->setVisible(false);
    ui->edtExperimentPushButton->setVisible(false);

    connect(createWid, &ExperimentCreateWidget::sgn_experimentSaved, this, &ExperimentManagerWidget::handleExperimentSaved);
}

void ExperimentManagerWidget::deleteClicked()
{
    if (!m_experimentListWidget->hasSelection()) {
        return;
    }

    Experiment selected = m_experimentListWidget->selectedExperiment();
    if (!canModifyExperiment(selected)) {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Delete Experiment"),
        tr("Delete the selected experiment?\nThis action cannot be undone."),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    RedisDBManager::getInstance()->deleteExperiment(selected.experimentId());
    m_experimentListWidget->reloadExperiments();
    updateActionButtons();
}

void ExperimentManagerWidget::handleSelectionChanged()
{
    updateActionButtons();
}

void ExperimentManagerWidget::handleExperimentSaved()
{
    if (m_experimentListWidget) {
        m_experimentListWidget->reloadExperiments();
    }
}

void ExperimentManagerWidget::updateActionButtons()
{
    if (!m_experimentListWidget) {
        return;
    }

    bool hasSelection = m_experimentListWidget->hasSelection();
    ui->edtExperimentPushButton->setEnabled(hasSelection);

    bool canModify = false;
    if (hasSelection) {
        canModify = canModifyExperiment(m_experimentListWidget->selectedExperiment());
    }
    ui->delExperimentPushButton->setEnabled(canModify);
}

bool ExperimentManagerWidget::canModifyExperiment(const Experiment &experiment) const
{
    if (experiment.owner().username().empty()) {
        return false;
    }
    if (m_currentUser.role() == ROLE_ADMIN || m_currentUser.role() == ROLE_ROOT) {
        return true;
    }
    return experiment.owner().username() == m_currentUser.username();
}
