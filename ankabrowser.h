#ifndef ANKABROWSER_H
#define ANKABROWSER_H

#include <QMainWindow>
#include <QString>
#include <QWebEngineView>
#include "ankabrowsersettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AnkaBrowser; }
QT_END_NAMESPACE

class AnkaBrowser : public QMainWindow
{
    Q_OBJECT

public:
    AnkaBrowser(QWidget *parent = nullptr);
    ~AnkaBrowser();

    void addNewTab(const QString &url, const QString &label);
    QString truncateTabText(const QString& text);
    void updateUrl(const QUrl& url);
    void updateTitle(QWebEngineView* browser, const QString& title);
    void closeTab(int index);
    void browserBack();
    void browserForward();
    void browserReload();
    void showSettingsMenu();
    void openSettings();
    void showDownloads();
    void showHistory();
    void openFile();
    void handleDownloadRequest(QWebEngineDownloadRequest *download);
private:
    Ui::AnkaBrowser *ui;
    AnkaBrowserSettings *ankabrowsersettings;
};
#endif // ANKABROWSER_H
