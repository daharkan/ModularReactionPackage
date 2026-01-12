#ifndef UIOptionsWIDGET_H
#define UIOptionsWIDGET_H

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

signals:
    void sgn_menuPositionChanged(bool isTop);

private:
    Ui::OptionsWidget *ui;
    QWidget *m_parent;

private slots:
    void applyButtonClicked();
};

#endif // UIOptionsWIDGET_H
