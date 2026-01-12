#ifndef TSMAINWINDOW_H
#define TSMAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QObject>

#include "uiLoginWidget.h"
#include "uiExperimentManagerWidget.h"
#include "uiOptionsWidget.h"
#include "uiReactorViewWidget.h"
#include "uiUserManagementWidget.h"
#include "uiHomeWidget.h"
#include "uiTempCellViewWidget.h"

#define WIDGET_IDX_LOGIN                0
#define WIDGET_IDX_HOME                 1
#define WIDGET_IDX_OPTIONS              2
#define WIDGET_IDX_USERMANAGEMENT       3
#define WIDGET_IDX_REACTOR              4
#define WIDGET_IDX_EXPMANAGER           5
#define WIDGET_IDX_CELLVIEW             6


QT_BEGIN_NAMESPACE
namespace Ui {
class TSMainWindow;
}
QT_END_NAMESPACE


enum PageType{
    PAGE_TYPE_UNKNOWN,
    PAGE_TYPE_HOME,
    PAGE_TYPE_LOGIN,
    PAGE_TYPE_OPTIONS,
    PAGE_TYPE_REACTORVIEW,
    PAGE_TYPE_CELLVIEW,
    PAGE_TYPE_EXPERIMENTMANAGER,
    PAGE_TYPE_USERMANAGEMENT
};



class TSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    TSMainWindow(QWidget *parent = nullptr);
    ~TSMainWindow();

    void setCurrentPageType(PageType type){
        m_currentPageType = type;
    };

private:
    Ui::TSMainWindow *ui;
    
    LoginWidget *m_loginWidget = nullptr;
    HomeWidget *m_homeWidget = nullptr;
    OptionsWidget *m_optionsWidget = nullptr;
    UserManagementWidget *m_userManagementWidget = nullptr;
    ReactorViewWidget *m_reactorViewWidget = nullptr;
    ExperimentManagerWidget *m_experimentManagerWidget = nullptr;
    TempCellViewWidget *m_tempCellViewWidget = nullptr;
    User m_currentUser;


    PageType m_currentPageType = PAGE_TYPE_UNKNOWN;
    QList<PageType> m_pageTypeHistory;


    void clearOlderHistory();


private slots:
    void loginSucceed();
    void loginFailed();

    void showHomeWidget();
    void showReactorWidget();
    void showExpManagerWidget();
    void showUserManagementWidget();
    void showOptionsWidget();
    void showCellViewWidget(const std::string &cellId, int positionIndex);
    void closeApp();

    void showPreviousPage();
    void mainPageChanged(int idx);


};
#endif // TSMAINWINDOW_H
