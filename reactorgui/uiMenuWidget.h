#ifndef UIMENUWIDGET_H
#define UIMENUWIDGET_H

#include <QWidget>

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

    void setTitle(const QString &title);
    void setBackEnabled(bool enabled);

signals:
    void sgn_backClicked();
    void sgn_reactorClicked();
    void sgn_experimentClicked();
    void sgn_userClicked();
    void sgn_optionsClicked();

private:
    Ui::MenuWidget *ui;
};

#endif // UIMENUWIDGET_H
