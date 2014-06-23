#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette pal = ui->frame->palette();
    pal.setColor(QPalette::Window, Qt::white);
    ui->frame->setPalette(pal);

    pal = ui->lcdNumber->palette();
    pal.setColor(QPalette::Background, QColor::fromRgb(240,250,240));
    ui->lcdNumber->setPalette(pal);

    lot="";
    rawLabelData="";
    labelData="";
    ui->lblLot->setText("Партида:");
    ui->lblLabelFormFile->setText("Етикет:");
    ui->textBrowser_ItemInfo->setText("");
    printingMode=false;

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
        s<< "[PRINT]\r\n";
        s<< "lowThreshold=50\r\n";
        s<< "threshold=100\r\n";
        s<< "averagingCicles=3\r\n";
        s<< "mesureDelay=15\r\n";
        s<< "totalCount=0\r\n";
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

    settings.averagingCicles = iniparser_getint(dict,QString("PRINT:averagingCicles").toLatin1().data(),3);
    settings.mesureDelay = iniparser_getint(dict,QString("PRINT:mesureDelay").toLatin1().data(),15);
    settings.threshold = iniparser_getint(dict,QString("PRINT:threshold").toLatin1().data(),100);
    settings.lowThreshold = iniparser_getint(dict,QString("PRINT:lowThreshold").toLatin1().data(),50);
    settings.totalCount = iniparser_getint(dict,QString("PRINT:totalCount").toLatin1().data(),0);

    settings.password  = QString( iniparser_getstring(dict, QString("OTHER:password").toLatin1().data(), QString("").toLatin1().data()) );

    delete dict;
    delete fn;

    //Обектите за работа със сериини портове са изнесени в други нишки
    scale.setupThread(&scaleThread);
    scale.moveToThread(&scaleThread);
    connect(&scaleThread, SIGNAL(finished()), this, SLOT(scaleReadingFinished()));
    scaleStatus=-1;

    lcdPrescaler= LCD_PRESCALER;
    prgState = WAIT_LOW_LEVEL;

    //Стартиране на везната
    scale.initSettings(&settings);
    scale.run=true;
    if(scaleThread.isRunning()) qDebug() << "scaleThread is still running.";
    else scaleThread.start();

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

void MainWindow::on_btnStartStop_clicked()
{
    printingMode=!printingMode;

    if(printingMode)
    {
        ui->btnStartStop->setText("СТОП");
    }
    else
    {
        ui->btnStartStop->setText("СТАРТ");
    }
}

void MainWindow::scaleReadingFinished()
{
    //Този слот се вика когато нишката за четене от везната приключи.
    //Прочетената стойност се изобразява. Проверява се кода на грешка и ако е различен от 0 се показва аларма.
    //Ако е в режим етикиране се отработва логиката на работа.
    //Нишката за четене отново се презапуска.

    if(!scale.run) return; //При излизане везната се спира (run=false) и порта се затваря

    double weight = scale.weight;

    //Изобразяването става веднъж на LCD_PRESCALER прочитания
    lcdPrescaler--;
    if(lcdPrescaler==0)
    {
        ui->lcdNumber->display(weight);
        lcdPrescaler=LCD_PRESCALER;
    }

    //Проверка на кода за грешка
    if(scaleStatus != scale.getErrStatus())
    {
        scaleStatus = scale.getErrStatus();

        QPalette pal = ui->frameScaleStatus->palette();
        switch(scaleStatus)
        {
            case 0: //ok
                pal.setColor(QPalette::Background, Qt::green);
                break;
            case 1: //Порта не е отворен
            case 2: //Няма връзка с везната
            case 3: //Некоректни данни
                pal.setColor(QPalette::Background, Qt::red);
                break;
            default:
                break;
        }
        ui->frameScaleStatus->setPalette(pal);

        if(scaleStatus)
        {
            scaleThread.start();
            return;//При грешка не се отработва логика
        }
    }

    if(printingMode)
    {
        switch(prgState)
        {
            case WAIT_LOW_LEVEL : //Чака везната да се успокои и да падне под settings.lowThreshold
                if(weight < settings.lowThreshold)
                {
                    prgState = WAIT_TRIGER_LEVEL;
                    qDebug() << "prgState = WAIT_TRIGER_LEVEL";
                }
                break;
            case WAIT_TRIGER_LEVEL :
                if(weight > settings.threshold)
                {
                    this->thread()->msleep(settings.mesureDelay);
                    avgWeight=0;
                    avgCounter=settings.averagingCicles;
                    prgState = AVERAGING;
                    qDebug() << "prgState = AVERAGING";
                }
                break;
            case AVERAGING:
                if(avgCounter)
                {
                    avgCounter--;
                    avgWeight+=weight;
                }
                else
                {
                    avgWeight /= settings.averagingCicles;
                    qDebug() << "Усреднено тегло = " << avgWeight;
//TODO: печат на етикет
//TODO: логика за тотал етикет и натрупване на тотал
                    prgState = WAIT_LOW_LEVEL;
                    qDebug() << "prgState = WAIT_LOW_LEVEL";
                }
                break;
        }
    }

    scaleThread.start();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    scale.run = false;
    scaleThread.wait(1000);
    event->accept();
}

void MainWindow::on_btnZeroize_clicked()
{
/*
 * http://boorick.livejournal.com/30856.html
 * http://logix4u.net/parallel-port/26-inpoutx64dll-for-win-xp-64-bit
 * http://logix4u.net/InpOutBinaries.zip
 * http://electrosofts.com/parallel/parallelwin.html
 *
 */

    HINSTANCE hLib;
    inpfuncPtr inp32;
    oupfuncPtr oup32;

    /* Загружаем необходимую dll-ку */
    hLib = LoadLibrary( (LPCWSTR)QString("inpout32.dll").utf16() );

    if (hLib == NULL)
    {
        qDebug() << "LoadLibrary Failed.";
        return;
    }

    /* получаем адрес функций */
    inp32 = (inpfuncPtr) GetProcAddress(hLib, "Inp32");

    if (inp32 == NULL)
    {
       qDebug() << "GetProcAddress for Inp32 Failed.";
         return;
    }

    oup32 = (oupfuncPtr) GetProcAddress(hLib, "Out32");

    if (oup32 == NULL)
    {
        qDebug() << "GetProcAddress for Oup32 Failed.";
        return;
    }


    // дальше просто пример использования функций
    // их у нас две:
    // void oup32(short address, short data)
    // short Inp32(short address)

    (oup32)(0x378,0xFF);
    qDebug() << "0xFF was writed to LPT port";

    // при выходе не забыть выгрузить библиотеку
    FreeLibrary(hLib);
}
