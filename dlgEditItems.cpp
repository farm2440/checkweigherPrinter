#include "dlgEditItems.h"
#include "ui_dlgEditItems.h"

DlgEditItems::DlgEditItems(QWidget *parent) :  QDialog(parent),  ui(new Ui::DlgEditItems)
{
    ui->setupUi(this);
    QHeaderView *hv = new QHeaderView(Qt::Horizontal, ui->tableWidget);
    ui->tableWidget->setHorizontalHeader(hv);
    hv->setSectionResizeMode(2,QHeaderView::Stretch);
    prepareTable();
//1. Отваря се файла с артикулите. Ако не съществува се създава нов
    QFile xmlFile("items.xml");
    if(!xmlFile.exists())
    {
        xmlFile.open(QIODevice::ReadWrite);
        QTextStream out(&xmlFile);
        out << "<itemsList></itemsList>";
    }else xmlFile.open(QIODevice::ReadWrite);
//2. Съдържанието се извлича
    xmlDoc.setContent(xmlFile.readAll());
    root = xmlDoc.documentElement();
    qDebug() << "xml parsing - root=" << root.nodeName();

    itmList = xmlDoc.elementsByTagName("item");
    for(int i=0 ; i!= itmList.count() ; i++)
    {
        ui->listItems->addItem(itmList.at(i).toElement().attribute("name"));
    }
    if(itmList.count())
    {
        on_listItems_currentItemChanged(ui->listItems->currentItem(),NULL);
    }

    xmlFile.close();
    unsavedChanges=false;
}

DlgEditItems::~DlgEditItems()
{
    delete ui;
}

void DlgEditItems::on_btnDeleteItem_clicked()
{
    if(ui->listItems->currentItem()==NULL) return;
    QString name = ui->listItems->currentItem()->text();
    deleteItem(name);
    //Артикулът е изтрит. Списъка с артикули се обновява и таблицата се изчиства
    ui->listItems->setCurrentRow(-1);
    ui->listItems->clear();
    itmList = xmlDoc.elementsByTagName("item");
    for(int j=0 ; j!= itmList.count() ; j++)
    {
        ui->listItems->addItem(itmList.at(j).toElement().attribute("name"));
    }

    ui->tableWidget->clear();
    prepareTable();
    unsavedChanges=true;
}

void DlgEditItems::on_btnAddItem_clicked()
{
    //Добавя се нов празен артикул и фокуса се премества върху него
    QDomElement newItem = xmlDoc.createElement("item");
    newItem.setAttribute("name","Нов артикул");
    newItem.setAttribute("code","няма зададен");
    root.appendChild(newItem);

    //Обновява се списъка и таблицата
    ui->listItems->setCurrentRow(-1);
    ui->listItems->clear();
    itmList = xmlDoc.elementsByTagName("item");
    for(int i=0 ; i!= itmList.count() ; i++)
    {
        ui->listItems->addItem(itmList.at(i).toElement().attribute("name"));
    }
    prepareTable();

    ui->listItems->setCurrentRow(ui->listItems->count()-1);
}

void DlgEditItems::on_listItems_itemClicked(QListWidgetItem *item)
{
    return;
    //При кликване върху артикул параметрите му се извличат от xml-а и се попълват в таблица
    QString name = item->text();
    QString param,tag,value,code;
    QDomNodeList paramList,detailList;

    prepareTable();

    for(int i=0 ; i!= itmList.count() ; i++) //itmList е запълнен в конструктора от файла items.xml
    {
       if(name == itmList.at(i).toElement().attribute("name"))
       {
           //Открива се избрания артикул и в първия ред на таблицата се дава името му
           code = itmList.at(i).toElement().attribute("code");
           ui->tableWidget->setItem(0,2, new QTableWidgetItem(name));
           ui->tableWidget->setItem(1,2, new QTableWidgetItem(code));
           //В следващите редове се извеждат фиксираните параметри
           paramList = itmList.at(i).childNodes();
           for(int j=0; j!=paramList.count() ; j++)
           {
               QDomElement eParam = paramList.at(j).toElement();
               if(eParam.nodeName()=="param")
               {
                   detailList = eParam.childNodes();
                   param=""; tag=""; value="";
                   for(int k=0 ; k!=detailList.count() ; k++)
                   {
                       if(detailList.at(k).toElement().nodeName()=="paramName")
                           param = detailList.at(k).toElement().text();
                       if(detailList.at(k).toElement().nodeName()=="paramTag")
                           tag = detailList.at(k).toElement().text();
                       if(detailList.at(k).toElement().nodeName()=="paramValue")
                           value = detailList.at(k).toElement().text();
                   }
                   ui->tableWidget->setItem(j+2,0, new QTableWidgetItem(param));
                   ui->tableWidget->setItem(j+2,1, new QTableWidgetItem(tag));
                   ui->tableWidget->setItem(j+2,2, new QTableWidgetItem(value));
               }
           }
           return;
       }
    }
}

