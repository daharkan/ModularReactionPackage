#include "uiTSMainWindow.h"
#include "ui_uiTSMainWindow.h"

#define IS_MACHINE_CONNECTED false

#define MAX_HISTORY_SIZE 200

TSMainWindow::TSMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TSMainWindow)
{
    ui->setupUi(this);

    QLocale::setDefault(QLocale::C);

    connect(ui->homePushButton, &QPushButton::clicked, this, &TSMainWindow::showHomeWidget);
    connect(ui->backPushButton, &QPushButton::clicked, this, &TSMainWindow::showPreviousPage);
    connect(ui->mainStackedWidget, &QStackedWidget::currentChanged, this, &TSMainWindow::mainPageChanged);


    m_loginWidget = new LoginWidget(IS_MACHINE_CONNECTED, this);
    m_homeWidget = new HomeWidget(this);
    m_optionsWidget = new OptionsWidget(this);
    m_userManagementWidget = new UserManagementWidget(this);
    m_reactorViewWidget = new ReactorViewWidget(this);
    m_experimentManagerWidget = new ExperimentManagerWidget(this);
    m_tempCellViewWidget = new TempCellViewWidget(this);

    ui->mainStackedWidget->addWidget(m_loginWidget);
    ui->mainStackedWidget->addWidget(m_homeWidget);
    ui->mainStackedWidget->addWidget(m_optionsWidget);
    ui->mainStackedWidget->addWidget(m_userManagementWidget);
    ui->mainStackedWidget->addWidget(m_reactorViewWidget);
    ui->mainStackedWidget->addWidget(m_experimentManagerWidget);
    ui->mainStackedWidget->addWidget(m_tempCellViewWidget);

    connect(m_loginWidget, &LoginWidget::sgn_loginSucceed, this, &TSMainWindow::loginSucceed);
    connect(m_loginWidget, &LoginWidget::sgn_loginFailed, this, &TSMainWindow::loginFailed);

    connect(m_homeWidget, &HomeWidget::sgn_openReactorWidget, this, &TSMainWindow::showReactorWidget);
    connect(m_homeWidget, &HomeWidget::sgn_openExpManagerWidget, this, &TSMainWindow::showExpManagerWidget);
    connect(m_homeWidget, &HomeWidget::sgn_openUserManagerWidget, this, &TSMainWindow::showUserManagementWidget);
    connect(m_homeWidget, &HomeWidget::sgn_openOptionsWidget, this, &TSMainWindow::showOptionsWidget);
    connect(m_homeWidget, &HomeWidget::sgn_closeApp, this, &TSMainWindow::closeApp);
    connect(m_reactorViewWidget, &ReactorViewWidget::sgn_openCellView, this, &TSMainWindow::showCellViewWidget);


    ui->backPushButton->setVisible(false);
    ui->homePushButton->setVisible(false);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_LOGIN);


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
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_HOME;
    m_pageTypeHistory.push_back(m_currentPageType);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_HOME);
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

void TSMainWindow::showCellViewWidget(const std::string &cellId)
{
    clearOlderHistory();
    m_currentPageType = PAGE_TYPE_CELLVIEW;
    m_pageTypeHistory.push_back(m_currentPageType);
    m_tempCellViewWidget->setCellId(cellId);
    ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_CELLVIEW);
}

void TSMainWindow::closeApp()
{
    close();
}

void TSMainWindow::showPreviousPage()
{
    if(m_pageTypeHistory.size() > 0 ){
        m_pageTypeHistory.pop_back();
        m_currentPageType = m_pageTypeHistory.last();
    }

    switch (m_currentPageType) {
    case PAGE_TYPE_HOME:
        ui->mainStackedWidget->setCurrentIndex(WIDGET_IDX_HOME);
        break;

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
    case WIDGET_IDX_HOME:
        ui->pageHeadlineLabel->setText("HOME");
        break;

    case WIDGET_IDX_EXPMANAGER:
        ui->pageHeadlineLabel->setText("EXPERIMENT MANAGER");
        break;

    case WIDGET_IDX_OPTIONS:
        ui->pageHeadlineLabel->setText("OPTIONS");
        break;

    case WIDGET_IDX_REACTOR:
        ui->pageHeadlineLabel->setText("REACTOR");
        break;

    case WIDGET_IDX_USERMANAGEMENT:
        ui->pageHeadlineLabel->setText("USER MANAGEMENT");
        break;

    case WIDGET_IDX_CELLVIEW:
        ui->pageHeadlineLabel->setText("CELL DETAIL");
        break;


    default:
        break;
    }
}

void TSMainWindow::loginSucceed()
{
    m_currentUser = m_loginWidget->currentUser();
    m_experimentManagerWidget->setCurrentUser(m_currentUser);
    m_userManagementWidget->setCurrentUser(m_currentUser);
    ui->backPushButton->setVisible(true);
    ui->homePushButton->setVisible(true);
    showHomeWidget();
}

void TSMainWindow::loginFailed()
{

}
