#ifndef UIOPTIONSWIDGET_H
#define UIOPTIONSWIDGET_H

#include <QWidget>

namespace Ui {
class OptionsWidget;
}

class OptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsWidget(QWidget *parent = nullptr);
    ~OptionsWidget();

private:
    Ui::OptionsWidget *ui;
};

#endif // UIOPTIONSWIDGET_H
