#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lot="";
    rawLabelData="";
    labelData="";
    ui->lblLot->setText("Партида:");
    ui->lblLabelFormFile->setText("Етикет:");
    ui->textBrowser_ItemInfo->setText("");

    connect(&secondsTimer, SIGNAL(timeout()), this, SLOT(on_secondsTimer()));
    secondsTimer.start(1000);


    //зареждане на настройки от ini файл
    QFile iniFile("settings.ini");
    if(!iniFile.exists())
    {
        //Файла не съществува. Създава се нов и в него се записват настройки по подразбиране
        qDebug("ERR: failed to open settings.ini. Creating new with default values.");
        iniFile.open(QIODevice::WriteOnly);
        QTextStream s(&iniFile);
        s<< "[COMMSETTINGS]\r\n";

        s<< "\r\n";
        s<< "labelPrinterPortName=\r\n";
        s<< "labelPrinterBaudrate=9600\r\n";
        s<< "labelPrinterDataBits=8\r\n";
        s<< "labelPrinterParity=0\r\n";
        s<< "labelPrinterStopBits=1\r\n";
        s<< "\r\n";
        s<< "totalPrinterPortName=\r\n";
        s<< "totalPrinterBaudrate=9600\r\n";
        s<< "totalPrinterDataBits=8\r\n";
        s<< "totalPrinterParity=0\r\n";
        s<< "totalPrinterStopBits=1\r\n";
        s<< "\r\n";
        s<< "scalePortName=\r\n";
        s<< "scaleBaudrate=9600\r\n";
        s<< "scaleDataBits=8\r\n";
        s<< "scaleParity=0\r\n";
        s<< "scaleStopBits=1\r\n";
        s<< "\r\n";
        s<< "[OTHER]\r\n";
        s<< "password=\r\n";
        iniFile.close();
    }

    char * fn = new char[40];
    strcpy(fn,"settings.ini");
    dictionary *dict;
    dict = iniparser_load(fn);
    if(dict == NULL)   qDebug("ERR: Failed to load dictionary from settings.ini");

    settings.scalePortName        = QString( iniparser_getstring(dict, QString("COMMSETTINGS:scalePortName").toLatin1().data(), QString("").toLatin1().data()) );
    settings.labelPrinterPortName = QString( iniparser_getstring(dict, QString("COMMSETTINGS:labelPrinterPortName").toLatin1().data(), QString("").toLatin1().data()) );
    settings.totalPrinterPortName = QString( iniparser_getstring(dict, QString("COMMSETTINGS:totalPrinterPortName").toLatin1().data(), QString("").toLatin1().data()) );

    settings.labelPrinterBaudrate = iniparser_getint(dict,QString("COMMSETTINGS:labelPrinterBaudrate").toLatin1().data(),9600);
    settings.labelPrinterDataBits = iniparser_getint(dict,QString("COMMSETTINGS:labelPrinterDataBits").toLatin1().data(),8);
    settings.labelPrinterParity   = iniparser_getint(dict,QString("COMMSETTINGS:labelPrinterParity").toLatin1().data(),0);
    settings.labelPrinterStopBits = iniparser_getint(dict,QString("COMMSETTINGS:labelPrinterStopBits").toLatin1().data(),1);

    settings.totalPrinterBaudrate = iniparser_getint(dict,QString("COMMSETTINGS:totalPrinterBaudrate").toLatin1().data(),9600);
    settings.totalPrinterDataBits = iniparser_getint(dict,QString("COMMSETTINGS:totalPrinterDataBits").toLatin1().data(),8);
    settings.totalPrinterParity   = iniparser_getint(dict,QString("COMMSETTINGS:totalPrinterParity").toLatin1().data(),0);
    settings.totalPrinterStopBits = iniparser_getint(dict,QString("COMMSETTINGS:totalPrinterStopBits").toLatin1().data(),1);

    settings.scaleBaudrate = iniparser_getint(dict,QString("COMMSETTINGS:scaleBaudrate").toLatin1().data(),9600);
    settings.scaleDataBits = iniparser_getint(dict,QString("COMMSETTINGS:scaleDataBits").toLatin1().data(),8);
    settings.scaleParity   = iniparser_getint(dict,QString("COMMSETTINGS:scaleParity").toLatin1().data(),0);
    settings.scaleStopBits = iniparser_getint(dict,QString("COMMSETTINGS:scaleStopBits").toLatin1().data(),1);

    settings.password  = QString( iniparser_getstring(dict, QString("OTHER:password").toLatin1().data(), QString("").toLatin1().data()) );

    delete dict;
    delete fn;

    //Настройка на сериините портове
    spScale.setPortName(settings.scalePortName);
    spScale.setFlowControl(QSerialPort::NoFlowControl);
    spScale.setBaudRate((QSerialPort::BaudRate) settings.scaleBaudrate);
    spScale.setDataBits((QSerialPort::DataBits) settings.scaleDataBits);
    spScale.setParity((QSerialPort::Parity) settings.scaleParity);
    spScale.setStopBits((QSerialPort::StopBits) settings.scaleStopBits);

    spLabel.setPortName(settings.labelPrinterPortName);
    spLabel.setFlowControl(QSerialPort::NoFlowControl);
    spLabel.setBaudRate((QSerialPort::BaudRate) settings.labelPrinterBaudrate);
    spLabel.setDataBits((QSerialPort::DataBits) settings.labelPrinterDataBits);
    spLabel.setParity((QSerialPort::Parity) settings.labelPrinterParity);
    spLabel.setStopBits((QSerialPort::StopBits) settings.scaleStopBits);

    spTotalLabel.setPortName(settings.totalPrinterPortName);
    spTotalLabel.setFlowControl(QSerialPort::NoFlowControl);
    spTotalLabel.setBaudRate((QSerialPort::BaudRate) settings.totalPrinterBaudrate);
    spTotalLabel.setDataBits((QSerialPort::DataBits) settings.totalPrinterDataBits);
    spTotalLabel.setParity((QSerialPort::Parity) settings.totalPrinterParity);
    spTotalLabel.setStopBits((QSerialPort::StopBits)settings.totalPrinterStopBits);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionItemSelect_triggered()
{
    DlgItemSelect dlg;
    if(dlg.exec()== QDialog::Accepted)
    {
        ui->textBrowser_ItemInfo->setHtml(dlg.htmlParams);
        hashLabelParams = dlg.hashParams;
    }
}

