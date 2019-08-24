#include "utils.h"
#include <QXmlStreamReader>
#include <QApplication>
#include <QMessageBox>
#include <QTextStream>
#include <QProcess>
#include <QProgressDialog>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextCursor>
#include <QDebug>

Utils::Utils()
{
}
QString Utils::getBookPath(const QString & name,const QString &pathCostm)
{

    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
    appDir.cd(".");
    QString pathApp=  appDir.absolutePath()+"/books/";
#else
    appDir.cdUp();
    QString pathApp=  appDir.absolutePath()+"/share/elkirtasse/books/";
#endif	
qDebug()<<pathApp;
    if(name.startsWith("pdfs")&&name.toLower().endsWith("pdf")){

        if(QFile::exists(name)){
            return name;
        }else if(QFile::exists( pathCostm + "/" + name )){
            return pathCostm + "/" + name;
        }else if(QFile::exists( pathApp + name )){
            return pathApp + name;
        }else{
            return QString();
        }

    }
    //التأكد من وجود الكتاب في المسار
       QString idparent=name.section("_",0,0);
    idparent=idparent.remove("bk");

    if( QFile::exists( pathCostm + "/" + name +"/book.xml")){

        return pathCostm + "/" + name;
    }else if( QFile::exists( pathCostm + "/" + idparent+"/" + name +"/book.xml")){

        return pathCostm+ "/" + idparent+"/" + name;

    }else if(QFile::exists( pathApp + name +"/book.xml")){

            return pathCostm + name;

    }else{

        return QString();

    }
     return QString();
}
QDir Utils::getPluginDir()
{
    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
	appDir.cd(".");
	QString m_pathApp=  appDir.absolutePath()+"/";
	QDir pluginsDir(m_pathApp);
#else
	appDir.cdUp();
	QString m_pathApp=  appDir.absolutePath()+"/share/elkirtasse";
    QDir pluginsDir(m_pathApp);
#endif
#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
   return pluginsDir;
#endif
    if (!QFile::exists(m_pathApp+"/plugins")){
        if(QFile::exists("/usr/lib/elkirtasse")){
            pluginsDir.setPath("/usr/lib/elkirtasse");
        }else if(QFile::exists("/usr/lib64/elkirtasse")){
            pluginsDir.setPath("/usr/lib64/elkirtasse");
        }
    }
    return pluginsDir;
}

/***********************************************************************************
 *                             عمليات التحميل
 **********************************************************************************/
