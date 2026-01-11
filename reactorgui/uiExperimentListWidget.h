#ifndef UIEXPERIMENTLISTWIDGET_H
#define UIEXPERIMENTLISTWIDGET_H

#include <QTableWidgetItem>
#include <QWidget>
#include <vector>
#include "Experiment.h"

namespace Ui {
class ExperimentListWidget;
}

class ExperimentListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentListWidget(QWidget *parent = nullptr);
    ~ExperimentListWidget();
    void reloadExperiments();
    QString selectedExperimentId() const;
    Experiment selectedExperiment() const;
    bool hasSelection() const;

private:
    Ui::ExperimentListWidget *ui;
    std::vector<Experiment> m_experiments;

private slots:
    void filterTableByLetter();
    void filterTable(const QString &filterText);
    void filterTableBySearch(const QString &text);
    void listAllData();
    void handleItemDoubleClicked(QTableWidgetItem *item);

signals:
    void sgn_selectionChanged();
    void sgn_experimentActivated();

};

#endif // UIEXPERIMENTLISTWIDGET_H
