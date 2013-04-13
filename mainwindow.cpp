#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->addWidget(ui->lSender);
    ui->statusBar->addWidget(ui->lReceiver);
    ui->statusBar->addWidget(ui->lDate);
    ui->statusBar->addWidget(ui->lTime);

    m_countReceivePackage = 0;
    m_countSendPackage = 0;

    m_strFileName = "";

    //------------COM
    for (size_t idx=1; idx <256; ++idx)
    {
        QString str = (idx < 10) ? tr("COM%1").arg(idx) : tr("\\\\.\\COM%1").arg(idx);
        bool check = false;

        HANDLE hPort = CreateFileA(str.toStdString().c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
        if (hPort == INVALID_HANDLE_VALUE)
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_ACCESS_DENIED)
                check = true;
        }
        else
        {
            check = true;
            CloseHandle(hPort);
        }
        if (check)
            ui->cboxCom->addItem(str);
    }
    Sleep(100);

    comPort = new WorkComPort();
    comPort->start();
    connect(comPort, SIGNAL(send_data(QByteArray)), this, SLOT(receive_data(QByteArray)));

    setsPort = new ComSettings;

    QRegExp regExp("[0-9]+");
    QValidator *validator = new QRegExpValidator(regExp, 0);
    ui->leNumber->setValidator(validator);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCurrentTime()));
    timer->start(1000);

    updateCurrentDate();

    timerSender = new QTimer();
    connect(timerSender, SIGNAL(timeout()), SLOT(on_pbStart_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;

    delete comPort;
    comPort = 0;

    delete setsPort;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    on_pbClosePort_clicked();
    qApp->quit();
}

void MainWindow::on_aExit_triggered()
{
    qApp->quit();
}

void MainWindow::updateCurrentTime()
{
    ui->lTime->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::updateCurrentDate()
{
    ui->lDate->setText(QDate::currentDate().toString(Qt::SystemLocaleLongDate));
}

void MainWindow::on_aAboutQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_aClearSendData_triggered()
{
    ui->teSendData->clear();
    m_countSendPackage = 0;
}

void MainWindow::on_aClearReceiveData_triggered()
{
    ui->tbReceiveData->clear();
    m_countReceivePackage = 0;
}


void MainWindow::on_pbOpenPort_clicked()
{
    comPort->setStringPort(ui->cboxCom->currentText());

    if (comPort->isBusyComPort())
        comPort->closeComPort();

    comPort->setComPortSettings(setsPort->getBaudrate(),
                             setsPort->getStopBits(),
                             setsPort->getByteSize(),
                             setsPort->getParity(),
                             setsPort->getDtrState(),
                             setsPort->getRtsState());

    if (comPort->openComPort())
    {
        if (comPort->configComPort())
        {
            setsPort->getLogString("Set configuration.");
            ui->pbOpenPort->setDisabled(true);
            ui->pbClosePort->setEnabled(true);
        }
        else
        {
            setsPort->getLogString("Configuration failed.");
            ui->pbOpenPort->setEnabled(true);
            ui->pbClosePort->setDisabled(true);
        }
    }
    else
    {
        ui->pbOpenPort->setEnabled(true);
        ui->pbClosePort->setDisabled(true);
    }
    setsPort->getLogString(comPort->getStringMessage());
    m_countReceivePackage = 0;
    m_countSendPackage = 0;
}

void MainWindow::on_pbClosePort_clicked()
{
    comPort->closeComPort();

    ui->pbClosePort->setDisabled(true);
    ui->pbOpenPort->setEnabled(true);

    setsPort->getLogString(comPort->getStringMessage());
}

void MainWindow::on_aConfigPort_triggered()
{
    if (setsPort->isVisible())
        setsPort->hide();
    else
        setsPort->show();
}

void MainWindow::receive_data(QByteArray bArray)
{
    ui->tbReceiveData->append(bArray.toHex());

    m_countReceivePackage += bArray.size();
    ui->lReceiver->setText(tr("Receive packs - %1").arg(countReceivePackage));
}

void MainWindow::on_pb_Send_clicked()
{
    QString str = ui->teSendData->toPlainText();
    comPort->writeToComPort(str);

    m_countSendPackage += str.size();
    ui->lSender->setText(tr("Sended packs - %1").arg(countSendPackage));
}

void MainWindow::on_pbStart_clicked()
{
    if (ui->leNumber->text() != "")
    {
        int time = ui->leNumber->text().toInt();
        timerSender->start(time);

        QString str = ui->teSendData->toPlainText();
        comPort->writeToComPort(str);

        m_countSendPackage += str.size();
        ui->lSender->setText(tr("Sended packs - %1").arg(m_countSendPackage));

        ui->pbStart->setDisabled(true);
        ui->pbStop->setEnabled(true);
    }
}


void MainWindow::on_pbStop_clicked()
{
    timerSender->stop();

    ui->pbStop->setDisabled(true);
    ui->pbStart->setEnabled(true);
}

void MainWindow::on_gbTimer_clicked()
{
    if (ui->gbTimer->isChecked())
    {
        ui->pb_Send->setDisabled(true);
    }
    else
    {
        ui->pb_Send->setEnabled(true);
    }
}

void MainWindow::on_aAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("<b>ComToolkit for change data between COM ports</b>");
    msgBox.setInformativeText("Developed by Yadimok, 2013");
    msgBox.exec();
}

void MainWindow::on_aLoadFile_triggered()
{
    m_countSendPackage = 0;
    m_strFileName = QFileDialog::getOpenFileName(0, "Open file", QDir::currentPath());
    QFile resultFile(m_strFileName);

    if(!resultFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, "Error!", "File not open!");
        return;
    }

    while (!resultFile.atEnd())
    {
        QString str = resultFile.read(4096);
        m_countSendPackage += str.size();
        comPort->writeToComPort(str);
        qApp->processEvents();
    }
    ui->lSender->setText(tr("Sended packs - %1").arg(m_countSendPackage));
}

void MainWindow::on_aLoadFiles_triggered()
{
    m_countSendPackage = 0;
    QString str;
    QStringList strList = QFileDialog::getOpenFileNames(0, "Choose one or more files for processing", QDir::currentPath());

    for (int index=0; index<strList.count(); index++)
    {
        str = strList.at(index);
        QFile tmpFile(str);
        if (!tmpFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error!", "File not open!");
            return;
        }

        while (!tmpFile.atEnd())
        {
            QString data = tmpFile.read(4096);
            m_countSendPackage += str.size();
            comPort->writeToComPort(data);
            qApp->processEvents();
        }
    }
    ui->lSender->setText(tr("Sended packs - %1").arg(m_countSendPackage));
}

void MainWindow::on_aSaveToFile_triggered()
{
    if (ui->tbReceiveData->toPlainText() != "")
    {
        QString str = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath());

        QFile resultFile(str);
        resultFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
        QTextStream outTextStream(&resultFile);
        outTextStream << ui->tbReceiveData->toPlainText();
        resultFile.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("<b>Text empty</b>");
        msgBox.exec();
    }

}
