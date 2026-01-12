#include "uiTSMainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TSMainWindow w;
    w.showMaximized();
    return a.exec();
}