//! تجميل المفضلة
void Utils::favoriteCharge(QTreeWidget *view, const QIcon &icong, const QIcon &iconf)
{
    //    QString path=QCoreApplication::applicationDirPath ();
#ifdef Q_OS_HAIKU
    QString path=QDir::homePath()+"/config/settings/elkirtasse";
#else
	QString path=QDir::homePath()+"/.kirtasse";
#endif

    QDomDocument doc;
    QString myxmlfavorite="<?xml version='1.0' encoding='UTF-8'?>"
                          "<dataroot>"
                          " <groupe name='عام' />"
                          "</dataroot>";
    QFile file(path +"/data/favorite.xml");
    file.open(QIODevice::ReadOnly);
    if (file.exists() ){
        if (!doc.setContent(&file))
            doc.setContent(myxmlfavorite);
    }
    file.close();
    QTreeWidgetItem *item;
    QTreeWidgetItem *osloItem ;
    view->clear();
    QDomElement racine = doc.documentElement(); //
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull())
    {
        QDomNodeList tab;
        QDomElement mesures;
        mesures=noeud.toElement();
        QString text1=mesures.attribute("Department");
        // QMessageBox::information(this, "conection", id1 + text1);
        item= new QTreeWidgetItem(view);
        item->setText(0,text1 );
        item->setIcon(0,icong);
        tab = mesures.childNodes();
        for(int i=0;i<(int)tab.length();i++)
        {
            QDomNode noeud2=tab.item(i);
            QDomElement mesures2=noeud2.toElement();
            QString id= mesures2.attribute("id");
            QString title= mesures2.attribute("title");
            QString bkname= mesures2.attribute("bkname");
            QString bktitle= mesures2.attribute("bktitle");
            QString autname= mesures2.attribute("autname");
            osloItem= new QTreeWidgetItem(item);
            osloItem->setText(0,title );
            osloItem->setData(2,1,id);
            osloItem->setText(1,bktitle );
            osloItem->setData(1,1,bkname);
            osloItem->setText(2,autname );
            osloItem->setIcon(0,iconf);
        }
        noeud = noeud.nextSibling();
    }
    doc.clear();
}
//! تحميل الاجزاء
void Utils::treeChargeJozaa(QTreeWidget *view)
{
    QTreeWidgetItem *item=new QTreeWidgetItem(view) ;
    QTreeWidgetItem *osloItem=new QTreeWidgetItem(item) ;
    QTreeWidgetItem *osloItem2 ;
    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
	appDir.cd(".");
	//mazbrili change
	//QString m_pathApp=  appDir.absolutePath()+"/data";
	//to
	QString pathApp=  appDir.absolutePath()+"/data";
	QFile file(pathApp +"/ajzaa.xml");
#else
    appDir.cdUp();
    QString pathApp=  appDir.absolutePath()+"/share/elkirtasse";
    QFile file(pathApp +"/data/ajzaa.xml");
#endif
    file.open(QIODevice::ReadOnly);
    view->clear();
    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {

        xml.readNext();
        if (xml.name() == "root"){
            QString idjoza= xml.attributes().value("id").toString();
            QString textjoza=xml.attributes().value("Name").toString();
            if(!textjoza.isNull()){
                item= new QTreeWidgetItem(view);
                item->setText(0,textjoza );
                item->setData(1,1,idjoza);
            }
        }else if (xml.name() == "Item"){
            QString idhizb= xml.attributes().value("id").toString();
            QString texthizb= xml.attributes().value("Name").toString();
            if(!texthizb.isNull()){
                osloItem= new QTreeWidgetItem(item);
                osloItem->setText(0,texthizb );
                osloItem->setData(1,1,idhizb);
            }
        }else if (xml.name() == "bk"){
            QString idhrobaa= xml.attributes().value("id").toString();
            QString textrobaa= xml.attributes().value("name").toString();
            if(!textrobaa.isNull()){
                osloItem2= new QTreeWidgetItem(osloItem);
                osloItem2->setText(0,textrobaa );
                osloItem2->setData(1,1,idhrobaa);
            }
        }

    }
    xml.clear();
    file.close();
}
//! تحميل السور
void Utils::treeChargeSoura(QTreeWidget *view)
{

    QTreeWidgetItem *itemsora=new QTreeWidgetItem(view);
    QTreeWidgetItem *itemaya ;
    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
	appDir.cd(".");
	//change by mazbrili
	//QString m_pathApp=  appDir.absolutePath()+"/data";
	//to
	QString pathApp=  appDir.absolutePath()+"/data";
	QFile file(pathApp +"/curan.xml");
#else
    appDir.cdUp();
    QString pathApp=  appDir.absolutePath()+"/share/elkirtasse";
    QFile file(pathApp +"/data/curan.xml");
#endif
    file.open(QIODevice::ReadOnly);

    view->clear();
    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.name() == "Department"){
            QString idSora= xml.attributes().value("id").toString();
            QString textSora=xml.attributes().value("Name").toString();
            if(!textSora.isNull()){
                itemsora= new QTreeWidgetItem(view);
                itemsora->setText(0,textSora );
                itemsora->setData(1,1,idSora);
            }
        }
        if (xml.name() == "Item"){
            QString idAya= xml.attributes().value("ID").toString();
            QString textAya=xml.readElementText();
            if(!textAya.isNull()){
                itemaya= new QTreeWidgetItem(itemsora);
                itemaya->setText(0,textAya );
                itemaya->setData(1,1,idAya);
            }
        }
    }
    xml.clear();
    file.close();
}
//! تحميل قائمة الكتب
void Utils::treeChargeGroupe(QTreeWidget *view,int checked,bool asCombobox,QComboBox *comboGroup)
{
#ifdef Q_OS_HAIKU
    QString path=QDir::homePath()+"/config/settings/elkirtasse";
#else
    QString path=QDir::homePath()+"/.kirtasse";
#endif
    QTreeWidgetItem *itemRoot= new QTreeWidgetItem(view);
    QTreeWidgetItem *itemGroup= new QTreeWidgetItem(itemRoot);
    QTreeWidgetItem *itemBook;
    QFile file(path+"/data/group.xml");
    file.open(QIODevice::ReadOnly);

    view->clear();
    if (asCombobox==false){
        if(comboGroup)
        comboGroup->clear();
    }
    QIcon iconPdf;
     iconPdf=    QIcon::fromTheme("application-pdf",QIcon(":/images/image/application-pdf"));

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/image/booki.png")), QIcon::Normal, QIcon::Off);
    QPixmap pixmap = icon.pixmap(QSize(22, 22),QIcon::Disabled,QIcon::Off);
    QIcon icon1;
    icon1.addPixmap(pixmap);
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/image/groupb.png")), QIcon::Normal, QIcon::Off);
    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        QString drbox;
        if (xml.name() == "root"){ //niveau 1
            QString textRoot=xml.attributes().value("Name").toString();
            if(!textRoot.isNull()){
                itemRoot= new QTreeWidgetItem(view);
                itemRoot->setText(0,textRoot );
                itemRoot->setIcon(0,icon2);

            }
        }else if (xml.name() == "Item"){ //niveau 2
            QString idGroup= xml.attributes().value("id").toString();
            QString textGroup= xml.attributes().value("Name").toString();
            if(!textGroup.isNull()){
                itemGroup= new QTreeWidgetItem( itemRoot);
                itemGroup->setText(0,textGroup );
                itemGroup->setIcon(0,icon2);
                itemGroup->setData(2,1,idGroup);
                if (checked==1){
                    itemGroup->setCheckState(0,Qt::Unchecked);
                }

            }

        }else if (xml.name() == "bk"){ //niveau 3

            QString idBook= xml.attributes().value("id").toString();
            QString textBook=xml.attributes().value("name").toString();
            QString AutBook= xml.attributes().value("aut").toString();
            QString BetakaBook= xml.attributes().value("betaka").toString();
            QString curanBook= xml.attributes().value("tfsr").toString();

            if(!textBook.isNull()){
                itemBook= new QTreeWidgetItem( itemGroup);
                itemBook->setText(0,textBook);
                itemBook->setText(1,AutBook);
                itemBook->setText(2,BetakaBook);
                itemBook->setData(1,1,idBook);
                if(idBook.endsWith("pdf"))
                     itemBook->setIcon(0,iconPdf);
                else
                itemBook->setIcon(0,icon);

                itemBook->setData(2,1,curanBook);

                if (checked==1){
                    itemBook->setCheckState(0,Qt::Unchecked);
                }
                if (asCombobox==false){

                    if (curanBook=="1")
                    {
                      if(comboGroup)
                        comboGroup->addItem(textBook,idBook+"/"+AutBook);
                    }
                }
            }
        }
    }
    if (xml.hasError()) {

        QMessageBox::information(0,"",xml.errorString());
      }

    xml.clear();
    file.close();
}
//! تحميل الفهارس
void Utils::treeChargeFahrass(QTreeWidget *view,const QString &Bname)
{

    QString titlepath= Bname +"/title.xml";
   // QString titlepath= Bname;
    QFile file(titlepath);
    file.open(QIODevice::ReadOnly);
    view->clear();
    int d=1;
 //   enum  { NumIndex = 10};
    QTreeWidgetItem *item;

    QString tit;
    QString id;
    QString lvl;

    item= new QTreeWidgetItem(view);
    item->setText(0,"بسم الله الرحمن الرحيم" );
    item->setData(1,1,1);

    QXmlStreamReader xml;
    xml.setDevice(&file);
    if (xml.readNextStartElement()) {
        if (xml.name() == "dataroot" ){
            Q_ASSERT(xml.isStartElement() && xml.name() == "dataroot");
            while (xml.readNextStartElement()) {
                if (xml.name() == "title"){

                    //   Q_ASSERT(xml.isStartElement() && xml.name() == "book");
                    while (xml.readNextStartElement()) {
                        if (xml.name() == "id"){
                            id=xml.readElementText();


                        }else if (xml.name() == "tit"){

                            tit= xml.readElementText();
                        }else if (xml.name() == "lvl"){

                            lvl= xml.readElementText().trimmed();
                        }else{
                            xml.skipCurrentElement();
                        }


                    }
                    bool ok;
                    d=lvl.toInt(&ok,0);

                 if (ok==true){
                        if (d<1){d=1;}
                        if (d==1){

                            item= new QTreeWidgetItem(view);
                            item->setText(0,tit );
                            item->setText(1,id );
                            item->setData(1,1,id);



                        }else{
                            int index=view->topLevelItemCount()-1;
                            QTreeWidgetItem *itemParent=(view->topLevelItem(index));

                            for(int i=0;i<d-2;i++){
                                if(itemParent)
                                  itemParent= getItem(itemParent) ;
                            }

                           item= new QTreeWidgetItem(itemParent);
                           item->setText(0,tit );
                           item->setText(1,id );
                           item->setData(1,1,id);

                        }
                    }else{
                        item= new QTreeWidgetItem(view);
                        item->setText(0,tit );
                        item->setText(1,id );
                        item->setData(1,1,id);
                    }
                }
            }
        }


    }
    file.close();
    //listId.clear();
    xml.clear();
}
QTreeWidgetItem* Utils::getItem(QTreeWidgetItem *item)
{
    int index=item->childCount()-1;
    return item->child(index);
}
//!
void Utils::comboCharge(QComboBox *combo)
{
    QDomDocument docCombo;
#ifdef Q_OS_HAIKU
    QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
    QString m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/group.xml");

    file.open(QIODevice::ReadOnly);
    if (!docCombo.setContent(&file)){return;}
    QDomElement racine = docCombo.documentElement(); //renvoie la balise racine
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull())
    {
        QDomNodeList tab;
        QDomNodeList tab2;
        QDomElement groupPer;
        groupPer=noeud.toElement();
        if (groupPer.tagName()== "root")
        {
            tab = groupPer.childNodes();
            for(int i=0;i<(int)tab.length();++i)
            {
                QDomNode noeud2=tab.item(i);
                QDomElement mygroup=noeud2.toElement();
                QString mygroupname= mygroup.attribute("Name");
                QString mygroupid= mygroup.attribute("id");
                combo->addItem(mygroupname,mygroupid);
            }
        }
        noeud = noeud.nextSibling();
    }
    docCombo.clear();
    file.close();
}

