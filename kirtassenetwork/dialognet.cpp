/****************************************************************************
//   elkirtasse Copyright (C) 2010 yahia abouzakaria <yahiaui@gmail.com>
//
//      This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
//      This is free software, and you are welcome to redistribute it
//      under certain conditions; type `show c' for details.
//
//  The hypothetical commands `show w' and `show c' should show the appropriate
//  parts of the General Public License.  Of course, your program's commands
//  might be different; for a GUI interface, you would use an "about box".
//
//    You should also get your employer (if you work as a programmer) or school,
//  if any, to sign a "copyright disclaimer" for the program, if necessary.
//  For more information on this, and how to apply and follow the GNU GPL, see
//  <http://www.gnu.org/licenses/>.
//
//    The GNU General Public License does not permit incorporating your program
//  into proprietary programs.  If your program is a subroutine library, you
//  may consider it more useful to permit linking proprietary applications with
//  the library.  If this is what you want to do, use the GNU Lesser General
//  Public License instead of this License.  But first, please read
//  <http://www.gnu.org/philosophy/why-not-lgpl.html>.
// ----------------------------------------------------------
** If you have questions regarding the use of this file, please contact
** abouzakaria (yahiaui@gmail.com)
** $elkirtasse_END_LICENSE$
**
****************************************************************************/
#include "dialognet.h"
#include "ui_dialognet.h"
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QtGui>
#include <QDomDocument>
class QSslError;
class QHttp;
class QHttpResponseHeader;

Dialognet::Dialognet( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialognet)
{
    init();
    //m_urlPath=urlPath;
    loadSettings();
    treeChargeGroupe(ui->treeWidget,0);
    ui->stackedWidget->setCurrentIndex(0);

}

Dialognet::Dialognet( const QString &urlfile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialognet)
{
    init();
    ui->stackedWidget->setCurrentIndex(1);
    m_urlFile=urlfile;

    process=new QProcess;
    downloadFile(urlfile,QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/download/");
}

void Dialognet::init()
{
    ui->setupUi(this);
     process=new QProcess;
       isDownlist=false;
}

void Dialognet::loadSettings()//load layou
{

    QString m_pathUser=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data";

    QSettings settings(m_pathUser+"/setting.ini",QSettings::IniFormat);

    //****************************

    settings.beginGroup("NetUrls");
      ui->comboBoxUrls->addItems(settings.value("listUrls",QStringList()<<
                              "http://sourceforge.net/projects/elkirtasse/files/allbooks/bookslist.xml"<<
                                              "http://dl.dropbox.com/u/13013844/update_liste/bookslist.xml").toStringList());


    settings.endGroup();


}

QStringList Dialognet::getListUrls()
{
    QStringList list;
    list.append(ui->comboBoxUrls->currentText());
    for (int i=0;i<ui->comboBoxUrls->count();i++){
        list.append(ui->comboBoxUrls->itemText(i));
    }
  list.removeDuplicates();
  return list;
}
void Dialognet::saveSettings()//حفظ البيانات الى ملف
{

    QString m_pathUser=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data";

    QSettings settings(m_pathUser+"/setting.ini",
						QSettings::IniFormat);
    // QSettings settings("Kirtasse", "setting");
    settings.beginGroup("NetUrls");

    settings.setValue("listUrls", getListUrls());

    settings.endGroup();
}


Dialognet::~Dialognet()
{
    delete ui;
}

void Dialognet::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialognet::treeChargeGroupe(QTreeWidget *view,int checked)
{

    QString path=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QFile file(path+"/data/bookslist.xml");
    if(!file.exists()){

    QMessageBox::information(0,"bookslist","file no exist :"+path+"/data/bookslist.xml");
       return;
    }

    QTreeWidgetItem *itemRoot= new QTreeWidgetItem(view);
    QTreeWidgetItem *itemGroup= new QTreeWidgetItem(itemRoot);
    QTreeWidgetItem *itemBook;
    file.open(QIODevice::ReadOnly);

    view->clear();


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
                if (checked==1){
                    itemGroup->setCheckState(0,Qt::Unchecked);
                }
            }

        }else if (xml.name() == "bk"){ //niveau 3

            QString idBook= xml.attributes().value("id").toString();
            QString textBook=xml.attributes().value("name").toString();
            QString AutBook= xml.attributes().value("aut").toString();
           // QString BetakaBook= xml.attributes().value("betaka").toString();
            //QString curanBook= xml.attributes().value("tfsr").toString();
            if(!textBook.isNull()){
                itemBook= new QTreeWidgetItem( itemGroup);
                itemBook->setText(0,textBook);
                itemBook->setText(1,AutBook);
               // itemBook->setText(2,BetakaBook);
                itemBook->setData(1,1,idBook);
                itemBook->setIcon(0,icon);
                //itemBook->setData(2,1,curanBook);
                if (checked==1){
                    itemBook->setCheckState(0,Qt::Unchecked);
                }

            }
        }
    }


    xml.clear();
    file.close();
}

