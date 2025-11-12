#ifndef UIHOMEWIDGET_H
#define UIHOMEWIDGET_H

#include <QWidget>

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

private:
    Ui::HomeWidget *ui;

private slots:
    void reactorClicked();
    void expManagerClicked();
    void userManagerClicked();
    void optionsClicked();
    void closeClicked();

signals:
    void sgn_openReactorWidget();
    void sgn_openExpManagerWidget();
    void sgn_openUserManagerWidget();
    void sgn_openOptionsWidget();
    void sgn_closeApp();


};

#endif // UIHOMEWIDGET_H