/***********************************************************************************
 *                             عمليات الحفظ
 **********************************************************************************/
//!
void Utils::favoriteSave(QTreeWidget *view)
{
#ifdef Q_OS_HAIKU
    QString pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
     QString  pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QString myxmlfavorite="<?xml version='1.0' encoding='UTF-8'?>"
                          "<dataroot>"
                          "</dataroot>";
      QDomDocument m_doc;
    m_doc.setContent(myxmlfavorite);
    QTreeWidgetItem *item;
    QTreeWidgetItem *itemols;
    QString grouptext;
    QDomElement racine = m_doc.documentElement();
    for(int i=0;i<view->topLevelItemCount();++i)
    {
        item= view->topLevelItem(i);
        grouptext=item->text(0);
        QDomElement groupe=m_doc.createElement("groupe");     //انشاء عنصر جديد
        racine.appendChild(groupe);                    //اظافته في اخر القائمة
        groupe.setAttribute("Department",grouptext);      //اسم الكتاب
        for(int r=0;r<item->childCount();++r)
        {
            itemols=item->child(r);
            QString id= itemols->data(2,1).toString();
            QString title=itemols->text(0);
            QString bkname=  itemols->data(1,1).toString();
            QString bktitle =itemols->text(1);
            QString autname=itemols->text(2);
            QDomElement items=m_doc.createElement("item");
            groupe.appendChild(items);
            items.setAttribute("id",id);
            items.setAttribute("title",title);
            items.setAttribute("bkname",bkname);
            items.setAttribute("bktitle",bktitle);
            items.setAttribute("autname",autname);
        }
    }
    QFile file(pathUser +"/data/favorite.xml");
    file.open(QIODevice::WriteOnly);              //فتح الملف للكتابة
    QTextStream out(&file);                       //الكتابة
    m_doc.save(out,1);                      //حفظ الملف
    file.close();
    m_doc.clear();
}
//!
bool Utils::treeSaveGroupe(QTreeWidget *view)
{
#ifdef Q_OS_HAIKU
    QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
	QString  m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QString myxmlgroup="<?xml version='1.0' encoding='UTF-8'?>"
                       "<setting>"
                       "</setting>";
   QDomDocument m_doc;
    m_doc.setContent(myxmlgroup);
    QTreeWidgetItem *item;
    QTreeWidgetItem *itemols;
    QTreeWidgetItem *itemols2;
    QString grouptext;
    QDomElement racine = m_doc.documentElement();
    for(int i=0;i<view->topLevelItemCount();++i)
    {
        item= view->topLevelItem(i);
        grouptext=item->text(0);
        QDomElement groupe=m_doc.createElement("root");     //انشاء عنصر جديد
        racine.appendChild(groupe);                    //اظافته في اخر القائمة
        groupe.setAttribute("Name",grouptext);      //اسم الكتاب
        groupe.setAttribute("id",i);
        for(int r=0;r<item->childCount();++r)
        {
            itemols=item->child(r);
            QString id= itemols->data(2,1).toString();
            QString title=itemols->text(0);
            QDomElement items=m_doc.createElement("Item");
            groupe.appendChild(items);
            items.setAttribute("id",id);
            items.setAttribute("Name",title);
            for(int r=0;r<itemols->childCount();++r)
            {
                itemols2=itemols->child(r);
                QString id= itemols2->data(1,1).toString();
                QString betaka=itemols2->text(2);
                QString aut  =itemols2->text(1);
                QString name =itemols2->text(0);
                QDomElement itemsbk=m_doc.createElement("bk");
                items.appendChild(itemsbk);
                itemsbk.setAttribute("id",id);
                itemsbk.setAttribute("betaka",betaka);
                itemsbk.setAttribute("aut",aut);
                itemsbk.setAttribute("name",name);
                if (itemols2->data(2,1)=="1")
                {
                    itemsbk.setAttribute("tfsr","1");
                }
            }
        }
    }
    QFile file(m_pathUser +"/data/group.xml");
    file.open(QIODevice::WriteOnly);              //فتح الملف للكتابة
    QTextStream out(&file);                       //الكتابة
    m_doc.save(out,1);                      //حفظ الملف
    file.close();
    m_doc.clear();
    return true;
}
//!
bool Utils::fahrasSave(QTreeWidget *view, const QString &bkname, const QString &pathCostum)
{
    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
	appDir.cd(".");
	//mazbrili change
	//QString m_pathApp=  appDir.absolutePath()+"/data";
	//into
	QString pathApp=  appDir.absolutePath()+"/data";
#else
    appDir.cdUp();
    QString pathApp=  appDir.absolutePath()+"/share/elkirtasse";
#endif
    QFile file;
    QString path;
    if(file.exists(pathCostum+"/" +bkname+"/title.xml")){
        path=pathCostum+"/" +bkname+"/title.xml";
    }else if (file.exists(pathApp+"/books/" +bkname+"/title.xml")){
        path=pathApp+"/books/" +bkname+"/title.xml";
    }else{
        return false;
    }
    file.setFileName(path);
    if(!file.open(QIODevice::WriteOnly) )  {            //فتح الملف للكتابة عليها
        return false;
    }
    QString myxmltitle="<?xml version='1.0' encoding='UTF-8'?>"
                       "<dataroot>"
                       "</dataroot>";
  QDomDocument m_doc;
    m_doc.setContent(myxmltitle);
    QTreeWidgetItem *itemr;
    QTreeWidgetItem *item=new QTreeWidgetItem;
    QList<QTreeWidgetItem *> found =
            view->findItems(
                    "",Qt::MatchContains | Qt::MatchRecursive,0);//تنشاء قائمة بكل العناصر
    if (found.count()>0) {
        int lvl=1;
        foreach (item, found){
            itemr= item;
            if (!item->parent()){//اذا كان العنصر جذرا
                writeInDoc(item->text(0),item->data(1,1).toString(),"1",m_doc);
                lvl=1;
            }else{//اذا كان له اباء
                while(itemr->parent()){//معرفة عدد الاباء
                    itemr=itemr->parent();
                    lvl=lvl+1;
                }
                QVariant lvlstr=lvl;
                writeInDoc(item->text(0),item->data(1,1).toString(),lvlstr.toString(),m_doc);
                lvl=1;
            }

        }
    }


    QTextStream out(&file);                       //الكتابة
    const int IndentSize = 1;
    m_doc.save(out,IndentSize);                      //حفظ الملف
    file.close();
    m_doc.clear();

    return true;
}

