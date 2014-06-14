#ifndef DLGSELECTLOTNUMBER_H
#define DLGSELECTLOTNUMBER_H

#include <QDialog>

namespace Ui {
class DlgSelectLotNumber;
}

class DlgSelectLotNumber : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelectLotNumber(QWidget *parent = 0);
    ~DlgSelectLotNumber();

    void setLot(QString lot);
    QString getLot();
private:
    Ui::DlgSelectLotNumber *ui;

};

#endif // DLGSELECTLOTNUMBER_H
