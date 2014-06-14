#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtSerialPort>

struct Settings
{
    //[COMMSETTINGS]
    QString scalePortName;
    int scaleBaudrate;
    int scaleParity;
    int scaleStopBits;
    int scaleDataBits;

    QString labelPrinterPortName;
    int labelPrinterBaudrate;
    int labelPrinterParity;
    int labelPrinterStopBits;
    int labelPrinterDataBits;

    QString totalPrinterPortName;
    int totalPrinterBaudrate;
    int totalPrinterParity;
    int totalPrinterStopBits;
    int totalPrinterDataBits;

    //[OTHER]
    QString password; //Парола за защитени менюта
};

#endif // SETTINGS_H
