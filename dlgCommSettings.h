#ifndef DLGCOMMSETTINGS_H
#define DLGCOMMSETTINGS_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QList>

#include "settings.h"

namespace Ui {
class DlgCommSettings;
}

class DlgCommSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCommSettings(QWidget *parent = 0, Settings *settings = 0);
    ~DlgCommSettings();

private slots:
    void on_btOK_clicked();

private:
    Ui::DlgCommSettings *ui;
    Settings *_settings;
};

#endif // DLGCOMMSETTINGS_H
