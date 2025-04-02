#include "ankabrowser.h"
#include "ui_ankabrowser.h"
#include "ankabrowsersettings.h"
#include "ui_ankabrowsersettings.h"
#include <QDir>
#include <QSettings>
#include <QIODevice>
#include <QtWebEngineWidgets>
#include <QDebug>
#include <fstream>
#include <QFileDialog>



AnkaBrowser::AnkaBrowser(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AnkaBrowser)
    , ankabrowsersettings(new AnkaBrowserSettings(this))
{
    ui->setupUi(this);


    QString homeDir = QDir::homePath();
    QString configPath = homeDir + "/.config/Anka";
    QString configFile = configPath + "/config.conf";
    QString bookmarks = configPath + "/bookmarks.txt";
    QString downloads = configPath + "/downloads.txt";
    QString history = configPath + "/history.txt";

    QSettings settings(configFile, QSettings::IniFormat);

    QString search_engine = settings.value("Settings/search_engine").toString();


    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(configPath);
    }
    QFile file (configPath);
    if (!file.exists()){
        if (file.open(QIODevice::WriteOnly)){
            file.close();
        }

        settings.setValue("Settings/search_engine", "https://google.com");
        settings.setValue("Appearance/selected_tab_color", "#940e0e");
        settings.setValue("Appearance/not_selected_tab_color", "#6e0a0a");
        settings.setValue("Language/language", "tr-TR");
    }
    QFile file1 (bookmarks);
    if (!file1.exists()) {
        if (file1.open(QIODevice::WriteOnly)){
            file1.close();
        }
    }
    QFile file2 (downloads);
    if (!file2.exists()) {
        if (file2.open(QIODevice::WriteOnly)){
            file2.close();
        }

    }
    QFile file3 (history);
    if (!file3.exists()) {
        if (file3.open(QIODevice::WriteOnly)){
            file3.close();
        }

    }


    std::string tab_name = "New Tab | Anka Browser";

    addNewTab(search_engine, QString::fromStdString(tab_name));



}

AnkaBrowser::~AnkaBrowser()
{
    delete ui;
}

void AnkaBrowser::addNewTab(const QString& url, const QString& label)
{
    QWebEngineView* newBrowser = new QWebEngineView();
    QString homeDir = QDir::homePath();
    QString configPath = homeDir + "/.config/Anka/";
    QString configFile = configPath + "config.conf";
    QSettings settings(configFile, QSettings::IniFormat);
    QString tab_color = settings.value("Appearance/selected_tab_color").toString();
    QString not_selected_tab_color = settings.value("Appearance/not_selected_tab_color").toString();
    std::string tab_name = "New Tab | Anka Browser";

    QString search_engine = settings.value("Settings/search_engine", "https://google.com").toString();
    if (!url.isEmpty()) {
        newBrowser->setUrl(QUrl(url));
    } else {
        newBrowser->setUrl(QUrl(search_engine));
    }


    int index = ui->tabs->addTab(newBrowser, truncateTabText(label));
    ui->tabs->setCurrentWidget(newBrowser);
    ui->tabs->setMovable(true);
    ui->tabs->setTabsClosable(true);
    QString tabStyleSheet = "QTabBar::tab { background: " + not_selected_tab_color + " ; min-width:145px; max-width:220px; height:25px; border-top-left-radius: 10px; border-top-right-radius: 10px; padding: 5px; } QTabBar::tab:selected { background: " + tab_color  + "; }";
    ui->tabs->tabBar()->setStyleSheet(tabStyleSheet);

    connect(ui->newtab_button, &QPushButton::clicked, [this, search_engine, tab_name]() {
        addNewTab(search_engine, QString::fromStdString(tab_name));
    });
    connect(ui->back_button, &QPushButton::clicked, this, &AnkaBrowser::browserBack);
    connect(ui->forward_button, &QPushButton::clicked, this, &AnkaBrowser::browserForward);
    connect(ui->reload_button, &QPushButton::clicked, this, &AnkaBrowser::browserReload);
    connect(ui->settings_button, &QPushButton::clicked, this, &AnkaBrowser::showSettingsMenu);

    connect(ui->tabs, &QTabWidget::tabCloseRequested, this, [this, index](int tabIndex) {
        if (tabIndex == index) {
            closeTab(tabIndex);
        }
    });

    connect(newBrowser, &QWebEngineView::titleChanged, [this, newBrowser](const QString& title) {
        updateTitle(newBrowser, title);
    });

    connect(newBrowser, &QWebEngineView::urlChanged, [this](const QUrl& q) {
        updateUrl(q);
    });

    connect(newBrowser->page()->profile(), &QWebEngineProfile::downloadRequested, this, &AnkaBrowser::handleDownloadRequest);

}

