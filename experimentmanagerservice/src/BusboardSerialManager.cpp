#include "BusboardSerialManager.h"
#include "src/common/Cell.h"
#include <QCoreApplication>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <sstream>
#include <thread>
#include <QSet>
#include <QElapsedTimer>

#define BAUDRATE 115200
#define BUSBOARD_HANDSHAKE "bbb"

namespace {
QSet<QString> s_usedPorts;
constexpr int kAckTimeoutMs = 250;

std::string cleanBusboardToken(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result), [](char c) {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    });
    return result;
}

std::string normalizeBusboardId(const std::string& token) {
    std::string cleaned = cleanBusboardToken(token);
    std::string upper = cleaned;
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });

    std::string side = "RHS";
    if (upper.find("LHS") != std::string::npos || upper.find("LEFT") != std::string::npos) {
        side = "LHS";
    } else if (upper.find("RHS") != std::string::npos || upper.find("RIGHT") != std::string::npos) {
        side = "RHS";
    }

    std::string digits;
    for (char c : cleaned) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        }
    }
    if (digits.empty()) {
        digits = "000";
    } else if (digits.size() < 3) {
        digits.insert(digits.begin(), 3 - digits.size(), '0');
    }

    return "bbb_" + side + "_" + digits;
}

bool buildAckTokenFromCommand(const QString &command, QString *tokenOut) {
    QString trimmed = command.trimmed();
    if (!trimmed.startsWith('>') || !trimmed.endsWith('<')) {
        return false;
    }

    QString payload = trimmed.mid(1, trimmed.size() - 2);
    QStringList parts = payload.split('#');
    if (parts.size() < 5) {
        return false;
    }

    bool okPos = false;
    bool okChecksum = false;
    int pos = parts.at(0).toInt(&okPos);
    int checksum = parts.at(4).toInt(&okChecksum);
    if (!okPos || !okChecksum) {
        return false;
    }

    *tokenOut = QString("ACK#%1#%2").arg(pos).arg(checksum);
    return true;
}
} // namespace

BusboardSerialManager::BusboardSerialManager(QObject *parent)
    : QObject(parent)
{
}

BusboardSerialManager::~BusboardSerialManager()
{
    if (m_serialPort) {
        m_serialPort->close();
    }
}
bool BusboardSerialManager::connectAndAssignThePort()
{    
    QList<QSerialPortInfo> comPorts = QSerialPortInfo::availablePorts();
    //qDebug() << "tss refreshAndInit";

    for (int i = 0; i < comPorts.size(); i++) {
        //qDebug() << " tss com port processing..." << comPorts.at(i).portName();

        QString portName = comPorts.at(i).portName();
        if (s_usedPorts.contains(portName)) {
            continue;
        }

        QSerialPort *tmpSerial = new QSerialPort(this);
        tmpSerial->setPortName(portName);
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
                    m_recievedBusboardSerial = normalizeBusboardId(token);

                    qDebug() << "connected to " << tmpSerial->portName() << " with the serial: " << m_recievedBusboardSerial;

                    m_serialPort = tmpSerial;
                    m_portName = portName;
                    s_usedPorts.insert(portName);
                    m_serialPort->clearError();
                    connect(m_serialPort, &QSerialPort::readyRead, this, &BusboardSerialManager::serialRecieved);
                    return true;
                }

                cnt++;
            }  //while waiting BUSBOARD_HANDSHAKE
        }
        qDebug() << " tss com port closing..." << comPorts.at(i).portName();
        tmpSerial->close();
        tmpSerial->deleteLater();

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
    if (!m_serialPort) {
        return false;
    }
    return (m_serialPort->isOpen() && (m_serialPort->error() == QSerialPort::NoError));
}

bool BusboardSerialManager::writeCellUpdateString(QString str)
{
    //qDebug() << "-BusboardSerialManager::writeCellUpdateString-";

    if(!m_serialPort){
        return false;
    }

    QString ackToken;
    bool expectAck = buildAckTokenFromCommand(str, &ackToken);
    if (expectAck) {
        m_pendingAckTokens.insert(ackToken);
    }

    writeString(str.trimmed(), m_serialPort);

    if (!expectAck) {
        return true;
    }

    bool acked = waitForAckToken(ackToken, kAckTimeoutMs);
    if (!acked) {
        m_pendingAckTokens.remove(ackToken);
        qDebug() << "ACK timeout for" << ackToken;
    }
   // qDebug() << "-BusboardSerialManager::writeCellUpdateString-----";

    return acked;
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
    if (!m_serialPort) {
        return;
    }

    m_rxBuffer.append(m_serialPort->readAll());

    int newlineIndex = -1;
    while ((newlineIndex = m_rxBuffer.indexOf('\n')) != -1) {
        QByteArray line = m_rxBuffer.left(newlineIndex);
        m_rxBuffer.remove(0, newlineIndex + 1);

        QString dataString = QString::fromUtf8(line).remove("\r").remove(" ").remove("\u0000");
        if (dataString.isEmpty()) {
            continue;
        }

        //qDebug() << "dataString:" << dataString;
        QCoreApplication::processEvents();

        if (dataString.startsWith("ACK#")) {
            QString ackToken = dataString.trimmed();
            if (m_pendingAckTokens.contains(ackToken)) {
                m_pendingAckTokens.remove(ackToken);
            }
            continue;
        }

        if (dataString.startsWith("presence#")) {
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
            continue;
        }

        if (dataString.contains("GO")) {
            flushPendingUpdates();
            clearMessageQueue();
            continue;
        }

        if (dataString.count("#") < 8) {
            continue;
        }

        Cell cell;
        cell.updateStatusFromBoard(dataString.toStdString());
        cell.setIsPlugged(true);
        //qDebug() << "cell id: " << cell.cellID();
        QCoreApplication::processEvents();

        emit sgn_updateCell(cell);

        //qDebug() << std::chrono::system_clock::now().time_since_epoch()/1000000 << " -  incoming:  " << dataString;
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
    if (m_writing) {
        return;
    }


    if (!port->isOpen() || !port->isWritable()) {
        qDebug() << "serial port not writable";
        return;
    }


    m_writing = true;

    str.append("\n");
    QByteArray ba = str.toUtf8();
    if (port->bytesToWrite() > 0) {
        port->clear(QSerialPort::Output);
    }
    port->write(ba);
    port->waitForBytesWritten(200);
    delay(20);
    port->write(ba);
    port->waitForBytesWritten(200);
    qDebug() << "outgoing: " << str;

    delay(200);

    m_writing = false;

}

bool BusboardSerialManager::waitForAckToken(const QString &token, int timeoutMs)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < timeoutMs) {
        if (!m_pendingAckTokens.contains(token)) {
            return true;
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return false;
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

QString BusboardSerialManager::portName() const
{
    return m_portName;
}
