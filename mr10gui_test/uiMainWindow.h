#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <string>
#include <cpp_redis/cpp_redis>
#include "../experimentmanagerservice/src/common/RedisDBManager.h"
#include "../experimentmanagerservice/src/common/Cell.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<Cell> m_cells;
    QTimer *m_updateTimer = nullptr;
    std::string m_busboardId;

    double m_prevVal = 0;
    unsigned long m_prevTime = 0;


private slots:
    void updateTable();
    void sendTargetsClicked();
};
#endif // MAINWINDOW_H
