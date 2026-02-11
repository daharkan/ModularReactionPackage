#include "uiTSMainWindow.h"
#include "ui_uiTSMainWindow.h"

#include "RedisDBManager.h"
#include "uiExperimentListWidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>

#define IS_MACHINE_CONNECTED true

#define MAX_HISTORY_SIZE 200

namespace {
const char *kUiConfigPath = "conf/reactorgui.conf";

QString readConfigValue(const QString &key)
{
    QString target = key.trimmed().toLower();
    QFile file(QString::fromUtf8(kUiConfigPath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#') || line.startsWith(';')) {
            continue;
        }
        int idx = line.indexOf('=');
        if (idx <= 0) {
            continue;
        }
        QString foundKey = line.left(idx).trimmed().toLower();
        if (foundKey != target) {
            continue;
        }
        return line.mid(idx + 1).trimmed();
    }
    return QString();
}

QString normalizeMenuPosition(const QString &value)
{
    QString lower = value.trimmed().toLower();
    if (lower == "bottom") {
        return "bottom";
    }
    if (lower == "top") {
        return "top";
    }
    return QString();
}
} // namespace

TSMainWindow::TSMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TSMainWindow)
{
    ui->setupUi(this);

    QLocale::setDefault(QLocale::C);

    connect(ui->mainStackedWidget, &QStackedWidget::currentChanged, this, &TSMainWindow::mainPageChanged);

    m_menuWidget = new MenuWidget(this);
    ui->menuContainerLayout->addWidget(m_menuWidget);
    connect(m_menuWidget, &MenuWidget::sgn_backClicked, this, &TSMainWindow::showPreviousPage);
    connect(m_menuWidget, &MenuWidget::sgn_reactorClicked, this, &TSMainWindow::showReactorWidget);
    connect(m_menuWidget, &MenuWidget::sgn_experimentClicked, this, &TSMainWindow::showExpManagerWidget);
    connect(m_menuWidget, &MenuWidget::sgn_userClicked, this, &TSMainWindow::showUserManagementWidget);
    connect(m_menuWidget, &MenuWidget::sgn_optionsClicked, this, &TSMainWindow::showOptionsWidget);

    m_loginWidget = new LoginWidget(IS_MACHINE_CONNECTED, this);
    m_optionsWidget = new OptionsWidget(this);
    m_userManagementWidget = new UserManagementWidget(this);
    m_reactorViewWidget = new ReactorViewWidget(this);
    m_experimentManagerWidget = new ExperimentManagerWidget(this);
    m_tempCellViewWidget = new TempCellViewWidget(this);

    ui->mainStackedWidget->addWidget(m_loginWidget);
    ui->mainStackedWidget->addWidget(m_optionsWidget);
    ui->mainStackedWidget->addWidget(m_userManagementWidget);
    ui->mainStackedWidget->addWidget(m_reactorViewWidget);
    ui->mainStackedWidget->addWidget(m_experimentManagerWidget);
    ui->mainStackedWidget->addWidget(m_tempCellViewWidget);

    connect(m_loginWidget, &LoginWidget::sgn_loginSucceed, this, &TSMainWindow::loginSucceed);
    connect(m_loginWidget, &LoginWidget::sgn_loginFailed, this, &TSMainWindow::loginFailed);

    connect(m_reactorViewWidget, &ReactorViewWidget::sgn_openCellView, this, &TSMainWindow::showCellViewWidget);
    connect(m_reactorViewWidget, &ReactorViewWidget::sgn_assignExperimentRequested,
            this, &TSMainWindow::handleAssignExperimentRequested);
    connect(m_tempCellViewWidget, &TempCellViewWidget::sgn_assignExperimentRequested,
            this, [this](const std::string &cellId) {
                handleAssignExperimentRequested(cellId, 0);
            });
    connect(m_optionsWidget, &OptionsWidget::sgn_menuPositionChanged, this, &TSMainWindow::handleMenuPositionChanged);


    ui->menuContainer->setVisible(false);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_LOGIN);
    m_currentPageType = PAGE_TYPE_LOGIN;
    loadMenuPosition();


}

TSMainWindow::~TSMainWindow()
{
    delete ui;
}


void TSMainWindow::clearOlderHistory() {
    if (m_pageTypeHistory.size() > MAX_HISTORY_SIZE) {
        m_pageTypeHistory.erase(m_pageTypeHistory.begin(), m_pageTypeHistory.begin() + MAX_HISTORY_SIZE/2);
    }
}

void TSMainWindow::showHomeWidget()
{
    m_pageTypeHistory.clear();
    showReactorWidget();
}

void TSMainWindow::showReactorWidget()
{
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_REACTORVIEW;
    m_pageTypeHistory.push_back(m_currentPageType);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_REACTOR);
}

void TSMainWindow::showExpManagerWidget()
{
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_EXPERIMENTMANAGER;
    m_pageTypeHistory.push_back(m_currentPageType);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_EXPMANAGER);
}

void TSMainWindow::showUserManagementWidget()
{
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_USERMANAGEMENT;
    m_pageTypeHistory.push_back(m_currentPageType);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_USERMANAGEMENT);
}

void TSMainWindow::showOptionsWidget()
{
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_OPTIONS;
    m_pageTypeHistory.push_back(m_currentPageType);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_OPTIONS);
}

void TSMainWindow::showCellViewWidget(const std::string &cellId, int positionIndex)
{
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_CELLVIEW;
    m_pageTypeHistory.push_back(m_currentPageType);
    m_tempCellViewWidget->setCellInfo(cellId, positionIndex);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_CELLVIEW);
}

