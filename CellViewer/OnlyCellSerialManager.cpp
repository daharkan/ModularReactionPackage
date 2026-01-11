#include "OnlyCellSerialManager.h"
#include <QCoreApplication>
#include <chrono>
#include <thread>
#include <sstream>


#define BAUDRATE 9600
#define BUSBOARD_HANDSHAKE "bb"
#define DEVICE_UPDATE_ASK_COMMAND "GO"

OnlyCellSerialManager* OnlyCellSerialManager::m_instance = nullptr;


OnlyCellSerialManager::OnlyCellSerialManager() : QObject() {
    m_messageQueue = "";
}



OnlyCellSerialManager* OnlyCellSerialManager::getInstance()
{
    if(m_instance == nullptr){
        m_instance = new OnlyCellSerialManager();
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
bool OnlyCellSerialManager::connectAndAssignThePort()
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
            while (cnt < 20
                   ) {
                tmpSerial->waitForReadyRead(250);
                QString incoming="";

                if(tmpSerial->bytesAvailable() > 4){
                    incoming = tmpSerial->readLine();
                }else{
                    cnt++;
                    continue;
                }

                if(incoming.contains(BUSBOARD_HANDSHAKE) || incoming.contains(DEVICE_UPDATE_ASK_COMMAND)){
                    std::stringstream ss(incoming.toStdString());
                    std::string token;

                    // Read tokens separated by '#' delimiter
                    std::getline(ss, token, '#'); // Serial number, ignoring
                    m_recievedBusboardSerial = removeNonAlphanumeric(token);

                    qDebug() << "connected to " << tmpSerial->portName() << " with the serial: " << m_recievedBusboardSerial;

                    m_serialPort = tmpSerial;
                    m_serialPort->clearError();
                    connect(m_serialPort, &QSerialPort::readyRead, this, &OnlyCellSerialManager::serialRecieved);
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

void OnlyCellSerialManager::sendExampleString(QString str)
{
    writeString(str, m_serialPort);

}

bool OnlyCellSerialManager::isSerialPortOK()
{
    //qDebug() << "::: " << m_serialPort->error();
    return (m_serialPort->isOpen() && (m_serialPort->error() == QSerialPort::NoError));
}

bool OnlyCellSerialManager::writeCellUpdateString(QString str)
{
    //qDebug() << "-OnlyCellSerialManager::writeCellUpdateString-";

    if(!m_serialPort){
        return false;
    }

    writeString2Queue(str);

    // qDebug() << "-OnlyCellSerialManager::writeCellUpdateString-----";

    return true;
}




void OnlyCellSerialManager::serialRecieved()
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

                    if(dataString.contains(DEVICE_UPDATE_ASK_COMMAND) && m_messageQueue.size() > 0){
                        writeString(m_messageQueue, m_serialPort);
                        clearMessageQueue();
                        return;

                    }

                    if(dataString.contains("pid")){
                        std::string datastringS = dataString.toStdString();
                        datastringS.erase(std::remove_if(datastringS.begin(), datastringS.end(),::isspace),datastringS.end());

                        // Use stringstream to parse the input string
                        std::stringstream ss(datastringS);
                        std::string token;

                        ///pid#96#0

                        // Read tokens separated by '#' delimiter
                        std::getline(ss, token, '#'); // pid prefix, ignoring

                        std::getline(ss, token, '#'); // heaterDutyCycle
                        int heaterPerc = QString::fromStdString(token).toInt();

                        std::getline(ss, token, '#'); // Position index
                        int peltierPerc = QString::fromStdString(token).toInt();
                        emit sgn_updatePIDParameters(heaterPerc, peltierPerc);
                    }
                    if(dataString.count("#") != 8){
                        return;
                    }


                    Cell cell;
                    cell.updateStatusFromBoard(dataString.toStdString());
                    //qDebug() << "cell id: " << cell.cellID();
                    QCoreApplication::processEvents();

                    m_cell = cell;
                    emit sgn_updateCell(m_cell);

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






void OnlyCellSerialManager::writeString(QString str, QSerialPort *port)
{
    QCoreApplication::processEvents();

    if(port == nullptr)
        throw std::exception();

    if(m_sleeping){
        return;
    }
    m_writing = true;
    str.append( "\n");
    str.append( "\0");
    QByteArray ba = str.toUtf8();
    port->write(ba);
    qDebug() << "outgoing: " << str;


    port->flush();
    port->clear();

    m_writing = false;

}

void OnlyCellSerialManager::writeString2Queue(QString str)
{
    if(!m_messageQueue.contains(str)){
        m_messageQueue = m_messageQueue + str;
    }
}

void OnlyCellSerialManager::clearMessageQueue()
{
    m_messageQueue = "";
}


void OnlyCellSerialManager::delay(int msec)
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

Cell OnlyCellSerialManager::cell() const
{
    return m_cell;
}

std::string OnlyCellSerialManager::recievedBusboardSerial() const
{
    return m_recievedBusboardSerial;
}


