#ifndef UIREACTORVIEWWIDGET_H
#define UIREACTORVIEWWIDGET_H

#include <QWidget>
#include <QTimer>

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

private:
    Ui::ReactorViewWidget *ui;
    QTimer *m_flowTimer = nullptr;
};

#endif // UIREACTORVIEWWIDGET_H
