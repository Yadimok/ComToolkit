#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>

#include "threadcom.h"
#include "comsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();
    
private slots:
    void on_aExit_triggered();
    void on_aAboutQt_triggered();
    void on_aClearSendData_triggered();
    void on_aClearReceiveData_triggered();

    void updateCurrentTime();
    void on_pbOpenPort_clicked();
    void on_pbClosePort_clicked();

    void on_aConfigPort_triggered();
    void on_pb_Send_clicked();
    void receive_data(QByteArray bArray);
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_gbTimer_clicked();
    void on_aAbout_triggered();
    void on_aLoadFile_triggered();

    void on_aLoadFiles_triggered();

    void on_aSaveToFile_triggered();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow  *ui;

    WorkComPort     *comPort;
    ComSettings     *setsPort;

    QTimer          *timer;
    QTimer          *timerSender;

    QString         m_strFileName;

    void updateCurrentDate();

    long long m_countReceivePackage;
    long long m_countSendPackage;
};

#endif // MAINWINDOW_H