void Utils::writeInDoc(QString tit,QString data,QString lvl,QDomDocument doc)
{
    if(data.isEmpty()||data.isNull())data="1";
    QDomElement racine = doc.documentElement(); //renvoie la balise racine
    QDomNode noeud = racine.firstChild();
    QDomElement myel=doc.createElement("title");     //انشاء عنصر جديد
    racine.appendChild(myel);
    QDomElement myid=doc.createElement("id");     //انشاء عنصر جديد
    myel.appendChild(myid);
    QDomText t1 = doc.createTextNode(data);
    myid.appendChild(t1);
    QDomElement mynass=doc.createElement("tit");     //انشاء عنصر جديد
    myel.appendChild(mynass);
    QDomText t2 = doc.createTextNode(tit);
    mynass.appendChild(t2);
    QDomElement mylvl=doc.createElement("lvl");     //انشاء عنصر جديد
    myel.appendChild(mylvl);
    QDomText t3 = doc.createTextNode(lvl);
    mylvl.appendChild(t3);

}

/***********************************************************************************
 *                             عمليات متعددة
 **********************************************************************************/
//! حذف الكتاب المحدد
bool Utils::treeMenuRemoveBook(const QString &BKname,bool removall,const QString &pathCostum)
{
#ifdef Q_OS_HAIKU
    QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
    QString m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/group.xml");
    file.open(QIODevice::ReadOnly);
    QDomDocument m_doc;
    if (!m_doc.setContent(&file)){return false;}
    file.close();
    QDomElement racine = m_doc.documentElement(); //renvoie la balise racine
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull())
    {
        QDomNodeList tab;
        QDomNodeList tab2;
        QDomElement Root;
        Root=noeud.toElement();                   // تحديد اول عنصر من ابناء العنصر الاصلي
        if (Root.tagName()== "root")
        {
            tab = Root.childNodes();
            for (int i=0;i< (int)tab.length();++i)
            {
                QDomNode noeud2=tab.item(i);
                QDomElement Group=noeud2.toElement();
                tab2 = Group.childNodes();
                for(int r=0;r<(int)tab2.length();r++)
                {
                    QDomNode noeud3=tab2.item(r);
                    QDomElement Book=noeud3.toElement();
                    QString idBook =Book.attribute("id");
                    if (BKname==idBook){
                        Group.removeChild(Book);
                        file.open(QIODevice::WriteOnly);              //فتح الملف للكتابة عليها
                        QTextStream out(&file);                       //الكتابة
                        const int IndentSize = 1;
                        m_doc.save(out,IndentSize);                      //حفظ الملف
                        file.close();
                        m_doc.clear();                                   //مسح المحتويات من الذاكرة
                        if (removall==true){                     //الحذف من الجهاز
                            QString    pathbook=pathCostum;
                            QDir dir(pathCostum+"/" +BKname);
                            if (!dir.exists()){
                                return false;
                            }
                            dir.setFilter(QDir::Files | QDir::Hidden );
                            QFileInfoList list = dir.entryInfoList();
                            for (int i = 0; i < list.size(); ++i) {
                                QFileInfo fileInfo = list.at(i);
                                if(dir.remove(fileInfo.fileName())==false){
                                    return false;
                                }
                            }
                            QDir dirm(pathbook );
                            if(dirm.rmdir(BKname)==false){
                                return false;
                            }
                        }
                        return true;
                    }
                }
            }
        }
        noeud = noeud.nextSibling();
    }
    m_doc.clear();
    file.close();
    return true;
}
//التحقق كم زجزد الكتاب
bool Utils::checkBookExist(QString BKname)
{
#ifdef Q_OS_HAIKU
    QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
	QString m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/group.xml");
    file.open(QIODevice::ReadOnly);
    QDomDocument m_doc;
    if (!m_doc.setContent(&file)){return false;}
    file.close();
    QDomElement racine = m_doc.documentElement(); //renvoie la balise racine
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull())
    {
        QDomNodeList tab;
        QDomNodeList tab2;
        QDomElement Root;
        Root=noeud.toElement();                   // تحديد اول عنصر من ابناء العنصر الاصلي
        if (Root.tagName()== "root")
        {
            tab = Root.childNodes();
            for (int i=0;i< (int)tab.length();++i)
            {
                QDomNode noeud2=tab.item(i);
                QDomElement Group=noeud2.toElement();
                tab2 = Group.childNodes();
                for(int r=0;r<(int)tab2.length();r++)
                {
                    QDomNode noeud3=tab2.item(r);
                    QDomElement Book=noeud3.toElement();
                    QString idBook =Book.attribute("id");
                     // qDebug()<<idBook<<"==="<<BKname;
                    if (BKname==idBook){
                        m_doc.clear();
                        file.close();
                        return true;
                    }
                }
            }
        }
        noeud = noeud.nextSibling();
    }
    m_doc.clear();
    file.close();
    return false;
}
//!
QTreeWidgetItem *Utils::getItemByBook(QTreeWidget *view,const QString &dirName)
{
     QTreeWidgetItem *itemnul;
    for (int i = 0; i < view->topLevelItemCount(); ++i) {
        QTreeWidgetItem *itemTop=view->topLevelItem(i);
        for (int r = 0; r <itemTop->childCount(); ++r) {
            QTreeWidgetItem *itemP=itemTop->child(r);
            for (int c = 0; c < itemP->childCount(); ++c) {
                 QTreeWidgetItem *item=itemP->child(c);
                 if(item->data(1,1).toString()==dirName)
                     return item;
            }
        }
    }
    return itemnul;

}
//!
QString Utils::getUrlInBookList(const QString &dirName)
{
#ifdef Q_OS_HAIKU
    QString path=QDir::homePath()+"/config/settings/elkirtasse";
#else
    QString path=QDir::homePath()+"/.kirtasse";
#endif

    QFile file(path+"/data/bookslist.xml");
    if(!file.exists()){

    QMessageBox::information(0,"bookslist","file noexist :"+path+"/data/bookslist.xml");
    return "";
    }

//    qDebug()<<"file exist--------"<<path+"/data/bookslist.xml";
    file.open(QIODevice::ReadOnly);


     QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.name() == "bk"){ //niveau 3

            QString idBook= xml.attributes().value("id").toString();

//            QString textBook=xml.attributes().value("name").toString();
//            QString AutBook= xml.attributes().value("aut").toString();
// qDebug()<<idBook;
            if(!idBook.isNull()){
                QString name=idBook;
                name=name.split("/").last().remove(".tar.gz");
            // qDebug()<<dirName<<"name-----------------"<<name;
                if(name==dirName)
                return idBook;


            }
        }
    }


    xml.clear();
    file.close();
    return "";
}
//!
void Utils::treeUpdateGroupe(QTreeWidget *view,bool remove,const QString &pathCostum)
{
#ifdef Q_OS_HAIKU
    QString path=QDir::homePath()+"/config/settings/elkirtasse";
#else
    QString path=QDir::homePath()+"/.kirtasse";
#endif
    QTreeWidgetItem *itemRoot= new QTreeWidgetItem(view);
    QTreeWidgetItem *itemGroup= new QTreeWidgetItem(itemRoot);
    QTreeWidgetItem *itemBook;
    QFile file(path+"/data/group.xml");
    file.open(QIODevice::ReadOnly);

    view->clear();
    QIcon iconPdf;
     iconPdf=    QIcon::fromTheme("application-pdf",QIcon(":/images/image/application-pdf"));


    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/image/booki.png")), QIcon::Normal, QIcon::Off);
    QPixmap pixmap = icon.pixmap(QSize(22, 22),QIcon::Disabled,QIcon::Off);
    QIcon icon1;
    icon1.addPixmap(pixmap);
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/image/groupb.png")), QIcon::Normal, QIcon::Off);
    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.name() == "root"){ //niveau 1
            QString textRoot=xml.attributes().value("Name").toString();
            if(!textRoot.isNull()){
                itemRoot= new QTreeWidgetItem(view);
                itemRoot->setText(0,textRoot );
                itemRoot->setIcon(0,icon2);

            }
        }else if (xml.name() == "Item"){ //niveau 2
            QString idGroup= xml.attributes().value("id").toString();
            QString textGroup= xml.attributes().value("Name").toString();
            if(!textGroup.isNull()){
                itemGroup= new QTreeWidgetItem( itemRoot);
                itemGroup->setText(0,textGroup );
                itemGroup->setIcon(0,icon2);
                itemGroup->setData(2,1,idGroup);

            }

        }else if (xml.name() == "bk"){ //niveau 3

            QString idBook= xml.attributes().value("id").toString();
            QString textBook=xml.attributes().value("name").toString();
            QString AutBook= xml.attributes().value("aut").toString();
            QString BetakaBook= xml.attributes().value("betaka").toString();
            QString curanBook= xml.attributes().value("tfsr").toString();
            QString path=getBookPath(idBook,pathCostum);
            bool exist=QFile::exists(path);

//            QFile file(path);
//            if (file.exists()){
//                exist=true;
//            }
            if(!textBook.isNull()){
                if(remove==false){
                    itemBook= new QTreeWidgetItem( itemGroup);
                    itemBook->setText(0,textBook);
                    itemBook->setText(1,AutBook);
                    itemBook->setText(2,BetakaBook);
                    itemBook->setData(1,1,idBook);
                    itemBook->setData(2,1,curanBook);
                    if(exist==true){
                        itemBook->setIcon(0,icon);
                    }else{
                        itemBook->setIcon(0,icon1);
                    }
                }else{
                    if(exist==true){
                        itemBook= new QTreeWidgetItem( itemGroup);
                        itemBook->setText(0,textBook);
                        itemBook->setText(1,AutBook);
                        itemBook->setText(2,BetakaBook);
                        itemBook->setData(1,1,idBook);
                        itemBook->setData(2,1,curanBook);

                        if(idBook.endsWith("pdf"))
                             itemBook->setIcon(0,iconPdf);
                        else
                        itemBook->setIcon(0,icon);

                    }
                }

            }
        }
    }


    xml.clear();
    file.close();
}
//!
bool Utils::addNewBook(const QString &bkpath,const QString &bktitle,
                       const QString &bkauth,const QString &bkbetaka,
                       const QString &groupid ,bool cheked)
{
#ifdef Q_OS_HAIKU
    QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
	QString m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/group.xml");
    if (bkpath.isEmpty()){return false;}
    //  if (!filepath.exists()){return false;}
//    if (bktitle.isEmpty()){bktitle=("بدون اسم");}
//    if (bkauth.isEmpty()){ bkauth=("غير معروف");}
    file.open(QIODevice::ReadOnly);
     QDomDocument m_doc;
    if (!m_doc.setContent(&file)){return false;}
    file.close();
    QDomElement racine = m_doc.documentElement();                  //عنصر الشجرة الاصلي
    QDomNode noeud = racine.firstChild();                       // تحديد اول عنصر من ابناء العنصر الاصلي
    while(!noeud.isNull())                                       //كلما كان العنصر غير فارغا
    {
        QDomNodeList tab;
        QDomNodeList tab2;
        QDomElement groupPer;
        groupPer=noeud.toElement();
        if (groupPer.tagName()== "root")                         //اذا كان اسم العنصر "root"
        {
            tab = groupPer.childNodes();                          //قائمة الابناء
            for(int i=0;i<(int)tab.length();++i)                        //عدد الابناء
            {
                QDomNode noeud2=tab.item(i);
                QDomElement mygroup=noeud2.toElement();
                if  (mygroup.attribute("id")==groupid)     //التوقف عند العنصر الذي اخترناه من المجموعة مثلا "التفاسير
                {
                    QDomElement myel=m_doc.createElement("bk");     //انشاء عنصر جديد
                    mygroup.appendChild(myel);                    //اظافته في اخر القائمة
                    myel.setAttribute("id",bkpath);        //اسم مجلد الكتاب
                    myel.setAttribute("name",bktitle);      //اسم الكتاب
                    myel.setAttribute("aut",bkauth);      //اسم المؤلف
                    myel.setAttribute("betaka", bkbetaka);      //بطاقة عن الكتاب
                    if (cheked==true) {
                        myel.setAttribute("tfsr", "1");
                    }
                    file.open(QIODevice::WriteOnly);              //فتح الملف للكتابة عليها
                    QTextStream out(&file);                       //الكتابة
                    const int IndentSize = 1;
                    m_doc.save(out,IndentSize);                      //حفظ الملف
                    file.close();
                    m_doc.clear();                                   //مسح المحتويات من الذاكرة
                    return true;                                         //انهاء العملية
                }
            }
        }
        noeud = noeud.nextSibling();
    }
    m_doc.clear();
    file.close();
    return true;
}
//!
bool Utils::treeviewItemRemove(QTreeWidget *view)
{
    QTreeWidgetItem *item= view->currentItem();
    if (!item){
        return false;
    }
    int t;
    if (item->parent()){
        t= item->parent()->indexOfChild(item);
        item->parent()->takeChild(t);
    } else{
        t=view->indexOfTopLevelItem(item);
        view->takeTopLevelItem(t);
    }
    return true;
}
//!
bool Utils::treeviewItemUp(QTreeWidget *view)
{
    int t;
    QTreeWidgetItem *new_item;           //العنصر الجديد
    QTreeWidgetItem *curent_item=view->currentItem();      //العنصر المحدد
    QTreeWidgetItem *old_item = view->itemAbove(curent_item);   //العنصر السابق
    if (!curent_item || !old_item){ //اذا لم يوجد عنصرا محددا او السابق موجودا يخرج
        return false;;
    }
    new_item=curent_item->clone(); //نسخ العنصر المحدد مع ابنائه ان كان يحمل ابناء
    if (!curent_item->parent()){  //اذا كان في قمة الشجرة اي ليس له ابناء
        t=view->indexOfTopLevelItem(curent_item);//معرفة ترقيم العنصر
        view->takeTopLevelItem(t);//حذف العنصر المحدد
        view->insertTopLevelItem(t-1,new_item); //اظافة العنصر المنسوخ في الذاكرة بعد العنصر التالي
    }    else{     //اذا كان العنصر ابنا
        QTreeWidgetItem *parent=curent_item->parent();
        t= parent->indexOfChild(curent_item);
        if(t==0){    //اذا كان العنصر اول عنصر ابن
            return false;;
        }
        parent->takeChild(t);
        parent->insertChild(t-1,new_item);
    }
    view->setCurrentItem(new_item); //
    return true;
}
//!
bool Utils::treeviewItemDown(QTreeWidget *view)
{
    int t;
    int r;
    QTreeWidgetItem *new_item;
    QTreeWidgetItem *curent_item=view->currentItem();//العنصر المحدد
    QTreeWidgetItem *old_item =view->itemBelow(curent_item);//العنصر التالي
    if (!curent_item || !old_item){  return false; } //اذا لم يوجد عنصرا محددا او التالي يخرج
    new_item=curent_item->clone(); //نسخ العنصر المحدد مع ابنائه ان كان يحمل ابناء
    if (!curent_item->parent()){  //اذا كان في قمة الشجرة اي ليس له ابناء
        t=view->indexOfTopLevelItem(curent_item);//معرفة ترقيم العنصر
        r=view->topLevelItemCount()-1;
        if(t==r){ return false; }
        view->takeTopLevelItem(t);//حذف العنصر المحدد
        view->insertTopLevelItem(t+1,new_item); //اظافة العنصر المنسوخ في الذاكرة بعد العنصر التالي
    } else{//اذا كان العنصر ابنا
        QTreeWidgetItem *parent=curent_item->parent();

        t= parent->indexOfChild(curent_item);
        r=parent->childCount()-1;
        if(t==r){return false;  }//اذا كان العنصر اخر عنصر ابن
        parent->takeChild(t);

        parent->insertChild(t+1,new_item);

    }
    view->setCurrentItem(new_item); //تحديد العنصر الجديد
    return true;
}