void Dialognet::on_toolButtonGetList_clicked()
{
    isDownlist=true;

    if (QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/bookslist.xml-old"))
            QFile::remove(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/bookslist.xml-old");

    QFile::rename(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/bookslist.xml",
                        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"data/bookslist.xml-old");

    downloadFile(ui->comboBoxUrls->currentText(),QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/");



}
//-****************
//! [0]
  void Dialognet::readstdout()
  {
//qDebug()<<process->readAllStandardOutput();
 // ui->lineEditUrl->setText(process->readAllStandardOutput());


   QString  stdoutdata = process->readAllStandardOutput();

   ui->textEditCmd->insertPlainText("\n"+stdoutdata);
   int index= ui->textEditCmd->toPlainText().length();
   QTextCursor cursor(ui->textEditCmd->document());
   cursor.setPosition(index,QTextCursor::MoveAnchor);
   cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
   ui->textEditCmd->setTextCursor(cursor);
   qApp->processEvents();
    qDebug()<<stdoutdata;
  }
  void Dialognet::readsteror()
  {
      QString eror=process->readAllStandardError();

      m_error=true;
      QString   txt=  tr("Download error wait : %1.\n")
              .arg(eror);
      ui->textEditCmd->insertPlainText(txt);

      int index= ui->textEditCmd->toPlainText().length();
      QTextCursor cursor(ui->textEditCmd->document());
      cursor.setPosition(index,QTextCursor::MoveAnchor);
      cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
      ui->textEditCmd->setTextCursor(cursor);
      qApp->processEvents();
      qDebug()<<txt;
  }

void Dialognet::processHasFinished(int /*index*/)
{
 if  (isDownlist==true){

     if(QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/bookslist.xml"))
         QFile::rename(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/bookslist.xml-old",QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/data/bookslist.xml");
     isDownlist=false;
     ui->stackedWidget->setCurrentIndex(0);
     saveSettings();
      treeChargeGroupe(ui->treeWidget,0);
     return;
 }
    //qDebug()<<process->workingDirectory();

    QString fileName = QFileInfo(QUrl(m_urlFile).path()).fileName();
    tagzName=fileName;


    if(!QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/download/"+tagzName)){
              QMessageBox::information(0, tr("HTTP"),
                                       tr("Download failed: %1.")
                                       .arg(m_urlFile));

         //      this->reject();
          //    return;
}

    qDebug()<<"dialognet return--------------------"<<tagzName;
          this->accept();

}

bool Dialognet::downloadFile(const QString &urlfile, const QString &distPath)
{
    ui->stackedWidget->setCurrentIndex(1);
    m_error=false;
    QAbstractButton *button=ui->buttonBox->button(QDialogButtonBox::Ok);
    button->setEnabled(false);

    m_urlFile=urlfile;
    ui->lineEditUrl->setText(m_urlFile);

    qApp->processEvents();


    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(readstdout()));
    connect(process,SIGNAL(readyReadStandardError()),this,SLOT(readsteror()));
    connect(process,SIGNAL(finished(int)),this,SLOT(processHasFinished(int)));

    process->setWorkingDirectory(distPath);
#if defined(Q_OS_WIN)
    QString prog=QApplication::applicationDirPath()+"/wget/wget.exe";
      process->start("\""+prog+"\" -c "+m_urlFile);
#else
     process->start("wget -c "+m_urlFile);
#endif

    qApp->processEvents();
    //for tedting------------------------------------------------------------------
//    QString path="/media/ARCHIVE/documents/dinia-diverts/shamila/bk_1002.tar.gz";
//    QString pathToExtract="/home/yahia/download/tartar/";
//    process->start(QString("cp  %1   %2").arg("\""+path+"\"").arg("\""+distPath+"\""));
    //-----------------------------------------------------------------------------


    return true;
}
void Dialognet::cancelDownload()
{
   //  QMessageBox::information(0, tr("HTTP"),tr("Download canceled."));
  process->close();

}


void Dialognet::on_treeWidget_itemSelectionChanged()
{
    QAbstractButton *button=ui->buttonBox->button(QDialogButtonBox::Ok);


    QTreeWidgetItem *item=ui->treeWidget->currentItem();
    if (!item->data(1,1).isNull()){
        m_urlFile= item->data(1,1).toString();


        ui->lineEditUrl->setText(m_urlFile);
        button->setEnabled(true);
    }else{
        ui->lineEditUrl->setText("");
        m_urlFile=QString::null;
            button->setEnabled(false);
    }
}
////--------------------------
void Dialognet::searchInTreeview(QTreeWidget *view,QString searchString,int colum)
{
    for( int i = 0; i <view->topLevelItemCount(); i++ ) {
        view->topLevelItem(i)->setHidden(false );
        showAllItems(view->topLevelItem(i),view);
    }
    if (!searchString.isEmpty()){
        for( int i = 0; i <view->topLevelItemCount(); i++ ) {
            searchTreeForString(searchString,view->topLevelItem(i),true,i,view,colum) ;
        }
    }
}

bool Dialognet::searchTreeForString( const QString &searchString, QTreeWidgetItem* parent,bool itemtop,int  topindex,QTreeWidget *view,int colum)
{
    int hiddenCount = 0;
    int hiddenparent=0;
    if (parent->childCount()==0 && !parent->text(colum).contains( searchString, Qt::CaseInsensitive )) {
        parent->setHidden(true);
    }
    for( int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *child = parent->child(i);
        if( parent->isHidden() )
            hiddenCount++;
        else if(child->childCount()>0
                && !child->text(colum).contains( searchString, Qt::CaseInsensitive ) ) {//لا يحمل النص وله ابناء
            if( !searchTreeForString( searchString, child ,false,topindex,view,colum)) {
                child->setHidden(true);
                hiddenCount++;
            }
        } else if(child->childCount()>0// يحمل النص  له ابناء
                  && ( child->text(colum).contains( searchString, Qt::CaseInsensitive ) ) ){
            searchTreeForString( searchString, child ,false,topindex,view,colum);
        } else if(child->childCount()==0//لا يحمل النص وليس له ابناء
                  && ( !child->text(colum).contains( searchString, Qt::CaseInsensitive ) ) ) {
            child->setHidden(true);
            hiddenCount++;
        }
        if (itemtop==true){
            if(child->isHidden()){hiddenparent++;}
        }
    }
    QTreeWidgetItem *toplevel=view->topLevelItem(topindex);
    int topc=toplevel->childCount();
    if( hiddenparent == topc  && !toplevel->text(colum).contains( searchString, Qt::CaseInsensitive )){
        toplevel->setHidden(true);
    }
    if( hiddenCount == parent->childCount() )
        return false;
    return true;
}

bool Dialognet::showAllItems( QTreeWidgetItem* parent,QTreeWidget *view )
{
    for( int i = 0; i < parent->childCount(); i++ ) {
        view->setItemHidden(parent->child(i),false);
        showAllItems( parent->child( i) ,view);
    }
    return true;
}


void Dialognet::on_lineEdit_2_textChanged(QString searchString )
{

////    Findbook->searchInTreeview(ui->treeWidget,searchString,0);
searchInTreeview(ui->treeWidget,searchString,0);

}
//**********************



void Dialognet::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button)==QDialogButtonBox::Ok){

        QString h=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/download/";
        QDir dir;
        if (!dir.exists(h))
            dir.mkdir( h);


        if (m_urlFile.isEmpty())
            return;

        if (downloadFile(m_urlFile,h)==true){
            // this->accept();
        }

    }else if(ui->buttonBox->standardButton(button)==QDialogButtonBox::Cancel){
        process->close();
        this->reject();
    }

}