void DlgEditItems::on_btnClear_clicked()
{
    prepareTable();
}

void DlgEditItems::prepareTable()
{
    ui->tableWidget->clear();
    ui->tableWidget->setItem(0,0, new QTableWidgetItem("Артикул"));
    ui->tableWidget->setItem(0,1, new QTableWidgetItem("ITM"));
    ui->tableWidget->setItem(0,2, new QTableWidgetItem(""));
    ui->tableWidget->setItem(1,0, new QTableWidgetItem("Код на стока"));
    ui->tableWidget->setItem(1,1, new QTableWidgetItem("CDE"));
    ui->tableWidget->setItem(1,2, new QTableWidgetItem(""));
    for(int i=2 ; i!=ui->tableWidget->rowCount() ; i++)
    {
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(""));
    }
    //Не може да се редактира "Артикул", таг за артикул, "Код на стока" и таг за код на стока
    ui->tableWidget->item(0,0)->setFlags(0);
    ui->tableWidget->item(0,1)->setFlags(0);
    ui->tableWidget->item(1,0)->setFlags(0);
    ui->tableWidget->item(1,1)->setFlags(0);
    ui->tableWidget->item(0,0)->setBackgroundColor(Qt::yellow);
    ui->tableWidget->item(0,1)->setBackgroundColor(Qt::yellow);
    ui->tableWidget->item(1,0)->setBackgroundColor(Qt::yellow);
    ui->tableWidget->item(1,1)->setBackgroundColor(Qt::yellow);
}

void DlgEditItems::on_btnSave_clicked()
{
    //Ако е имало промени по текущия артикул и той не е сменян те ще бъдат записани в xmlDoc
    if(ui->listItems->currentItem()!=NULL)
    {
        if((ui->tableWidget->item(0,2)->text()!="") && (ui->tableWidget->item(1,2)->text()!=""))
        {
            deleteItem(ui->listItems->currentItem()->text());
            if(!addItem()) return;
        }
    }

    QFile xmlFile("items.xml");
    xmlFile.open(QIODevice::ReadWrite);
    xmlFile.resize(0);
    QTextStream stream;
    stream.setDevice(&xmlFile);
    xmlDoc.save(stream,0);
    xmlFile.close();

    unsavedChanges=false;
    this->accept();
}

void DlgEditItems::closeEvent(QCloseEvent *event)
{
    if(!unsavedChanges)  event->accept();
    else
    {
        int res = QMessageBox::warning(this,"ПРЕДУПРЕЖДЕНИЕ","Направените промени няма да бъдат записани. Сигурни ли сте?",QMessageBox::Yes,QMessageBox::No);
        if(res==QMessageBox::Yes) event->accept();
        else event->ignore();
    }
}

void DlgEditItems::deleteItem(QString itemNode)
{
    //Премахва артикул от xmlDoc
    for(int i=0 ; i!=itmList.count() ; i++)
    {
        if(itmList.at(i).toElement().attribute("name")==itemNode)
        {
            QDomNode node = itmList.at(i);
            root.removeChild(node);
            return;
        }
    }
}

bool DlgEditItems::addItem()
{
    QString tag,value;
    //1. Проверява се дали са попълнени полетата коректно
    if(!checkParams()) return false;

    QString name = ui->tableWidget->item(0,2)->text();
    QString code = ui->tableWidget->item(1,2)->text();

    //Добавяне на новия артикул в xml-a. Параметрите се взимат от таблицата
    QDomElement newParam,paramName,paramTag,paramValue;
    QDomText nameTxt, tagTxt, valueTxt;
    QDomElement newItem = xmlDoc.createElement("item");
    newItem.setAttribute("name",name);
    newItem.setAttribute("code",code);

    for(int i=2 ; i!=ui->tableWidget->rowCount() ; i++)
    {
        name  = ui->tableWidget->item(i,0)->text();
        tag   = ui->tableWidget->item(i,1)->text();
        value = ui->tableWidget->item(i,2)->text();

        if((tag!="") && (value!=""))
        {
            newParam = xmlDoc.createElement("param");

            paramName = xmlDoc.createElement("paramName");
            nameTxt = xmlDoc.createTextNode(name);
            paramTag = xmlDoc.createElement("paramTag");
            tagTxt = xmlDoc.createTextNode(tag);
            paramValue = xmlDoc.createElement("paramValue");
            valueTxt = xmlDoc.createTextNode(value);

            paramName.appendChild(nameTxt);
            paramTag.appendChild(tagTxt);
            paramValue.appendChild(valueTxt);

            newParam.appendChild(paramName);
            newParam.appendChild(paramTag);
            newParam.appendChild(paramValue);

            newItem.appendChild(newParam);
        }
    }
    root.appendChild(newItem);
    return true;
}

