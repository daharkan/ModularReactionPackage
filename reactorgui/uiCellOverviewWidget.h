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
    void setSelected(bool selected);
    std::string cellId() const;

signals:
    void sgn_cellClicked(const std::string &cellId, int positionIndex);
    void sgn_cellSelected(const std::string &cellId, int positionIndex);
    void sgn_assignExperimentRequested(const std::string &cellId, int positionIndex);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    Ui::CellOverviewWidget *ui;
    std::string m_cellId;
    QString m_sideLabel;
    int m_slotIndex = 0;
    bool m_isActive = false;
    bool m_isSelected = false;
    QString m_stateKey;

    void updateHeader();
    void applyActiveStyle(bool active);
    void applyStateStyle(const QString &stateKey);
};

#endif // UICELLOVERVIEWWIDGET_H
