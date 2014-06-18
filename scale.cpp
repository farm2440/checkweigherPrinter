#include "scale.h"


/* Обекта работи в отделна нишка. След като бъде стартиран чете показанието на везната,
 * оценява кога е надхвърлен прага, прави осредняване и завършва работа когато е готов.
 */

Scale::Scale(QObject *parent) :  QObject(parent)
{
    run=false;
}

void Scale::setupThread(QThread *scaleThread)
{
    connect(scaleThread, SIGNAL(started()), this, SLOT(startWork()));
    run=false;
}

void Scale::startWork()
{
    bool ok;
    QTime time;

    _err=0;
    // err=
    //1 - порта не е отворен
    //2 - везната не отговаря
    //3 - отговора на везната е некоректен

    if(!sp.isOpen())
    {//Ако порта не е отворен връща грешка 1
        _err=1;
         goto exitThread;
    }

    sp.write("MSV?\r\n");
    time.restart();
    while(1)
    {
        if(!run) goto exitThread;
        if(time.elapsed()>500)
        {
            _err=2;
            goto exitThread;
        }

        if(sp.canReadLine())
        {
            weight = QString(sp.readLine()).remove('\n').remove('\r').toDouble(&ok);
            if(!ok) _err=3;
            goto exitThread;
        }
        else  thread()->msleep(2); //един байт се предава за ~0.8 мс
    }

exitThread:
    thread()->exit();
}

bool Scale::initSettings(Settings *settings)
{//Отваря и инициализира серииния порт към везната. Връща true ако е ОК и false ако не успее
    _settings = settings;
    if(sp.isOpen()) sp.close();
    sp.setPortName(settings->scalePortName);
    sp.open(QIODevice::ReadWrite);
    sp.setBaudRate((QSerialPort::BaudRate)settings->scaleBaudrate);
    sp.setDataBits((QSerialPort::DataBits)settings->scaleDataBits);
    sp.setParity((QSerialPort::Parity)settings->scaleParity);
    sp.setStopBits((QSerialPort::StopBits)settings->scaleStopBits);
    sp.setFlowControl(QSerialPort::NoFlowControl);

    return sp.isOpen();
}

int Scale::getErrStatus() { return _err; }

