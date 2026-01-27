#include <cpp_redis/core/client.hpp>
#include <iostream>
#include "ServiceRunner.h"
#include "src/common/Cell.h"
#include "src/common/Experiment.h"
#include <chrono>
#include <thread>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTextStream>
#include <QThread>
#include <fstream>

using namespace std;

namespace {
const char *kServiceConfigPath = "conf/experimentmanagerservice.conf";
const char *kDefaultRedisHost = "127.0.0.1";
const int kDefaultRedisPort = 6379;

struct RedisConfig {
    std::string host;
    int port;
};

void ensureConfDir()
{
    QDir().mkpath("conf");
}

bool parsePort(const QString &value, int *portOut)
{
    bool ok = false;
    int port = value.trimmed().toInt(&ok);
    if (!ok || port <= 0 || port > 65535) {
        return false;
    }
    *portOut = port;
    return true;
}

RedisConfig loadRedisConfig()
{
    RedisConfig config{std::string(kDefaultRedisHost), kDefaultRedisPort};
    QFile file(QString::fromUtf8(kServiceConfigPath));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#') || line.startsWith(';')) {
                continue;
            }
            int idx = line.indexOf('=');
            if (idx <= 0) {
                continue;
            }
            QString key = line.left(idx).trimmed().toLower();
            QString value = line.mid(idx + 1).trimmed();
            if (key == "redis_host" && !value.isEmpty()) {
                config.host = value.toStdString();
            } else if (key == "redis_port") {
                int port = config.port;
                if (parsePort(value, &port)) {
                    config.port = port;
                }
            }
        }
    } else {
        ensureConfDir();
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream out(&file);
            out << "redis_host=" << QString::fromStdString(config.host) << '\n';
            out << "redis_port=" << config.port << '\n';
        }
    }
    return config;
}
} // namespace


void writeJSONToFile(const std::string& jsonString, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << jsonString;
        outputFile.close();
        std::cout << "JSON data has been written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
    }
}

bool ensureRedisRunning(const std::string& host, int port)
{
    cpp_redis::client client;
    client.connect(host, port);
    if (client.is_connected()) {
        return true;
    }

    QString hostValue = QString::fromStdString(host).trimmed().toLower();
    if (hostValue != "127.0.0.1" && hostValue != "localhost" && hostValue != "::1") {
        std::cerr << "Redis host is remote; skipping redis-server startup." << std::endl;
        return false;
    }

    QString program = "redis-server";
    QStringList arguments;
    arguments << "--bind" << QString::fromStdString(host);
    arguments << "--port" << QString::number(port);

    bool started = QProcess::startDetached(program, arguments);
    if (!started) {
        std::cerr << "Failed to start redis-server process." << std::endl;
        return false;
    }

    for (int attempt = 0; attempt < 10; ++attempt) {
        QThread::msleep(200);
        client.connect(host, port);
        if (client.is_connected()) {
            return true;
        }
    }

    std::cerr << "redis-server did not become available after start attempt." << std::endl;
    return false;
}

int main(int argc, char *argv[])
{
    RedisConfig config = loadRedisConfig();
    RedisDBManager::setDefaultConnection(config.host, config.port);
    ensureRedisRunning(config.host, config.port);

/*
    for(int i = 0; i < 9; i++){
        Cell cell;
        cell.setCellID("s25_" + QString::number((i+1)*111).toStdString());
        cell.setPositionIdx(i+1);
        Document document;
        document.SetObject();

        // Pass the document's allocator to toJSON method to allocate memory for JSON objects
        Document::AllocatorType& allocator = document.GetAllocator();

        Value cellJSON = cell.toJSON(allocator);
        document.AddMember(DB_CELLJSON_KEY, cellJSON, allocator);

        // Prepare writer and string buffer to convert JSON to string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        // Get the JSON string
        std::string jsonString = buffer.GetString();
        std::future<cpp_redis::reply> reply = client.hset(DB_CELLTABLE_KEY, cell.cellID(), jsonString);
    }
    Cell cell;
    cell.setCellID("s25_101010");
    cell.setPositionIdx(10);
    Document document;
    document.SetObject();

    // Pass the document's allocator to toJSON method to allocate memory for JSON objects
    Document::AllocatorType& allocator = document.GetAllocator();

    Value cellJSON = cell.toJSON(allocator);
    document.AddMember(DB_CELLJSON_KEY, cellJSON, allocator);

    // Prepare writer and string buffer to convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // Get the JSON string
    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = client.hset(DB_CELLTABLE_KEY, cell.cellID(), jsonString);
    client.sync_commit();


    return 0;
*/

    QCoreApplication app(argc, argv); // Create a QCoreApplication instance to run the event loop

    ServiceRunner serviceRunner(config.host, config.port);
    while (true) {
        if (serviceRunner.initService()) {
            try {
                serviceRunner.startServiceLoop();
            }
            catch (...) {
                continue;
            }
        }
        std::cout << "waiting for busboard..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Process events in the Qt event loop
        app.processEvents();
    }

    return app.exec(); // This will never be reached in a non-GUI application
}