//!
int Utils::genirateId(QTreeWidget *view)
{
    int newid=0;
    QList<QString> list;
    QTreeWidgetItem *itemtop;
    QTreeWidgetItem *itemgroup;
    for(int i=0;i<view->topLevelItemCount();++i){
        itemtop=view->topLevelItem(i);
        for(int r=0;r<itemtop->childCount();++r)
        {
            itemgroup=itemtop->child(r);
            QVariant data=itemgroup->data(2,1);
            list.append(data.toString());
        }
    }
    bool exist=false;

    while(exist==false)
    {
        newid=newid+1;
        QVariant dd=newid;
        if  ( !list.contains(dd.toString())){
            exist=true;
        }
    }
    list.clear();
    return newid;
}
//!
QString Utils::geniratNewBookName(QString groupParent)
{

    QList<QString> list;
#ifdef Q_OS_HAIKU
    QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
	QString m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/group.xml");
    file.open(QIODevice::ReadOnly);
     QDomDocument m_doc;
    if (!m_doc.setContent(&file)){return "bk"+groupParent;}
    file.close();
    QDomElement racine = m_doc.documentElement(); //renvoie la balise racine
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull())
    {
        QDomNodeList tab;
        QDomNodeList tab2;
        QDomElement Root;
        Root=noeud.toElement();                   // تحديد اول عنصر من ابناء العنصر الاصلي
        if (Root.tagName()== "root")
        {
            tab = Root.childNodes();
            for(int i=0;i<(int)tab.length();++i)
            {
                QDomNode noeud2=tab.item(i);
                QDomElement Group=noeud2.toElement();
                tab2 = Group.childNodes();
                for(int r=0;r<(int)tab2.length();r++)
                {
                    QDomNode noeud3=tab2.item(r);
                    QDomElement Book=noeud3.toElement();
                    QString idBook =Book.attribute("id");
                    list.append(idBook);
                }
            }
        }
        noeud = noeud.nextSibling();
    }
    m_doc.clear();
    file.close();
    int newid=0;
    bool exist=false;
    QString newBookName;
    while(exist==false)
    {
        newid=newid+1;

        QVariant dd=newid;
        newBookName="bk"+groupParent+"_"+dd.toString();
        if  ( !list.contains(newBookName)){
            exist=true;
        }
    }
    list.clear();
    return newBookName;
}
//!
bool Utils::saveBookInfo(const QString &bookname,const QString &title,
                         const QString &author,const QString &betaka,
                         const QString &pathCostum)
{
    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
	appDir.cd(".");
#else
    appDir.cdUp();
#endif
   // QString pathApp=  appDir.absolutePath()+"/share/elkirtasse";
    QFile file;
    QString path;
//    if(file.exists(pathCostum+"/" +bookname+"/bookinfo.info")){
//        path=pathCostum+"/" +bookname+"/bookinfo.info";
//    }else if (file.exists(pathApp+"/books/" +bookname+"/bookinfo.info")){
//        path=pathApp+"/books/" +bookname+"/bookinfo.info";
//    }else{
//        path=pathCostum+"/" +bookname+"/bookinfo.info";
//    }

 path=getBookPath(bookname,pathCostum);
 file.setFileName(path+"/bookinfo.info");
    if(! file.open(QIODevice::WriteOnly)) {
        return false;
    }
    //فتح الملف للكتابة

    QString myxmltitle="<?xml version='1.0' encoding='UTF-8'?>"
                       "<dataroot>"
                       "</dataroot>";
     QDomDocument m_doc;
    m_doc.setContent(myxmltitle);
    // QString grouptext;
    QDomElement racine = m_doc.documentElement();
    QDomElement groupe=m_doc.createElement("groupe");     //انشاء عنصر جديد
    racine.appendChild(groupe);                       //الكتابة
    //اظافته في اخر القائمة
    groupe.setAttribute("title",title);
    groupe.setAttribute("author",author);
    groupe.setAttribute("betaka",betaka);

    QTextStream out(&file);                  //حفظ الملف
    m_doc.save(out,1);
    file.close();
    m_doc.clear();
    return true;
}
bool Utils::chekBookInfo(QString bookname)
{

    QFile file(bookname+"/bookinfo.info");

    if (!file.open(QIODevice::ReadOnly)){
        return false;
    }
     QDomDocument m_doc;
    if (!m_doc.setContent(&file)){
        return false;
    }
    file.close();

    m_doc.clear();
    return true;

}
QHash<QString, QString> Utils::loadBookInfo(QString bookname)
{
QHash<QString, QString> hash;
hash["title"] = "";
hash["author"] = "";
hash["betaka"] = "";
    QFile file(bookname+"/bookinfo.info");

    if (!file.open(QIODevice::ReadOnly)){
        return hash;
    }
     QDomDocument m_doc;
    if (!m_doc.setContent(&file)){
        return hash;
    }
    file.close();
    QDomElement racine = m_doc.documentElement(); //
    QDomNode noeud = racine.firstChild();
    QDomElement myper=noeud.toElement();

    //   mesures=noeud.toElement();
    hash["title"] = myper.attribute("title");
    hash["author"]= myper.attribute("author");
    hash["betaka"]= myper.attribute("betaka");

    m_doc.clear();
    return hash;

}

