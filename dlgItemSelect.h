#ifndef DLGITEMSELECT_H
#define DLGITEMSELECT_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QtXml>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHash>


namespace Ui {
class DlgItemSelect;
}

class DlgItemSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgItemSelect(QWidget *parent = 0);
    ~DlgItemSelect();
    QString htmlParams;
    QHash<QString,QString> hashParams;

private slots:
    void on_listWidgetItems_itemClicked(QListWidgetItem *item);
    void on_listWidgetItems_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::DlgItemSelect *ui;

    QDomDocument xmlDoc;
    QDomNodeList itmList;
    QDomElement root;
};

#endif // DLGITEMSELECT_H