void AnkaBrowser::handleDownloadRequest(QWebEngineDownloadRequest *download){
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", download->suggestedFileName());

    if (!filePath.isEmpty()) {
        download->setDownloadDirectory(QFileInfo(filePath).absolutePath());
        download->setDownloadFileName(QFileInfo(filePath).fileName());
        download->accept();
    }

    QString homeDir = QDir::homePath();
    QString downloadsFile = homeDir + "/.config/Anka/downloads.txt";

    QFile file(downloadsFile);


    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);


        out << QFileInfo(filePath).fileName() << " - "
            << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            << "\n";

        file.close();
    }

}


QString AnkaBrowser::truncateTabText(const QString& text)
{
    const int maxTabTextLength = 15;
    return text.length() > maxTabTextLength ? text.left(maxTabTextLength) + "..." : text;
}


void AnkaBrowser::updateTitle(QWebEngineView* browser, const QString& title)
{
    int index = ui->tabs->indexOf(browser);
    if (index != -1) {
        ui->tabs->setTabText(index, truncateTabText(title));
    }
}

void AnkaBrowser::updateUrl(const QUrl& url)
{
    ui->url_bar->setText(url.toString());
    QString homeDir = QDir::homePath();
    QString configPath = homeDir + "/.config/Anka/";
    QString historyFile = configPath + "history.txt";

    QFile file(historyFile);


    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);


        out << url.toString() << " - "
            << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            << "\n";

        file.close();
    }

}

void AnkaBrowser::showHistory()
{
    QString homeDir = QDir::homePath();
    QString historyFile = homeDir + "/.config/Anka/history.txt";

    QFile file(historyFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QDialog *historyDialog = new QDialog(this);
        historyDialog->setWindowTitle("History");
        historyDialog->setGeometry(750,500,750,500);


        QVBoxLayout *layout = new QVBoxLayout(historyDialog);
        QListWidget *historyListWidget = new QListWidget(historyDialog);
        layout->addWidget(historyListWidget);


        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            historyListWidget->addItem(line);
        }

        file.close();

        historyDialog->setLayout(layout);
        historyDialog->exec();
    }
}

void AnkaBrowser::showDownloads()
{
    QString homeDir = QDir::homePath();
    QString downloadsFile = homeDir + "/.config/Anka/downloads.txt";

    QFile file(downloadsFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QDialog *downloadsDialog = new QDialog(this);
        downloadsDialog->setWindowTitle("History");
        downloadsDialog->setGeometry(750,500,750,500);


        QVBoxLayout *layout = new QVBoxLayout(downloadsDialog);
        QListWidget *downloadsListWidget = new QListWidget(downloadsDialog);
        layout->addWidget(downloadsListWidget);


        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            downloadsListWidget->addItem(line);
        }

        file.close();

        downloadsDialog->setLayout(layout);
        downloadsDialog->exec();
    }
}

void AnkaBrowser::closeTab(int index)
{
    if(ui->tabs->count() == 1){
        exit(false);
    }
    else{
        ui->tabs->removeTab(index);
    }

}
void AnkaBrowser::browserBack()
{
    QWebEngineView* current_browser = qobject_cast<QWebEngineView*>(ui->tabs->currentWidget());
    current_browser->back();
}
void AnkaBrowser::browserForward()
{
    QWebEngineView* current_browser = qobject_cast<QWebEngineView*>(ui->tabs->currentWidget());
    current_browser->forward();
}
void AnkaBrowser::browserReload()
{
    QWebEngineView* current_browser = qobject_cast<QWebEngineView*>(ui->tabs->currentWidget());
    current_browser->reload();
}
void AnkaBrowser::showSettingsMenu()
{
    QMenu menu;

    QAction* settingsAction = new QAction("Settings");
    connect(settingsAction, &QAction::triggered, this, &AnkaBrowser::openSettings);
    menu.addAction(settingsAction);
    QAction* downloadsAction = new QAction("Downloads");
    connect(downloadsAction, &QAction::triggered, this, &AnkaBrowser::showDownloads);
    menu.addAction(downloadsAction);
    QAction* historyAction = new QAction("History");
    connect(historyAction, &QAction::triggered, this, &AnkaBrowser::showHistory);
    menu.addAction(historyAction);


    QPoint buttonPosition = ui->settings_button->mapToGlobal(QPoint(0, ui->settings_button->height()));
    menu.exec(buttonPosition);

}
void AnkaBrowser::openSettings()
{
    ankabrowsersettings->exec();
}

