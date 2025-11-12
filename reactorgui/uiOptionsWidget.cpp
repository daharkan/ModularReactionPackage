#include "uiOptionsWidget.h"
#include "ui_uiOptionsWidget.h"

#include <QDir>
#include <fstream>

OptionsWidget::OptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionsWidget)
{
    ui->setupUi(this);
    QDir dir;
    dir.setPath(":/resources/qss");
    QStringList filters;
    filters << "*.qss";
    filters << "*.css";

    dir.setNameFilters(QStringList(filters));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();
    ui->themesComboBox->addItem("none");
    for (int i=0; i<fileList.count(); i++)
    {
        QString filename = fileList.at(i);
        ui->themesComboBox->addItem(filename);
    }

    m_parent = parent;

    std::ifstream styleConf("conf/style.conf");

    std::string line;
    std::getline(styleConf, line);
    styleConf.close();

    QString filename;
    if(line.size() > 0){
        ui->themesComboBox->setCurrentText(QString::fromStdString(line));
        filename = ":/resources/qss/" + ui->themesComboBox->currentText();

    }else{
        filename = ":/resources/qss/" + ui->themesComboBox->currentText();
    }

    QFile styleFile(filename );
    bool ok = styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    if(ok){
    QString style( styleFile.readAll() );
        this->setStyleSheet(style);
        m_parent->setStyleSheet(style);
    }else{
        this->setStyleSheet(styleSheet());
        m_parent->setStyleSheet(styleSheet());

    }


    connect(ui->applyPrefsPushButton, &QPushButton::clicked, this, &OptionsWidget::applyButtonClicked);
}

OptionsWidget::~OptionsWidget()
{
    delete ui;
}

void OptionsWidget::applyButtonClicked()
{

    std::ofstream styleConf("conf/style.conf");
    styleConf << ui->themesComboBox->currentText().toStdString();
    styleConf.close();

    if(ui->themesComboBox->currentText().compare("none") == 0){
        this->setStyleSheet("");
        std::ofstream styleConf("conf/style.conf");
        styleConf << ui->themesComboBox->currentText().toStdString();
        styleConf.close();
        return;
    }


    QString filename = ":/resources/qss/" + ui->themesComboBox->currentText();

    QFile styleFile(filename );
    bool ok = styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    if(ok){
        QString style( styleFile.readAll() );
        this->setStyleSheet(style);
        m_parent->setStyleSheet(style);
    }else{
        this->setStyleSheet(styleSheet());
        m_parent->setStyleSheet(styleSheet());

    }
}
