#include "dlgItemSelect.h"
#include "ui_dlgitemselect.h"

DlgItemSelect::DlgItemSelect(QWidget *parent) :  QDialog(parent),   ui(new Ui::DlgItemSelect)
{
    ui->setupUi(this);
    ui->listWidgetItems->clear();
    htmlParams="";
    //1. Отваря се файла с артикулите. Ако не съществува се създава нов
        QFile xmlFile("items.xml");
        if(!xmlFile.exists())
        {
            QMessageBox::critical(this,"ГРЕШКА", "Файлът с информация за артикулите не може да бъде открит!");
            return;
        }
        xmlFile.open(QIODevice::ReadOnly);
    //2. Съдържанието се извлича
        xmlDoc.setContent(xmlFile.readAll());
        root = xmlDoc.documentElement();

        itmList = xmlDoc.elementsByTagName("item");
        for(int i=0 ; i!= itmList.count() ; i++)
        {
            ui->listWidgetItems->addItem(itmList.at(i).toElement().attribute("name"));
        }
        xmlFile.close();

}

DlgItemSelect::~DlgItemSelect()
{
    delete ui;
}

void DlgItemSelect::on_listWidgetItems_itemClicked(QListWidgetItem *item)
{
    if(item==NULL) return;
}

void DlgItemSelect::on_listWidgetItems_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current==NULL) return;
    QString name = current->text();
    QString param,value,code,tag;
    QDomNodeList paramList,detailList;

    for(int i=0 ; i!= itmList.count() ; i++) //itmList е запълнен в конструктора от файла items.xml
    {
       if(name == itmList.at(i).toElement().attribute("name"))
       {
           hashParams.clear();
           //Открива се избрания артикул и в първия ред на таблицата се дава името му
           code = itmList.at(i).toElement().attribute("code");
           htmlParams = QString("<P><B>Артикул:</B> %1 <P>").arg(name);
           htmlParams += QString("<P><B>Код на стока:</B> %1 <P>").arg(code);
           hashParams.insert("CDE",code);
           hashParams.insert("ITM",name);
           //В следващите редове се извеждат фиксираните параметри
           paramList = itmList.at(i).childNodes();
           for(int j=0; j!=paramList.count() ; j++)
           {
               QDomElement eParam = paramList.at(j).toElement();
               if(eParam.nodeName()=="param")
               {
                   detailList = eParam.childNodes();
                   param=""; value="";
                   for(int k=0 ; k!=detailList.count() ; k++)
                   {
                       if(detailList.at(k).toElement().nodeName()=="paramName")
                           param = detailList.at(k).toElement().text();
                       if(detailList.at(k).toElement().nodeName()=="paramTag")
                           tag = detailList.at(k).toElement().text();
                       if(detailList.at(k).toElement().nodeName()=="paramValue")
                           value = detailList.at(k).toElement().text();
                   }
                   htmlParams += QString("<P><B>%1:</B> %2 <P>").arg(param).arg(value);
                   if(tag!="") hashParams.insert(tag,value);
               }
           }
           ui->textBrowserItemParams->setHtml(htmlParams);
           return;
       }
    }

}
