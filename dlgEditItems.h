#ifndef DLGEDITITEMS_H
#define DLGEDITITEMS_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QtXml>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>
#include <QHeaderView>
#include <QCloseEvent>

namespace Ui {
class DlgEditItems;
}

class DlgEditItems : public QDialog
{
    Q_OBJECT

public:
    explicit DlgEditItems(QWidget *parent = 0);
    ~DlgEditItems();

private slots:
    void on_btnDeleteItem_clicked();
    void on_btnAddItem_clicked();
    void on_listItems_itemClicked(QListWidgetItem *item);
    void on_btnClear_clicked();

    void prepareTable();
    void on_btnSave_clicked();

    void closeEvent(QCloseEvent *event);

    void on_listItems_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::DlgEditItems *ui;

    QDomDocument xmlDoc;
    QDomNodeList itmList;
    QDomElement root;

    bool unsavedChanges;

    void deleteItem(QString itemNode);
    bool addItem();
    bool checkParams();
};

#endif // DLGEDITITEMS_H
