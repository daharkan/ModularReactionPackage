#ifndef UICELLOVERVIEWWIDGET_H
#define UICELLOVERVIEWWIDGET_H

#include <QWidget>
#include "Cell.h"

namespace Ui {
class CellOverviewWidget;
}

class CellOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellOverviewWidget(QWidget *parent = nullptr);
    ~CellOverviewWidget();

    void setSlotInfo(const QString &sideLabel, int slotIndex);
    void setCellData(const Cell &cell);
    void setInactive();
    std::string cellId() const;

signals:
    void sgn_cellClicked(const std::string &cellId, int positionIndex);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::CellOverviewWidget *ui;
    std::string m_cellId;
    QString m_sideLabel;
    int m_slotIndex = 0;
    bool m_isActive = false;

    void updateHeader();
    void applyActiveStyle(bool active);
    void applyStateStyle(const QString &stateKey);
};

#endif // UICELLOVERVIEWWIDGET_H
