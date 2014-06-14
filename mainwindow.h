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

#include "dlgItemSelect.h"
#include "dlgSelectLotNumber.h"
#include "dlgEditItems.h"
#include "dlgCommSettings.h"
#include "settings.h"

#include "dictionary.h"
#include "iniparser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionItemSelect_triggered();
    void on_actionSelectLotNumber_triggered();
    void on_actionLabelSelect_triggered();
    void on_secondsTimer();

    void on_actionEditItems_triggered();

    void on_actionCommSettings_triggered();

private:
    Ui::MainWindow *ui;

    QString lot; //Партида
    QString rawLabelData, labelData; //Данни за етикетната форма на апликатора
    QTimer secondsTimer; //Таймер за обновяване на показанието дата/час

    QHash<QString,QString> hashLabelParams; //Фиксирани параметри на етикета на апликатора
    QSerialPort spScale,spLabel, spTotalLabel;
    Settings settings;

};

#endif // MAINWINDOW_H
