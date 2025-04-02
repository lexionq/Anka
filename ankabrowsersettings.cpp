#include "ankabrowsersettings.h"
#include "ui_ankabrowsersettings.h"
#include <QSettings>
#include <QAbstractButton>
#include <QDir>
#include <QPushButton>
#include <QColorDialog>
#include <fstream>
#include <QFile>
#include <QTextStream>


AnkaBrowserSettings::AnkaBrowserSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AnkaBrowserSettings)
{
    ui->setupUi(this);

    QString homeDir = QDir::homePath();
    QString configFile = homeDir + "/.config/Anka/config.conf";
    QSettings settings(configFile, QSettings::IniFormat);
    QString searchEngine = settings.value("Settings/search_engine").toString();

    if (searchEngine == "https://google.com"){
        ui->search_engine_combobox->setCurrentIndex(0);
    }
    else if(searchEngine == "https://duckduckgo.com"){
        ui->search_engine_combobox->setCurrentIndex(1);
    }
    else if(searchEngine == "https://search.brave.com"){
        ui->search_engine_combobox->setCurrentIndex(2);
    }
    else if(searchEngine == "https://artadosearch.com"){
        ui->search_engine_combobox->setCurrentIndex(3);
    }
    else if(searchEngine == "https://bing.com"){
        ui->search_engine_combobox->setCurrentIndex(4);
    }
    else if(searchEngine == "https://startpage.com"){
        ui->search_engine_combobox->setCurrentIndex(5);
    }
    connect(ui->select_tab_color, &QPushButton::clicked, this, &AnkaBrowserSettings::openColorDialog);
    connect(ui->history_button, &QPushButton::clicked, this, &AnkaBrowserSettings::deleteHistory);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &AnkaBrowserSettings::onButtonClicked);

}

void AnkaBrowserSettings::openColorDialog()
{
    QString homeDir = QDir::homePath();
    QString configFile = homeDir + "/.config/Anka/config.conf";
    QSettings settings(configFile, QSettings::IniFormat);

    QColor selectedColor = QColorDialog::getColor(Qt::white, this, "Select Tab Color");
    if (selectedColor.isValid()) {
        QString selectedTabColor = selectedColor.name();
        QColor notSelectedTabColor0 = selectedColor.darker(135);
        QString notSelectedTabColor = notSelectedTabColor0.name();
        settings.setValue("Appearance/selected_tab_color", selectedTabColor);
        settings.setValue("Appearance/not_selected_tab_color", notSelectedTabColor);
    }
}


void AnkaBrowserSettings::deleteHistory()
{
    QString homeDir = QDir::homePath();
    QString historyFile = homeDir + "/.config/Anka/history.txt";

    QFile file(historyFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
    }
}

void AnkaBrowserSettings::onButtonClicked(QAbstractButton *button)
{
    QString homeDir = QDir::homePath();
    QString configFile = homeDir + "/.config/Anka/config.conf";
    QSettings settings(configFile, QSettings::IniFormat);

    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole){
        int selectedSearchEngineIndex = ui->search_engine_combobox->currentIndex();

        switch(selectedSearchEngineIndex){
        case 0:
            settings.setValue("Settings/search_engine", "https://google.com");
            break;
        case 1:
            settings.setValue("Settings/search_engine", "https://duckduckgo.com");
            break;
        case 2:
            settings.setValue("Settings/search_engine", "https://search.brave.com");
            break;
        case 3:
            settings.setValue("Settings/search_engine", "https://artadosearch.com");
            break;
        case 4:
            settings.setValue("Settings/search_engine", "https://bing.com");
            break;
        case 5:
            settings.setValue("Settings/search_engine", "https://startpage.com");
            break;
        default:
            break;

        }
        accept();


    }
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole){
        reject();
    }
}

AnkaBrowserSettings::~AnkaBrowserSettings()
{
    delete ui;
}
