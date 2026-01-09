#include "BusboardSerialManager.h"
#include "src/common/Cell.h"
#include <QCoreApplication>
#include <chrono>
#include <thread>
#include <sstream>

#define BAUDRATE 9600
#define BUSBOARD_HANDSHAKE "bb"

BusboardSerialManager* BusboardSerialManager::m_instance = nullptr;


BusboardSerialManager::BusboardSerialManager() : QObject() {
}



BusboardSerialManager* BusboardSerialManager::getInstance()
{
    if(m_instance == nullptr){
        m_instance = new BusboardSerialManager();
    }
    return m_instance;
}

std::string removeNonAlphanumeric(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c));
    });
    return result;
}
bool BusboardSerialManager::connectAndAssignThePort()
{    
    QList<QSerialPortInfo> comPorts = QSerialPortInfo::availablePorts();
    //qDebug() << "tss refreshAndInit";

    for (int i = 0; i < comPorts.size(); i++) {
        //qDebug() << " tss com port processing..." << comPorts.at(i).portName();

        QSerialPort *tmpSerial = new QSerialPort(comPorts.at(i).portName());
        tmpSerial->setPortName(comPorts.at(i).portName());
        tmpSerial->setBaudRate(BAUDRATE);
        tmpSerial->setDataBits(QSerialPort::Data8);
        tmpSerial->setParity(QSerialPort::NoParity);
        tmpSerial->setStopBits(QSerialPort::OneStop);
        tmpSerial->setFlowControl(QSerialPort::SoftwareControl);
        tmpSerial->open(QIODevice::ReadWrite);
       // tmpSerial->set(500, 500, 500); // Set read, write, and event timeouts (in milliseconds)

        int cnt = 0;
        if(tmpSerial->isOpen()){
            while (cnt < 50) {
                tmpSerial->waitForReadyRead(50);
                QString incoming="";

                if(tmpSerial->bytesAvailable() > 4){
                    incoming = tmpSerial->readLine();
                }else{
                    cnt++;
                    continue;
                }

                if(incoming.contains(BUSBOARD_HANDSHAKE)){
                    std::stringstream ss(incoming.toStdString());
                    std::string token;

                    // Read tokens separated by '#' delimiter
                    std::getline(ss, token, '#'); // Serial number, ignoring
                    m_recievedBusboardSerial = removeNonAlphanumeric(token);

                    qDebug() << "connected to " << tmpSerial->portName() << " with the serial: " << m_recievedBusboardSerial;

                    m_serialPort = tmpSerial;
                    m_serialPort->clearError();
                    connect(m_serialPort, &QSerialPort::readyRead, this, &BusboardSerialManager::serialRecieved);
                    return true;
                }

                cnt++;
            }  //while waiting BUSBOARD_HANDSHAKE
        }
        qDebug() << " tss com port closing..." << comPorts.at(i).portName();
        tmpSerial->close();

    } //for all com ports

    return false;
}

void BusboardSerialManager::sendExampleString(QString str)
{
    writeString(str, m_serialPort);

}

bool BusboardSerialManager::isSerialPortOK()
{
    //qDebug() << "::: " << m_serialPort->error();
    return (m_serialPort->isOpen() && (m_serialPort->error() == QSerialPort::NoError));
}

bool BusboardSerialManager::writeCellUpdateString(QString str)
{
    //qDebug() << "-BusboardSerialManager::writeCellUpdateString-";

    if(!m_serialPort){
        return false;
    }

    writeString2Queue(str.trimmed());
   // qDebug() << "-BusboardSerialManager::writeCellUpdateString-----";

    return true;
}

bool BusboardSerialManager::queueUpdateCommand(const QString &command)
{
    int open = command.indexOf('>');
    if (open == -1) {
        return false;
    }
    int close = command.indexOf('<', open + 1);
    if (close == -1) {
        return false;
    }

    QString payload = command.mid(open + 1, close - open - 1);
    QStringList parts = payload.split('#');
    if (parts.size() < 4) {
        return false;
    }

    bool ok = false;
    int positionIdx = parts.at(0).toInt(&ok);
    if (!ok) {
        return false;
    }

    QString framed = command.mid(open, close - open + 1).trimmed();
    if (framed.isEmpty()) {
        return false;
    }

    m_pendingUpdates[positionIdx] = framed;
    return true;
}

