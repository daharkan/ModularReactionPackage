#ifndef MAINWINDOWW_H
#define MAINWINDOWW_H

#include <QMainWindow>
#include "uiCellWidget.h"
#include "OnlyCellSerialManager.h"


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
    CellWidget *m_cellWidget;

    TempArc createLinearTempArc(float startTemp, float endTemp, float rank, unsigned long startTimeMSec);
    TempArc createPlateuTempArc(float startTemp, unsigned long startTimeMSec, unsigned long durationMSec);
    Profile createBasicStandardProfile(int type);

private slots:
    void assignExp1Clicked();
    void assignExp2Clicked();
    void assignExp3Clicked();

    void updatePIDParametersView(int heaterPerc, int peltierPerc);
    void updatePIDParametersOnBoardClicked();



};
#endif // MainWindow_H
