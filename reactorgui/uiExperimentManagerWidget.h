#ifndef UIEXPERIMENTMANAGERWIDGET_H
#define UIEXPERIMENTMANAGERWIDGET_H

#include <QWidget>

#include "uiExperimentListWidget.h"
#include "uiExperimentCreateWidget.h"
#include "User.h"

namespace Ui {
class ExperimentManagerWidget;
}

class ExperimentManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentManagerWidget(QWidget *parent = nullptr);
    ~ExperimentManagerWidget();
    void setCurrentUser(const User &user);

private:
    Ui::ExperimentManagerWidget *ui;
    ExperimentListWidget *m_experimentListWidget = nullptr;
    User m_currentUser;

    void updateActionButtons();
    bool canModifyExperiment(const Experiment &experiment) const;

private slots:
    void createClicked();
    void showListWidget();
    void editClicked();
    void deleteClicked();
    void handleSelectionChanged();
    void handleExperimentSaved();
    void handleExperimentDoubleClicked(const Experiment &experiment);
    void clearMainLayout();


};

#endif // UIEXPERIMENTMANAGERWIDGET_H
