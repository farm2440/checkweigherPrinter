#include "dlgCommSettings.h"
#include "ui_dlgCommSettings.h"

DlgCommSettings::DlgCommSettings(QWidget *parent, Settings *settings) :    QDialog(parent),    ui(new Ui::DlgCommSettings)
{
    ui->setupUi(this);

    _settings=settings;

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QString> portNames;
    QList<QString> baudRates;

    foreach(QSerialPortInfo info, ports)    portNames.append(info.portName());
    foreach(int baud, QSerialPortInfo::standardBaudRates()) baudRates.append(QString::number(baud));

    ui->comboLabelPortName->addItems(portNames);
    ui->comboLabelBaud->addItems(baudRates);
    ui->comboScalePortName->addItems(portNames);
    ui->comboScaleBaud->addItems(baudRates);
    ui->comboTotalPortName->addItems(portNames);
    ui->comboTotalBaud->addItems(baudRates);

    ui->comboLabelPortName->setCurrentText(settings->labelPrinterPortName);
    ui->comboScalePortName->setCurrentText(settings->scalePortName);
    ui->comboTotalPortName->setCurrentText(settings->totalPrinterPortName);

    ui->comboLabelBaud->setCurrentText(QString::number(settings->labelPrinterBaudrate));
    ui->comboTotalBaud->setCurrentText(QString::number(settings->totalPrinterBaudrate));
    ui->comboScaleBaud->setCurrentText(QString::number(settings->scaleBaudrate));

    ui->rbLabelDataBits8->setChecked(settings->labelPrinterDataBits==8);
    ui->rbLabelDataBits7->setChecked(settings->labelPrinterDataBits==7);
    ui->rbLabelStopBits1->setChecked(settings->labelPrinterStopBits==1);
    ui->rbLabelStopBits2->setChecked(settings->labelPrinterStopBits==2);
    ui->rbLabelParityNone->setChecked(settings->labelPrinterParity==0);
    ui->rbLabelParityOdd->setChecked(settings->labelPrinterParity==3);
    ui->rbLabelParityEven->setChecked(settings->labelPrinterParity==2);

    ui->rbScaleDataBits8->setChecked(settings->scaleDataBits==8);
    ui->rbScaleDataBits7->setChecked(settings->scaleDataBits==7);
    ui->rbScaleStopBits1->setChecked(settings->scaleStopBits==1);
    ui->rbScaleStopBits2->setChecked(settings->scaleStopBits==2);
    ui->rbScaleParityNone->setChecked(settings->scaleParity==0);
    ui->rbScaleParityOdd->setChecked(settings->scaleParity==3);
    ui->rbScaleParityEven->setChecked(settings->scaleParity==2);

    ui->rbTotalDataBits8->setChecked(settings->totalPrinterDataBits==8);
    ui->rbTotalDataBits7->setChecked(settings->totalPrinterDataBits==7);
    ui->rbTotalStopBits1->setChecked(settings->totalPrinterStopBits==1);
    ui->rbTotalStopBits2->setChecked(settings->totalPrinterStopBits==2);
    ui->rbTotalParityNone->setChecked(settings->totalPrinterParity==0);
    ui->rbTotalParityOdd->setChecked(settings->totalPrinterParity==3);
    ui->rbTotalParityEven->setChecked(settings->totalPrinterParity==2);
}

DlgCommSettings::~DlgCommSettings()
{
    delete ui;
}

void DlgCommSettings::on_btOK_clicked()
{
    //Направените настройки се отразяват в settings
    _settings->scalePortName = ui->comboScalePortName->currentText();
    _settings->scaleBaudrate = ui->comboScaleBaud->currentText().toInt();
    _settings->scaleDataBits = ui->rbScaleDataBits8->isChecked() ? 8 : 7;
    _settings->scaleStopBits = ui->rbScaleStopBits1->isChecked() ? 1 : 2;
    if(ui->rbScaleParityNone->isChecked()) _settings->scaleParity = 0;
    else if(ui->rbScaleParityOdd->isChecked()) _settings->scaleParity=3;
    else _settings->scaleParity=2;

     _settings->totalPrinterPortName = ui->comboTotalPortName->currentText();
     _settings->totalPrinterBaudrate = ui->comboTotalBaud->currentText().toInt();
     _settings->totalPrinterDataBits = ui->rbTotalDataBits8->isChecked() ? 8 : 7;
     _settings->totalPrinterStopBits = ui->rbTotalStopBits1->isChecked() ? 1 : 2;
     if(ui->rbTotalParityNone->isChecked()) _settings->totalPrinterParity = 0;
     else if(ui->rbTotalParityOdd->isChecked()) _settings->totalPrinterParity=3;
     else _settings->totalPrinterParity=2;

     _settings->labelPrinterPortName = ui->comboLabelPortName->currentText();
     _settings->labelPrinterBaudrate = ui->comboLabelBaud->currentText().toInt();
     _settings->labelPrinterDataBits = ui->rbLabelDataBits8->isChecked() ? 8 : 7;
     _settings->labelPrinterStopBits = ui->rbLabelStopBits1->isChecked() ? 1 : 2;
     if(ui->rbLabelParityNone->isChecked()) _settings->labelPrinterParity = 0;
     else if(ui->rbLabelParityOdd->isChecked()) _settings->labelPrinterParity=3;
     else _settings->labelPrinterParity=2;

     //запис на настройките в ini файл
    QFile iniFile("settings.ini");

    iniFile.open(QIODevice::WriteOnly);
    iniFile.resize(0);
    QTextStream s(&iniFile);
    s<< "[COMMSETTINGS]\r\n";
    s<< "\r\n";
    s<< "labelPrinterPortName=" << _settings->labelPrinterPortName << "\r\n";
    s<< "labelPrinterBaudrate=" << _settings->labelPrinterBaudrate << "\r\n";
    s<< "labelPrinterDataBits=" << _settings->labelPrinterDataBits << "\r\n";
    s<< "labelPrinterParity=" << _settings->labelPrinterParity << "\r\n";
    s<< "labelPrinterStopBits=" << _settings->labelPrinterStopBits << "\r\n";
    s<< "\r\n";
    s<< "totalPrinterPortName=" << _settings->totalPrinterPortName << "\r\n";
    s<< "totalPrinterBaudrate=" << _settings->totalPrinterBaudrate << "\r\n";
    s<< "totalPrinterDataBits=" << _settings->totalPrinterDataBits << "\r\n";
    s<< "totalPrinterParity=" << _settings->totalPrinterParity << "\r\n";
    s<< "totalPrinterStopBits=" << _settings->totalPrinterStopBits << "\r\n";
    s<< "\r\n";
    s<< "scalePortName=" << _settings->scalePortName << "\r\n";
    s<< "scaleBaudrate=" << _settings->scaleBaudrate << "\r\n";
    s<< "scaleDataBits=" << _settings->scaleDataBits << "\r\n";
    s<< "scaleParity=" << _settings->scaleParity << "\r\n";
    s<< "scaleStopBits=" << _settings->scaleStopBits << "\r\n";
    s<< "\r\n";
    s<< "[OTHER]\r\n";
    s<< "password=" << _settings->password << "\r\n";
    iniFile.close();

    QMessageBox::information(this,"ВАЖНО!" , "Новите настройки са записани. Програмата трябва да бъде рестартирана.");
    accept();
}
