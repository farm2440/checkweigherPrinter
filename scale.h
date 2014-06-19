#ifndef SCALE_H
#define SCALE_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QMutex>
#include <QTime>
#include <QByteArray>
#include <QDebug>

#include "settings.h"

class Scale : public QObject
{
    Q_OBJECT
public:
    explicit Scale(QObject *parent = 0);
    ~Scale();

    bool initSettings(Settings *settings); //Получава указател към структура с настройки на сериен порт и др;
    int getErrStatus();
    double weight;
    bool run; //Флаг за управление. Ако стане false нишката повече не се опитва да чете везната и излиз
private:
    Settings *_settings;
    int _err; //0-ok , 1-Серииния порт не е отворен, 2 - таймаут, няма отговор от везната
    QSerialPort sp;


signals:

public slots:
    void setupThread(QThread *scaleThread);
    void startWork();
};

#endif // SCALE_H
