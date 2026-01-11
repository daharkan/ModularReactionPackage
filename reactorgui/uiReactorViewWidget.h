#ifndef UIREACTORVIEWWIDGET_H
#define UIREACTORVIEWWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <string>
#include "uiCellOverviewWidget.h"

namespace Ui {
class ReactorViewWidget;
}

class ReactorViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReactorViewWidget(QWidget *parent = nullptr);
    ~ReactorViewWidget();

private slots:
    void updateFlowStatus();
    void handleCellClicked(const std::string &cellId);

private:
    Ui::ReactorViewWidget *ui;
    QTimer *m_flowTimer = nullptr;
    QVector<CellOverviewWidget*> m_lhsCells;
    QVector<CellOverviewWidget*> m_rhsCells;
    std::string m_lhsBusboardId;
    std::string m_rhsBusboardId;

    void setupCellOverviewWidgets();
    void refreshBusboardIds();
    void updateCellGroup(const std::string &busboardId, QVector<CellOverviewWidget*> &widgets, const QString &sideLabel);
    FlowStatus selectActiveFlowStatus() const;
};

#endif // UIREACTORVIEWWIDGET_H
