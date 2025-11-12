#ifndef UIREACTORVIEWWIDGET_H
#define UIREACTORVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class ReactorViewWidget;
}

class ReactorViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReactorViewWidget(QWidget *parent = nullptr);
    ~ReactorViewWidget();

private:
    Ui::ReactorViewWidget *ui;
};

#endif // UIREACTORVIEWWIDGET_H