void MainWindow::on_actionSelectLotNumber_triggered()
{//Избор на партида
    DlgSelectLotNumber dlg;
    dlg.setLot(lot);
    if(dlg.exec()== QDialog::Accepted) lot=dlg.getLot();

    ui->lblLot->setText("Партида: " + lot);
}

void MainWindow::on_actionLabelSelect_triggered()
{
    //Избор на файл за етикет на апликатора
    QFileDialog dlg(this,
                    "Избор на етикетна форма за апликатора",
                    "C:\\",
                    "Етикет (*.frm *.lbl)");
    dlg.setFileMode(QFileDialog::ExistingFile);
    if(dlg.exec()!=QDialog::Accepted) return;

    QString fileName = dlg.selectedFiles().at(0);
    QFile file(fileName,this);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,"ГРЕШКА","Възникна грешка при опит за отваряне на файла. Етикетната форма на апликатора няма да бъде заредена);");
        return;
    }

    ui->lblLabelFormFile->setText("Етикет: " + file.fileName());
    QTextStream in(&file);
    rawLabelData = in.readAll();
    qDebug() << "Файл етикетна форма:\n" << rawLabelData;
}

void MainWindow::on_secondsTimer()
{//Обновяване на дата/час
    QDateTime now = QDateTime::currentDateTime();
    ui->lblDateTime->setText(now.toString("d/M/yyyy hh:mm:ss"));
}

void MainWindow::on_actionEditItems_triggered()
{
    DlgEditItems dlg(this);
    dlg.exec();
}

void MainWindow::on_actionCommSettings_triggered()
{
    DlgCommSettings dlg(this, &settings);

    if(dlg.exec() == QDialog::Accepted)
    {

    }
}
