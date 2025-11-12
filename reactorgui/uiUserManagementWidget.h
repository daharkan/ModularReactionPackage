#ifndef UIUSERMANAGEMENTWIDGET_H
#define UIUSERMANAGEMENTWIDGET_H

#include <QWidget>

namespace Ui {
class UserManagementWidget;
}

class UserManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserManagementWidget(QWidget *parent = nullptr);
    ~UserManagementWidget();

private:
    Ui::UserManagementWidget *ui;
};

#endif // UIUSERMANAGEMENTWIDGET_H
