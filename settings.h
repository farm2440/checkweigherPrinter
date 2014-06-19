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

    //[LABELING]
    int threshold; //Тегло от везната над което започва отчитането
    int lowThreshold; //Тегло под което трябва да падне везната за да започне нов цикъл
    int mesureDelay; //Задръжка в милисекунди след достигане на прага
    int averagingCicles; //Брой измервания които се правят и резултата им се осреднява
    int totalCount; //Брой измервания на които се печата сумарен етикет

    //[OTHER]
    QString password; //Парола за защитени менюта
};

#endif // SETTINGS_H
