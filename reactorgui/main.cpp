#include "uiTSMainWindow.h"
#include "RedisDBManager.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

namespace {
const char *kUiConfigPath = "conf/reactorgui.conf";
const char *kDefaultRedisHost = "127.0.0.1";
const int kDefaultRedisPort = 6379;
const char *kDefaultTheme = "Light";
const char *kDefaultMenuPosition = "top";

struct UiConfig {
    std::string redisHost;
    int redisPort;
    QString theme;
    QString menuPosition;
};

QString normalizeTheme(const QString &value)
{
    QString lower = value.trimmed().toLower();
    if (lower.contains("touch")) {
        return "Touch";
    }
    if (lower.contains("dark")) {
        return "Dark";
    }
    if (lower.contains("light")) {
        return "Light";
    }
    return QString::fromUtf8(kDefaultTheme);
}

QString normalizeMenuPosition(const QString &value)
{
    QString lower = value.trimmed().toLower();
    if (lower == "bottom") {
        return "bottom";
    }
    return "top";
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

void ensureConfDir()
{
    QDir().mkpath("conf");
}

UiConfig loadUiConfig()
{
    UiConfig config{std::string(kDefaultRedisHost),
                    kDefaultRedisPort,
                    QString::fromUtf8(kDefaultTheme),
                    QString::fromUtf8(kDefaultMenuPosition)};

    QFile file(QString::fromUtf8(kUiConfigPath));
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
                config.redisHost = value.toStdString();
            } else if (key == "redis_port") {
                int port = config.redisPort;
                if (parsePort(value, &port)) {
                    config.redisPort = port;
                }
            } else if (key == "theme") {
                config.theme = normalizeTheme(value);
            } else if (key == "menu_position") {
                config.menuPosition = normalizeMenuPosition(value);
            }
        }
        return config;
    }

    ensureConfDir();
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&file);
        out << "redis_host=" << QString::fromStdString(config.redisHost) << '\n';
        out << "redis_port=" << config.redisPort << '\n';
        out << "theme=" << config.theme << '\n';
        out << "menu_position=" << config.menuPosition << '\n';
    }
    return config;
}
} // namespace

int main(int argc, char *argv[])
{
    UiConfig config = loadUiConfig();
    RedisDBManager::setDefaultConnection(config.redisHost, config.redisPort);

    QApplication a(argc, argv);
    TSMainWindow w;
    w.showMaximized();
    return a.exec();
}
