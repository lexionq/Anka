#ifndef ANKABROWSERSETTINGS_H
#define ANKABROWSERSETTINGS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class AnkaBrowserSettings;
}

class AnkaBrowserSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AnkaBrowserSettings(QWidget *parent = nullptr);
    ~AnkaBrowserSettings();
    void openColorDialog();
    void onButtonClicked(QAbstractButton *button);
    void deleteHistory();
    void deleteBookmarks();

private:
    Ui::AnkaBrowserSettings *ui;
};

#endif // ANKABROWSERSETTINGS_H