//!
bool Utils::loadTarGz(QString path)
{
    //verifier
    QProcess prosses;
#ifdef Q_OS_HAIKU
    QString pathToExtract=QDir::homePath()+"/config/settings/elkirtasse/download";
#else
    QString pathToExtract=QDir::homePath()+"/.kirtasse/download";
#endif
    QProgressDialog progress(QObject::trUtf8("الرجاء الانتظار...."), QObject::trUtf8("الغاء"), 0, 0, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.setEnabled(false);
    progress.show();
    qApp->processEvents();
#ifdef   Q_OS_WIN

    //removeTempDirs(pathToExtract);

    prosses.setWorkingDirectory(QApplication::applicationDirPath());
    QString extar="7z e \""+path + "\" -o\""+pathToExtract+"\" *.tar -y ";
    prosses.start(extar);
    prosses.waitForFinished();
    //info

    prosses.start("7z l \""+pathToExtract+"\"");
    while(prosses.state()==QProcess::Running){
        qApp->processEvents();
        progress.setValue(progress.value()+1);
    }

    //extract
    QByteArray result = prosses.readAll();
    //  QMessageBox::critical(this,trUtf8("خطأ"),result);
    if (!result.contains("book.xml")&&!result.contains("bookinfo.info")){
        QMessageBox::critical(0,QObject::trUtf8("خطأ"),QObject::trUtf8("الارشيف لا يحتوي على كتاب\n")+path);
        return false;
    }
    prosses.start("7z e \""+pathToExtract+"\"/*.tar -o\""+pathToExtract+"\" -y");
    while(prosses.state()==QProcess::Running){
        qApp->processEvents();
        progress.setValue(progress.value()+1);
    }
    //  archiveDir="";
    return true;


#else
    prosses.start("tar -tf  \""+path+"\"");
    prosses.waitForFinished();
    QByteArray result = prosses.readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString string = codec->toUnicode(result);
    if ( !string.contains("book.xml") && !string.contains("bookinfo.info")){
        QMessageBox::critical(0,QObject::trUtf8("خطأ"),QObject::trUtf8("الارشيف لا يحتوي على كتاب\n")+path);
        return false;
    }
    QString archiveDir=string.section( "/",0,0);
    // QMessageBox::critical(0,trUtf8("خطأ"),archiveDir);


    qDebug()<<"archiveDir"<<archiveDir;
    qDebug()<<"pathToExtract"<<pathToExtract;

    prosses.execute("mkdir -p "+pathToExtract);
    prosses.waitForFinished();
    prosses.start(QString("tar -xvf  %1 -C  %2").arg("\""+path+"\"").arg("\""+pathToExtract+"\""));
    QString debu=QString("tar -xvf  %1 -C  %2").arg("\""+path+"\"").arg("\""+pathToExtract+"\"");
    qDebug()<<debu;
    while(prosses.state()==QProcess::Running){
        qApp->processEvents();
        progress.setValue(progress.value()+1);
    }
    prosses.waitForFinished();
    prosses.start(QString("mv   %1   %2").arg("\""+pathToExtract+"/"+archiveDir+"/book.xml\"").arg("\""+pathToExtract+"\""));
    prosses.waitForFinished();
    prosses.start(QString("mv   %1   %2").arg("\""+pathToExtract+"/"+archiveDir+"/bookinfo.info\"").arg("\""+pathToExtract+"\""));
    prosses.waitForFinished();
    prosses.start(QString("mv   %1   %2").arg("\""+pathToExtract+"/"+archiveDir+"/title.xml\"").arg("\""+pathToExtract+"\""));
    prosses.waitForFinished();
    return true;
#endif

}

//!
void Utils::removeTempDirs(QString dirName)
{
    removeTempFiles(dirName);
    QString subdir;

    QDir dir(dirName);
    foreach ( subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot |
                                    QDir::Hidden )){

        if ( removeTempFiles(dirName+subdir)==true){
            // QMessageBox::information(0,trUtf8("خطأ"),dirName+"/"+subdir);
            removeTempDirs(dirName+"/"+subdir);
            dir.rmdir(subdir);

        }

    }
}


