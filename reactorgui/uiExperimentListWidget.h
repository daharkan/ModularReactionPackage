#ifndef UIEXPERIMENTLISTWIDGET_H
#define UIEXPERIMENTLISTWIDGET_H

#include <QWidget>

namespace Ui {
class ExperimentListWidget;
}

class ExperimentListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentListWidget(QWidget *parent = nullptr);
    ~ExperimentListWidget();

private:
    Ui::ExperimentListWidget *ui;

private slots:
    void filterTableByLetter();
    void filterTable(const QString &filterText);
    void filterTableBySearch(const QString &text);
    void listAllData();

};

#endif // UIEXPERIMENTLISTWIDGET_H
