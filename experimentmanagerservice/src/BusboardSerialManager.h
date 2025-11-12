#ifndef BUSBOARDSERIALMANAGER_H
#define BUSBOARDSERIALMANAGER_H

#include "common/Cell.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class BusboardSerialManager : public QObject
{
    Q_OBJECT
public:
    static BusboardSerialManager* getInstance();
    bool connectAndAssignThePort();
    void sendExampleString(QString str);
    bool isSerialPortOK();
    bool writeCellUpdateString(QString str);

    std::string recievedBusboardSerial() const;

private:
    BusboardSerialManager();
    QSerialPort* m_serialPort = nullptr;
    static BusboardSerialManager *m_instance;
    bool m_sleeping = false;
    QString m_messageQueue = "";
    bool m_writing = false;

    void writeString(QString str, QSerialPort *port);
    void writeString2Queue(QString str);
    void clearMessageQueue();
    void delay(int msec);


    std::string m_recievedBusboardSerial;


private slots:
    void serialRecieved();

signals:
    void sgn_updateCell(Cell& cell);

};

#endif // BUSBOARDSERIALMANAGER_H
