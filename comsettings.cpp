#include "comsettings.h"
#include "ui_comsettings.h"

ComSettings::ComSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComSettings)
{
    ui->setupUi(this);


    //------------Baudrate
    ui->cboxBaudrate->addItem("110");
    ui->cboxBaudrate->addItem("300");
    ui->cboxBaudrate->addItem("600");
    ui->cboxBaudrate->addItem("1200");
    ui->cboxBaudrate->addItem("2400");
    ui->cboxBaudrate->addItem("4800");
    ui->cboxBaudrate->addItem("9600");
    ui->cboxBaudrate->addItem("14400");
    ui->cboxBaudrate->addItem("19200");
    ui->cboxBaudrate->addItem("38400");
    ui->cboxBaudrate->addItem("56000");
    ui->cboxBaudrate->addItem("57600");
    ui->cboxBaudrate->addItem("115200");
    ui->cboxBaudrate->addItem("128000");
    ui->cboxBaudrate->addItem("256000");

    //------------Stop Bits
    ui->cboxStopBits->addItem("1");
    ui->cboxStopBits->addItem("1_5");
    ui->cboxStopBits->addItem("2");

    //------------Data Bits
    ui->cboxDataBits->addItem("5");
    ui->cboxDataBits->addItem("6");
    ui->cboxDataBits->addItem("7");
    ui->cboxDataBits->addItem("8");

    //------------Parity
    ui->cboxParity->addItem("NO");
    ui->cboxParity->addItem("ODD");
    ui->cboxParity->addItem("EVEN");
    ui->cboxParity->addItem("MARK");
    ui->cboxParity->addItem("SPACE");

    readIniSettings();
}

ComSettings::~ComSettings()
{
    delete ui;
}

void ComSettings::closeEvent(QCloseEvent* /*event*/)
{
    writeIniSettings();
}

DWORD ComSettings::getBaudrate()
{
    DWORD value(0);

    switch (ui->cboxBaudrate->currentIndex())
    {
    case 0:
        value = CBR_110;
        break;
    case 1:
        value = CBR_300;
        break;
    case 2:
        value = CBR_600;
        break;
    case 3:
        value = CBR_1200;
        break;
    case 4:
        value = CBR_2400;
        break;
    case 5:
        value = CBR_4800;
        break;
    case 6:
        value = CBR_9600;
        break;
    case 7:
        value = CBR_14400;
        break;
    case 8:
        value = CBR_19200;
        break;
    case 9:
        value = CBR_38400;
        break;
    case 10:
        value = CBR_56000;
        break;
    case 11:
        value = CBR_57600;
        break;
    case 12:
        value = CBR_115200;
        break;
    case 13:
        value = CBR_128000;
        break;
    case 14:
        value = CBR_256000;
        break;
    }
    return value;
}

BYTE ComSettings::getStopBits()
{
    BYTE value(0);
    switch (ui->cboxStopBits->currentIndex())
    {
    case 0:
        value = ONESTOPBIT;
        break;
    case 1:
        value = ONE5STOPBITS;
        break;
    case 2:
        value = TWOSTOPBITS;
        break;
    }
    return value;
}

BYTE ComSettings::getByteSize()
{
    BYTE value(0);
    switch (ui->cboxDataBits->currentIndex())
    {
    case 0:
        value = 5;
        break;
    case 1:
        value = 6;
        break;
    case 2:
        value = 7;
        break;
    case 3:
        value = 8;
        break;
    }
    return value;
}

BYTE ComSettings::getParity()
{
    BYTE value(0);
    switch(ui->cboxParity->currentIndex())
    {
    case 0:
        value = NOPARITY;
        break;
    case 1:
        value = ODDPARITY;
        break;
    case 2:
        value = EVENPARITY;
        break;
    case 3:
        value = MARKPARITY;
        break;
    case 4:
        value = SPACEPARITY;
        break;
    }
    return value;
}

DWORD ComSettings::getDtrState()
{
    return (ui->chckBoxDtr->checkState()) ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
}

DWORD ComSettings::getRtsState()
{
    return (ui->chckBoxRts->checkState()) ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
}

void ComSettings::getLogString(QString log)
{
    ui->textBrowser->append(log);
}

void ComSettings::readIniSettings()
{
    QSettings settings("Qt_ComToolkit.ini", QSettings::IniFormat);

    settings.beginGroup("window");
    QPoint pos = settings.value("pos", QPoint(400, 400)).toPoint();
    QSize size = settings.value("size", QSize(341, 351)).toSize();
    settings.endGroup();

    move(pos);
    resize(size);

    settings.beginGroup("port");
    ui->cboxBaudrate->setCurrentIndex(settings.value("baudrate", 12).toInt());
    ui->cboxStopBits->setCurrentIndex(settings.value("stop_bits", 0).toInt());
    ui->cboxDataBits->setCurrentIndex(settings.value("byte_size", 3).toInt());
    ui->cboxParity->setCurrentIndex(settings.value("parity", 0).toInt());
    ui->chckBoxDtr->setChecked(settings.value("dtr", true).toBool());
    ui->chckBoxRts->setChecked(settings.value("rts", true).toBool());
    settings.endGroup();

}

void ComSettings::writeIniSettings()
{
    QSettings settings("Qt_ComToolkit.ini", QSettings::IniFormat);

    settings.beginGroup("window");
    settings.setValue("pos", this->pos());
    settings.setValue("size", this->size());
    settings.endGroup();

    settings.beginGroup("port");
    settings.setValue("baudrate", ui->cboxBaudrate->currentIndex());
    settings.setValue("stop_bits", ui->cboxStopBits->currentIndex());
    settings.setValue("byte_size", ui->cboxDataBits->currentIndex());
    settings.setValue("parity", ui->cboxParity->currentIndex());
    settings.setValue("dtr", ui->chckBoxDtr->checkState());
    settings.setValue("rts", ui->chckBoxRts->checkState());
    settings.endGroup();
}
