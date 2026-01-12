#include "uiOptionsWidget.h"
#include "ui_uiOptionsWidget.h"

#include <QFile>
#include <QDir>
#include <fstream>

namespace {
const char *kThemeConfPath = "conf/style.conf";
const char *kMenuConfPath = "conf/menu.conf";

QString normalizeTheme(const QString &value)
{
    QString lower = value.toLower();
    if (lower.contains("dark")) {
        return "Dark";
    }
    if (lower.contains("light")) {
        return "Light";
    }
    return "Light";
}

QString themeToFile(const QString &theme)
{
    if (theme.compare("dark", Qt::CaseInsensitive) == 0) {
        return ":/resources/qss/tad_dark.qss";
    }
    return ":/resources/qss/tad_light.qss";
}

QString readFirstLine(const char *path)
{
    QFile file(QString::fromUtf8(path));
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    return QString::fromUtf8(file.readLine()).trimmed();
}

void ensureConfDir()
{
    QDir().mkpath("conf");
}
} // namespace

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    ui->themesComboBox->addItem("Light");
    ui->themesComboBox->addItem("Dark");
    ui->menuPositionComboBox->addItem("Top");
    ui->menuPositionComboBox->addItem("Bottom");

    m_parent = parent;

    QString theme = normalizeTheme(readFirstLine(kThemeConfPath));
    ui->themesComboBox->setCurrentText(theme);

    QString menuPosition = readFirstLine(kMenuConfPath);
    if (menuPosition.compare("bottom", Qt::CaseInsensitive) == 0) {
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
    ensureConfDir();

    QString theme = ui->themesComboBox->currentText().trimmed();
    std::ofstream styleConf(kThemeConfPath);
    styleConf << theme.toStdString();
    styleConf.close();

    QString menuPosition = ui->menuPositionComboBox->currentText().trimmed();
    std::ofstream menuConf(kMenuConfPath);
    menuConf << menuPosition.toLower().toStdString();
    menuConf.close();

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

    emit sgn_menuPositionChanged(menuPosition.compare("top", Qt::CaseInsensitive) == 0);
}
