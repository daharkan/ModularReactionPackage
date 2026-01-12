#ifndef UITEMPCELLVIEWWIDGET_H
#define UITEMPCELLVIEWWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QVector>
#include <QTimer>
#include <string>

#include "uiCellWidget.h"

class TempCellViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TempCellViewWidget(QWidget *parent = nullptr);
    ~TempCellViewWidget();

    void setCellInfo(const std::string &cellId, int positionIndex);
    std::string cellId() const;
    int positionIndex() const;

private slots:
    void refreshCellData();

private:
    QStackedLayout *m_stackLayout = nullptr;
    QVector<CellWidget*> m_cellWidgets;
    QTimer *m_updateTimer = nullptr;
    std::string m_cellId;
    int m_positionIndex = 0;

    void ensureRedisConnection();
    CellWidget *cellWidgetForPosition(int positionIndex) const;
};

#endif // UITEMPCELLVIEWWIDGET_H
