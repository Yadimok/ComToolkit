#ifndef COMSETTINGS_H
#define COMSETTINGS_H

#include <QDialog>
#include <QSettings>

#include <windows.h>

namespace Ui {
class ComSettings;
}

class ComSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit ComSettings(QWidget *parent = 0);
    ~ComSettings();

    DWORD getBaudrate();
    BYTE  getStopBits();
    BYTE  getByteSize();
    BYTE  getParity();
    DWORD getDtrState();
    DWORD getRtsState();

    void getLogString(QString log);
    
private:
    Ui::ComSettings *ui;

    void readIniSettings();
    void writeIniSettings();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // COMSETTINGS_H
