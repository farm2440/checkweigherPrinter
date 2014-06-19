#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QPalette>

#include "dlgItemSelect.h"
#include "dlgSelectLotNumber.h"
#include "dlgEditItems.h"
#include "dlgCommSettings.h"
#include "settings.h"
#include "scale.h"

#include "dictionary.h"
#include "iniparser.h"

#define LCD_PRESCALER 4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum PrgState {WAIT_TRIGER_LEVEL, AVERAGING, WAIT_LOW_LEVEL };

private slots:
    void on_actionItemSelect_triggered();
    void on_actionSelectLotNumber_triggered();
    void on_actionLabelSelect_triggered();
    void on_secondsTimer(); //За показанието на часовника
    void on_actionEditItems_triggered();
    void on_actionCommSettings_triggered();
    void on_btnStartStop_clicked();

    void scaleReadingFinished();  //Слота се вика когато приключи четене от везната. В него се отработва програмната логика
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;

    QString lot; //Партида
    QString rawLabelData, labelData; //Данни за етикетната форма на апликатора
    QTimer secondsTimer; //Таймер за обновяване на показанието дата/час

    QHash<QString,QString> hashLabelParams; //Фиксирани параметри на етикета на апликатора
    Settings settings; //съдържанието на настройките се зарежда и пази във файла settings.xml


    QThread scaleThread; //Нишкa за работа с везната
    Scale scale;         //Обект за четене на везната. Изнася се в горната нишка
    int scaleStatus;     //Пази последния код на грешка от везната. За следене на статус
    int avgCounter;
    double avgWeight;


    bool printingMode;    //true в режим етикиране. Управлява се от бутна btnStartStop
    int lcdPrescaler;    //брояч за по-инертно показване на теглото

    PrgState prgState;
};

#endif // MAINWINDOW_H
