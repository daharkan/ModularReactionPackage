#ifndef BUSBOARDSERIALMANAGER_H
#define BUSBOARDSERIALMANAGER_H

#include "common/Cell.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QQueue>
#include <QMap>
#include <QDebug>
#include <QSet>
#include <QByteArray>

class BusboardSerialManager : public QObject
{
    Q_OBJECT
public:
    explicit BusboardSerialManager(QObject *parent = nullptr);
    ~BusboardSerialManager();
    bool connectAndAssignThePort();
    void sendExampleString(QString str);
    bool isSerialPortOK();
    bool writeCellUpdateString(QString str);

    std::string recievedBusboardSerial() const;
    QString portName() const;

private:
    QSerialPort* m_serialPort = nullptr;
    bool m_sleeping = false;
    QQueue<QString> m_messageQueue;
    QMap<int, QString> m_pendingUpdates;
    QSet<QString> m_pendingAckTokens;
    bool m_writing = false;
    QString m_portName;
    QByteArray m_rxBuffer;

    void writeString(QString str, QSerialPort *port);
    void writeString2Queue(QString str);
    void clearMessageQueue();
    void flushPendingUpdates();
    bool queueUpdateCommand(const QString &command);
    bool waitForAckToken(const QString &token, int timeoutMs);
    void delay(int msec);


    std::string m_recievedBusboardSerial;


private slots:
    void serialRecieved();

signals:
    void sgn_updateCell(Cell& cell);
    void sgn_presenceUpdate(int slotIndex, bool isPresent);

};

#endif // BUSBOARDSERIALMANAGER_H
