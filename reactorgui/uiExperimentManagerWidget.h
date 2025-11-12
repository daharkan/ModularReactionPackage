#ifndef UIEXPERIMENTMANAGERWIDGET_H
#define UIEXPERIMENTMANAGERWIDGET_H

#include <QWidget>

#include "uiExperimentListWidget.h"
#include "uiExperimentCreateWidget.h"

namespace Ui {
class ExperimentManagerWidget;
}

class ExperimentManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentManagerWidget(QWidget *parent = nullptr);
    ~ExperimentManagerWidget();

private:
    Ui::ExperimentManagerWidget *ui;
    ExperimentListWidget *m_experimentListWidget = nullptr;

    void clearMainLayout();

private slots:
    void createClicked();
    void showListWidget();


};

#endif // UIEXPERIMENTMANAGERWIDGET_H
