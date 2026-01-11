#ifndef ONLYCELLSERIALMANAGER_H
#define ONLYCELLSERIALMANAGER_H
#include "Cell.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class OnlyCellSerialManager : public QObject
{
    Q_OBJECT

public:
    static OnlyCellSerialManager* getInstance();
    bool connectAndAssignThePort();
    void sendExampleString(QString str);
    bool isSerialPortOK();
    bool writeCellUpdateString(QString str);

    std::string recievedBusboardSerial() const;

    Cell cell() const;
    void writeString2Queue(QString str);

private:
    OnlyCellSerialManager();
    QSerialPort* m_serialPort = nullptr;
    static OnlyCellSerialManager *m_instance;
    bool m_sleeping = false;
    QString m_messageQueue = "";
    bool m_writing = false;
    bool m_useBusboardDummyFormat = false;

    void writeString(QString str, QSerialPort *port);
    QString convertUpdateStringForDummy(const QString &command) const;
    void clearMessageQueue();
    void delay(int msec);

    Cell m_cell;


    std::string m_recievedBusboardSerial;


private slots:
    void serialRecieved();

signals:
    void sgn_updateCell(Cell &cell);
    void sgn_updatePIDParameters(int heaterPerc, int peltierPerc);

};

#endif // ONLYCELLSERIALMANAGER_H