bool  Utils::removeTempFiles(QString tempDir)
{

        QDir dirS(tempDir);
        QString subfile;
        QFile filetemp;
        foreach ( subfile, dirS.entryList(QDir::AllEntries)){

            filetemp.remove(tempDir+"/"+subfile);
        }

return true;


}
void Utils::findFahariss(QTreeWidget *view,QString bookpath,QRegExp rx,QRegExp rx2,int rowcount)
{
    QFile file(bookpath+"/book.xml");

    QTreeWidgetItem *item ;
    QTreeWidgetItem *osloItem;

    int conter=0;
    QTextDocument *document=new QTextDocument;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {

        return ;
    }
 QXmlStreamReader xml;
    xml.setDevice(&file);

    view->clear();
    item=new QTreeWidgetItem(view) ;
    item->setText(0,QObject::trUtf8("بسم الله الرحمن الريم"));
    QProgressDialog progress(QObject::trUtf8("الرجاء الانتظار...."), QObject::trUtf8("الغاء"), 0, rowcount, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    qApp->processEvents();
    if (xml.readNextStartElement()) {
        if (xml.name() == "dataroot" || xml.name() == "root"){

            Q_ASSERT(xml.isStartElement() && xml.name() == "dataroot" || xml.isStartElement() && xml.name() == "root");

            while ( xml.readNextStartElement()) {
                if (xml.name() == "book"){
                    conter++;
                    progress.setValue(conter);
                    while (xml.readNextStartElement()) {
                        if (xml.name() == "nass"){
                            //  nass=xml.readElementText();
                            document->setPlainText(xml.readElementText());

                        }else{

                            xml.skipCurrentElement();
                        }
                    }
                    //*************************

                    QApplication::processEvents();
                    if (progress.wasCanceled())
                        break;
                    QTextCursor highlightCursor(document);
                    QTextCursor cursor(document);
                    cursor.beginEditBlock();
                    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
                        highlightCursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);

                        QString text=highlightCursor.selectedText().trimmed();
                        if(text.length()>=150)
                            text= text.mid(0,150);
                        int pos = rx.indexIn(text);
                        int pos2=rx2.indexIn(text);
                        if (pos<=3&&pos!=-1){
                            item=new QTreeWidgetItem(view) ;
                            item->setText(0,text.trimmed());
                            item->setData(1,1,conter);
                            item->setText(1,QString::number(conter) );
                        }
                        if (!rx2.isEmpty())
                            if (pos2<=3&&pos2!=-1){
                            osloItem=new QTreeWidgetItem(item) ;
                            osloItem->setText(0,text.trimmed());
                            osloItem->setData(1,1,conter);
                            osloItem->setText(1,QString::number(conter) );
                        }

                        highlightCursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);

                    }
                    cursor.endEditBlock();

                    //*********************************
                } else{

                    xml.skipCurrentElement();
                }


            }
            progress.cancel();
        }

    }

    xml.clear();
    file.close();
}