void BusboardSerialManager::flushPendingUpdates()
{
    if (!m_serialPort) {
        return;
    }

    for (auto it = m_pendingUpdates.cbegin(); it != m_pendingUpdates.cend(); ++it) {
        writeString(it.value(), m_serialPort);
    }
    m_pendingUpdates.clear();

    while (!m_messageQueue.isEmpty()) {
        writeString(m_messageQueue.dequeue(), m_serialPort);
    }
}

void BusboardSerialManager::serialRecieved()
{

    if(m_writing){
        return;
    }

    QByteArray data;
    while (m_serialPort->isOpen() && m_serialPort->waitForReadyRead(-1)) {
        while (m_serialPort->bytesAvailable() > 0) {
            char ch;
            if (m_serialPort->getChar(&ch)) {
                if (ch == '\n') {
                    QString incoming(data);
                    QString dataString = incoming.remove("\r").remove(" ").remove("\u0000");

                    qDebug() << "dataString:" << dataString;
                    QCoreApplication::processEvents();

                    if(dataString.startsWith("presence#")){
                        QStringList parts = dataString.split('#');
                        if (parts.size() >= 3) {
                            bool okSlot = false;
                            int slotIndex = parts.at(1).toInt(&okSlot);
                            bool okPresent = false;
                            int presentVal = parts.at(2).toInt(&okPresent);
                            if (okSlot && okPresent) {
                                emit sgn_presenceUpdate(slotIndex, presentVal == 1);
                            }
                        }
                        return;
                    }

                    if(dataString.contains("GO")){
                        flushPendingUpdates();
                        clearMessageQueue();
                        return;
                    }

                    if(dataString.count("#") < 8){
                        return;
                    }


                    Cell cell;
                    cell.updateStatusFromBoard(dataString.toStdString());
                    cell.setIsPlugged(true);
                    qDebug() << "cell id: " << cell.cellID();
                    QCoreApplication::processEvents();

                    emit sgn_updateCell(cell);

                    //qDebug() << std::chrono::system_clock::now().time_since_epoch()/1000000 << " -  incoming:  " << dataString;
                    data.clear();

                    return;
                } else {
                    data.append(ch);
                }
            }
        }
    }




}





void BusboardSerialManager::writeString(QString str, QSerialPort *port)
{
    QCoreApplication::processEvents();

    if(port == nullptr)
        throw std::exception();

    if(m_sleeping){
        return;
    }
    m_writing = true;
    str.append("\n");
    QByteArray ba = str.toUtf8();
    port->write(ba);
    qDebug() << "outgoing: " << str;


    port->flush();
    port->clear();

    m_writing = false;

}

void BusboardSerialManager::writeString2Queue(QString str)
{
    if (str.isEmpty()) {
        return;
    }

    int start = 0;
    bool queuedAny = false;
    while (true) {
        int open = str.indexOf('>', start);
        if (open == -1) {
            break;
        }
        int close = str.indexOf('<', open);
        if (close == -1) {
            break;
        }
        QString cmd = str.mid(open, close - open + 1).trimmed();
        if (queueUpdateCommand(cmd)) {
            queuedAny = true;
        }
        start = close + 1;
    }

    if (!queuedAny) {
        m_messageQueue.enqueue(str);
    }
}

void BusboardSerialManager::clearMessageQueue()
{
    m_messageQueue.clear();
}



void BusboardSerialManager::delay(int msec)
{
    m_sleeping = true;

    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::milliseconds(msec);

    while (std::chrono::steady_clock::now() < endTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small sleep to prevent high CPU usage
    }

    m_sleeping = false;
}

std::string BusboardSerialManager::recievedBusboardSerial() const
{
    return m_recievedBusboardSerial;
}