void TSMainWindow::closeApp()
{
    close();
}

void TSMainWindow::showPreviousPage()
{
    if(m_pageTypeHistory.size() > 1 ){
        m_pageTypeHistory.pop_back();
        m_currentPageType = m_pageTypeHistory.last();
    } else {
        if (m_menuWidget != nullptr) {
            m_menuWidget->setBackEnabled(false);
        }
        return;
    }

    switch (m_currentPageType) {
    case PAGE_TYPE_EXPERIMENTMANAGER:
        ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_EXPMANAGER);
        break;

    case PAGE_TYPE_OPTIONS:
        ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_OPTIONS);
        break;

    case PAGE_TYPE_REACTORVIEW:
        ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_REACTOR);
        break;

    case PAGE_TYPE_USERMANAGEMENT:
        ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_USERMANAGEMENT);
        break;

    case PAGE_TYPE_CELLVIEW:
        ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_CELLVIEW);
        break;

    default:
        break;
    }
}

void TSMainWindow::mainPageChanged(int idx)
{
    switch(idx){
    case WIDGET_IDX_EXPMANAGER:
        m_menuWidget->setTitle("Experiment Manager");
        break;

    case WIDGET_IDX_OPTIONS:
        m_menuWidget->setTitle("Options");
        break;

    case WIDGET_IDX_REACTOR:
        m_menuWidget->setTitle("Reactor");
        break;

    case WIDGET_IDX_USERMANAGEMENT:
        m_menuWidget->setTitle("User Management");
        break;

    case WIDGET_IDX_CELLVIEW:
        m_menuWidget->setTitle("Cell Detail");
        break;

    case WIDGET_IDX_LOGIN:
        m_menuWidget->setTitle("Login");
        break;


    default:
        m_menuWidget->setTitle(QString());
        break;
    }
    if (m_menuWidget != nullptr) {
        m_menuWidget->setBackEnabled(m_pageTypeHistory.size() > 1);
    }
}

void TSMainWindow::loginSucceed()
{
    m_currentUser = m_loginWidget->currentUser();
    m_experimentManagerWidget->setCurrentUser(m_currentUser);
    m_userManagementWidget->setCurrentUser(m_currentUser);
    m_reactorViewWidget->setMachineId(m_loginWidget->selectedMachineId());
    ui->menuContainer->setVisible(true);
    showHomeWidget();
}

void TSMainWindow::loginFailed()
{

}

void TSMainWindow::applyMenuPosition(bool isTop)
{
    if (m_menuTop == isTop) {
        return;
    }
    m_menuTop = isTop;
    ui->mainLayout->removeWidget(ui->menuContainer);
    if (m_menuTop) {
        ui->mainLayout->insertWidget(0, ui->menuContainer);
    } else {
        ui->mainLayout->addWidget(ui->menuContainer);
    }
}

void TSMainWindow::loadMenuPosition()
{
    QString menuPosition = normalizeMenuPosition(readConfigValue("menu_position"));
    applyMenuPosition(menuPosition != "bottom");
}

void TSMainWindow::handleMenuPositionChanged(bool isTop)
{
    applyMenuPosition(isTop);
}

void TSMainWindow::handleAssignExperimentRequested(const std::string &cellId, int positionIndex)
{
    Q_UNUSED(positionIndex);
    assignExperimentToCell(cellId);
}

void TSMainWindow::assignExperimentToCell(const std::string &cellId)
{
    if (cellId.empty()) {
        return;
    }

    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDefault();
    }
    if (!RedisDBManager::getInstance()->isConnected()) {
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Assign Experiment"));
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    auto *listWidget = new ExperimentListWidget(&dialog);
    layout->addWidget(listWidget);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QPushButton *assignButton = buttons->button(QDialogButtonBox::Ok);
    if (assignButton != nullptr) {
        assignButton->setText(tr("Assign"));
        assignButton->setEnabled(listWidget->hasSelection());
    }
    layout->addWidget(buttons);

    connect(listWidget, &ExperimentListWidget::sgn_selectionChanged, &dialog, [listWidget, assignButton]() {
        if (assignButton != nullptr) {
            assignButton->setEnabled(listWidget->hasSelection());
        }
    });
    connect(listWidget, &ExperimentListWidget::sgn_experimentActivated, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    if (!listWidget->hasSelection()) {
        return;
    }

    Experiment selectedExperiment = listWidget->selectedExperiment();
    if (selectedExperiment.experimentId().empty()) {
        return;
    }

    std::vector<std::string> ids = {cellId};
    std::vector<Cell> cells = RedisDBManager::getInstance()->getCellList(ids);
    if (cells.empty()) {
        return;
    }

    Cell cell = cells.front();
    bool isRunning = QString::fromStdString(cell.asignedExperiment().state())
        .compare("running", Qt::CaseInsensitive) == 0;
    QString confirmText = tr("Assign \"%1\" to this cell?")
                              .arg(QString::fromStdString(selectedExperiment.name()));
    confirmText.append(tr("\n\nThis will overwrite any existing assignment."));
    if (isRunning) {
        confirmText.append(tr("\n\nWarning: This cell is currently running an experiment."));
    }

    QMessageBox::StandardButton confirm = QMessageBox::question(
        this,
        tr("Assign Experiment"),
        confirmText,
        QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes) {
        return;
    }

    Experiment assignedExperiment = selectedExperiment;
    assignedExperiment.setOwner(m_currentUser);
    assignedExperiment.setStartSystemTimeMSecs(0);
    cell.setAsignedExperiment(assignedExperiment);
    RedisDBManager::getInstance()->pushCellAssignments({cell});
}
