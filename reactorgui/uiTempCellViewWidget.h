#ifndef UITEMPCELLVIEWWIDGET_H
#define UITEMPCELLVIEWWIDGET_H

#include <QWidget>
#include <QTimer>
#include <string>

#include "uiCellWidget.h"

class TempCellViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TempCellViewWidget(QWidget *parent = nullptr);
    ~TempCellViewWidget();

    void setCellId(const std::string &cellId);
    std::string cellId() const;

private slots:
    void refreshCellData();

private:
    CellWidget *m_cellWidget = nullptr;
    QTimer *m_updateTimer = nullptr;
    std::string m_cellId;

    void ensureRedisConnection();
};

#endif // UITEMPCELLVIEWWIDGET_H
