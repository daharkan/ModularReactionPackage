#ifndef UIREACTORVIEWWIDGET_H
#define UIREACTORVIEWWIDGET_H

#include <QWidget>
#include <QTimer>
#include <string>

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
    std::string m_busboardId;
};

#endif // UIREACTORVIEWWIDGET_H
