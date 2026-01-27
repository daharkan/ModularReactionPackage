#include "uiOptionsWidget.h"
#include "ui_uiOptionsWidget.h"

#include <QDir>
#include <QFile>
#include <QMap>
#include <QTextStream>

namespace {
const char *kUiConfigPath = "conf/reactorgui.conf";
const char *kDefaultTheme = "Light";
const char *kDefaultMenuPosition = "top";

using ConfigMap = QMap<QString, QString>;

void ensureConfDir()
{
    QDir().mkpath("conf");
}

ConfigMap readConfigFile(const QString &path)
{
    ConfigMap map;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return map;
    }
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
        if (!key.isEmpty()) {
            map.insert(key, value);
        }
    }
    return map;
}

bool writeConfigFile(const QString &path, const ConfigMap &map)
{
    ensureConfDir();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        out << it.key() << '=' << it.value() << '\n';
    }
    return true;
}

QString normalizeTheme(const QString &value)
{
    QString lower = value.toLower();
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
    return QString::fromUtf8(kDefaultMenuPosition);
}

QString themeToFile(const QString &theme)
{
    if (theme.compare("touch", Qt::CaseInsensitive) == 0) {
        return ":/resources/qss/tad_touch.qss";
    }
    if (theme.compare("dark", Qt::CaseInsensitive) == 0) {
        return ":/resources/qss/tad_dark.qss";
    }
    return ":/resources/qss/tad_light.qss";
}
} // namespace

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    ui->themesComboBox->addItem("Light");
    ui->themesComboBox->addItem("Dark");
    ui->themesComboBox->addItem("Touch");
    ui->menuPositionComboBox->addItem("Top");
    ui->menuPositionComboBox->addItem("Bottom");

    m_parent = parent;

    ConfigMap config = readConfigFile(QString::fromUtf8(kUiConfigPath));
    QString theme = normalizeTheme(config.value("theme", QString::fromUtf8(kDefaultTheme)));
    ui->themesComboBox->setCurrentText(theme);

    QString menuPosition = normalizeMenuPosition(config.value("menu_position", QString::fromUtf8(kDefaultMenuPosition)));
    if (menuPosition == "bottom") {
        ui->menuPositionComboBox->setCurrentText("Bottom");
    } else {
        ui->menuPositionComboBox->setCurrentText("Top");
    }

    QString filename = themeToFile(theme);
    QFile styleFile(filename);
    if (styleFile.open(QFile::ReadOnly)) {
        QString style(styleFile.readAll());
        this->setStyleSheet(style);
        if (m_parent != nullptr) {
            m_parent->setStyleSheet(style);
        }
    } else {
        this->setStyleSheet(styleSheet());
        if (m_parent != nullptr) {
            m_parent->setStyleSheet(styleSheet());
        }
    }


    connect(ui->applyPrefsPushButton, &QPushButton::clicked, this, &OptionsWidget::applyButtonClicked);
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget::applyButtonClicked()
{
    ConfigMap config = readConfigFile(QString::fromUtf8(kUiConfigPath));
    QString theme = normalizeTheme(ui->themesComboBox->currentText().trimmed());
    QString menuPosition = normalizeMenuPosition(ui->menuPositionComboBox->currentText().trimmed());
    config.insert("theme", theme);
    config.insert("menu_position", menuPosition);
    writeConfigFile(QString::fromUtf8(kUiConfigPath), config);

    QString filename = themeToFile(theme);
    QFile styleFile(filename);
    if (styleFile.open(QFile::ReadOnly)) {
        QString style(styleFile.readAll());
        this->setStyleSheet(style);
        if (m_parent != nullptr) {
            m_parent->setStyleSheet(style);
        }
    } else {
        this->setStyleSheet(styleSheet());
        if (m_parent != nullptr) {
            m_parent->setStyleSheet(styleSheet());
        }
    }

    emit sgn_menuPositionChanged(menuPosition == "top");
}