bool DlgEditItems::checkParams()
{
    //Проверява данните в таблицата. Ако са коректни връща true.
    //Прочита данните от таблицата, проверяваги и ако са коректни добавя в xmlDoc нов нод за артикул и връща true
    //Ако има некоректни данни, вади предупреждение и връща false
    QString tag;
    //1. Проверява се дали са попълнени полетата коректно
    QString name = ui->tableWidget->item(0,2)->text();
    QString code = ui->tableWidget->item(1,2)->text();
    if(name=="")
    {
        QMessageBox::critical(this,"ГРЕШКА", "Не е зададено име на артикул!");
        return false;
    }
    if(code=="")
    {
        QMessageBox::critical(this,"ГРЕШКА", "Не е зададен код на стока!");
        return false;
    }
    for(int i=2 ; i!=ui->tableWidget->rowCount() ; i++)
    {
        tag = ui->tableWidget->item(i,1)->text();
        if((tag=="ITM") || (tag=="CDE") || (tag=="WGH") || (tag=="DTA") || (tag=="TME"))
        {
            QMessageBox::critical(this,"ГРЕШКА", QString("Некоректна стойност в колона 2 ред %1.\nITM,CDE,DTA,TME и WGH са запазени.").arg(i+1));
            return false;
        }
    }
    //2.Проверява се за дублирано име на артикул и код на стока
    for(int i=0 ; i!=itmList.count() ; i++)
    {
        if(name==itmList.at(i).toElement().attribute("name"))
        {
            QMessageBox::critical(this,"ГРЕШКА", "Дублирано име на артикул!");
            return false;
        }
        if(code==itmList.at(i).toElement().attribute("code"))
        {
            QMessageBox::critical(this,"ГРЕШКА", "Дублиран код на стока!");
            return false;
        }
    }
    //Проверката е приключена
    return true;
}

void DlgEditItems::on_listItems_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{//Сменя се текущо избрания артикул. В таблицата са параметрите на предишния. За да не се загубят направени
    //промени артикула се изтрива в xmlDoc и се записва наново.

    if(current==NULL) return;

    if(previous!=NULL)
    {
        //Ако е имало промени те ще бъдат записани
        deleteItem(previous->text());
        addItem();
    }

    prepareTable();

    //Попълване в таблицата на параметрите на новия артикул
    QString name = current->text();
    QString param,tag,value,code;
    QDomNodeList paramList,detailList;

    for(int i=0 ; i!= itmList.count() ; i++) //itmList е запълнен в конструктора от файла items.xml
    {
       if(name == itmList.at(i).toElement().attribute("name"))
       {
           //Открива се избрания артикул и в първия ред на таблицата се дава името му
           code = itmList.at(i).toElement().attribute("code");
           ui->tableWidget->setItem(0,2, new QTableWidgetItem(name));
           ui->tableWidget->setItem(1,2, new QTableWidgetItem(code));
           //В следващите редове се извеждат фиксираните параметри
           paramList = itmList.at(i).childNodes();
           for(int j=0; j!=paramList.count() ; j++)
           {
               QDomElement eParam = paramList.at(j).toElement();
               if(eParam.nodeName()=="param")
               {
                   detailList = eParam.childNodes();
                   param=""; tag=""; value="";
                   for(int k=0 ; k!=detailList.count() ; k++)
                   {
                       if(detailList.at(k).toElement().nodeName()=="paramName")
                           param = detailList.at(k).toElement().text();
                       if(detailList.at(k).toElement().nodeName()=="paramTag")
                           tag = detailList.at(k).toElement().text();
                       if(detailList.at(k).toElement().nodeName()=="paramValue")
                           value = detailList.at(k).toElement().text();
                   }
                   ui->tableWidget->setItem(j+2,0, new QTableWidgetItem(param));
                   ui->tableWidget->setItem(j+2,1, new QTableWidgetItem(tag));
                   ui->tableWidget->setItem(j+2,2, new QTableWidgetItem(value));
               }
           }
           return;
       }
    }
}

void DlgEditItems::on_tableWidget_cellChanged(int row, int column)
{
    QRegExp rx("[<>\"!]"); //Забранени символи за XML
    QString txt = ui->tableWidget->item(row,column)->text().trimmed();
    if(txt.contains(rx))
    {
        qDebug() << "Forbidden chars in XML";
        QMessageBox::warning(this,"ПРЕДУПРЕЖДЕНИЕ","Символите <,>,! и \" са забранени!");
        ui->tableWidget->item(row,column)->setText(txt.remove(rx));
    }
    else ui->tableWidget->item(row,column)->setText(txt);
    unsavedChanges = true;

}
