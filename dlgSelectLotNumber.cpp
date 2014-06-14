#include "dlgSelectLotNumber.h"
#include "ui_dlgSelectLotNumber.h"

DlgSelectLotNumber::DlgSelectLotNumber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSelectLotNumber)
{
    ui->setupUi(this);
}

DlgSelectLotNumber::~DlgSelectLotNumber()
{
    delete ui;
}

void DlgSelectLotNumber::setLot(QString lot)
{
    ui->lineEdit->setText(lot);
}

QString DlgSelectLotNumber::getLot()
{
    return ui->lineEdit->text();
}
