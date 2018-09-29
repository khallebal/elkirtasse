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
#include "dialog.h"
#include "classepub.h"
#include "booksInterface.h"
#include "utils.h"
#include  "ui_dialog.h"
#include <QtXml>
#include <QDomDocument>
#include <QMessageBox>
#include<QtGui>

#ifdef Q_OS_HAIKU
	QString tempDirs=QDir::homePath()+"/config/settings/elkirtasse/download/";
	QString tempFile=QDir::homePath()+"/config/settings/elkirtasse/download/";
#else
	QString tempDirs=QDir::homePath()+"/.kirtasse/download/";
	QString tempFile=QDir::homePath()+"/.kirtasse/download/";
#endif

DialogAddBooks::DialogAddBooks(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogAddBooks)
{
    ui->setupUi(this);
    ui->toolButtonInfo->setIcon(QIcon::fromTheme("dialog-information",style()->standardPixmap(QStyle::SP_MessageBoxInformation)));
    ui->toolButton_fileNam->setIcon(QIcon::fromTheme("document-open-folder", style()->standardPixmap(QStyle::SP_DirOpenIcon)));
   ui->toolButton_zipFileNam->setIcon( QIcon::fromTheme("file", QIcon(":/images/image/file")));
    ui->progressBar->setVisible(false);

// m_path=QCoreApplication::applicationDirPath ();



 Utils::comboCharge(ui->comboBox_group);
Utils::removeTempDirs(QDir::homePath()+tempDirs);
}

DialogAddBooks::~DialogAddBooks()
{
Utils::removeTempDirs(QDir::homePath()+tempDirs);
}


void DialogAddBooks::on_comboBox_group_currentIndexChanged(int index)
{

    QVariant idx;
    idx=ui->comboBox_group->itemData(index);
    m_addGroupeId=idx.toString();
    m_addGroupeName= ui->comboBox_group->itemText(index);


    if (idx.toString()=="28")
    {
        ui->checkBox_curan->setVisible(true);
    }
    else{
        ui->checkBox_curan->setVisible(false);
        ui->checkBox_curan->setChecked(false);
    }
}


void DialogAddBooks::on_buttonBox_clicked(QAbstractButton* button)
{
    if(ui->buttonBox->standardButton(button)==QDialogButtonBox::Ok){
        int countItem=ui->listWidget->count();
        if (countItem<1){
            this->reject();
        return;
}
        ui->progressBar->setMaximum(countItem-1);
        ui->progressBar->setVisible(true);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        for(int i=0;i<countItem;i++){
            ui->progressBar->setValue(i);
            QString filname= ui->listWidget->item(i)->text();
            QFileInfo fi(filname);
            QString ext = fi.completeSuffix();
            if (ext.toLower()=="tar.gz"||ext.toLower()=="krts"){
            //   Utils::removeTempDirs(QDir::homePath()+"/.kirtasse/download/");
                if(Utils::loadTarGz(filname)==true){

                    //QString tempFile=QDir::homePath()+"/.kirtasse/download";
                    loadBookInfo(tempFile);
                    if (copyDir(tempFile,countItem)==true)
                        msgTitle=msgTitle+"\n"+Add_Book_Title+"\n"+m_newPathDir;
                    //  archiveDir="";
                }
            } else if(ext.toLower()==("epub")){
               Utils::removeTempDirs(QDir::homePath()+tempDirs);
              classepub  *Classepub=new classepub;
                  if  (Classepub->ebubUnzip(filname)==true) {
                      Add_Book_Title=Classepub->infoBookTitle;
                      Add_Autor_Name=Classepub->infoBookAutor;
                      Add_Betaka=Classepub->infoBookBetaka;
                    //QString tempFile=QDir::homePath()+"/.kirtasse/download/";
                      if (copyDir(tempFile,countItem)==true)
                          msgTitle=msgTitle+"\n"+Add_Book_Title+"\n"+m_newPathDir;
                 }
            }else if(ext.toLower()==("pdf")){
              if  (copyPdfFile(filname)){
                    msgTitle=msgTitle+"\n"+Add_Book_Title+"\n"+m_newPathDir;
              }
            }else{

                loadBookInfo(filname);
                if (copyDir(filname,countItem)==true)
                    msgTitle=msgTitle+"\n"+Add_Book_Title+"\n"+m_newPathDir;
            }
            qApp->processEvents();
            QApplication::restoreOverrideCursor();
        }  //for

        if(!msgTitle.isEmpty()){

            QMessageBox msgBox;
            msgBox.setText(trUtf8(" لقد تم انشاء الكتب التالية بنجاح داخل المجموعة :  ")+m_addGroupeName );
            msgBox.setInformativeText(trUtf8("انقر على اظهار التفاصيل لمعرفة قائمة الكتب "));
            msgBox.setDetailedText(msgTitle);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setLayoutDirection(Qt::RightToLeft);
            msgBox.setWindowTitle(trUtf8("تعليمات"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
       Utils:: removeTempDirs(QDir::homePath()+tempDirs);
            qApp->processEvents();

            this->accept();
        } //msg
    } //ok
 this->accept();
}


bool DialogAddBooks::copyDir(QString filname,int )
{
    if ( creat_dir()==false){
            QMessageBox::information(this,trUtf8("خطأ"), trUtf8("خطأ لم استطع انشاء المجلد"));

        return false;
    }
//m_newPathDir=m_path+"/"+archiveDir;
//QMessageBox::information(this,trUtf8("خطأ"), m_newPathDir);

    QFile file;      //نسخ الملفات الى وجهة المكتبة
    if(file.copy(filname+"/title.xml",m_newPathDir+"/title.xml")==false){
        QMessageBox::information(this,trUtf8("خطأ"), trUtf8("خطأ لم استطع انشاء الملف title.xml"));
   // return false;
    }
    if(file.copy(filname+"/book.xml",m_newPathDir+"/book.xml")==false){
        QMessageBox::information(this,trUtf8("خطأ"), trUtf8("خطأ لم استطع انشاء الملف book.xml"));
    return false;
    }

//    if(items==1){
//    Add_Book_Name=ui->lineEdit_booknam->text();
//    Add_Autor_Name =ui->lineEdit_autor->text()  ;
//    Add_Betaka=ui->textEdit_betaka->textCursor().document()->toPlainText();
//    }
    bool checked=false;

    if (ui->checkBox_curan->checkState()==Qt::Checked) {
        checked=true;
    }
    if (Utils::addNewBook(m_bookDir,Add_Book_Title,Add_Autor_Name,Add_Betaka,m_addGroupeId,checked)==false)
    {
        QMessageBox::information(this,trUtf8("خطأ"), trUtf8("ربما نسيت ملأ احد اخانات الضرورية أو ان بيانات الكتاب خاطئة"));
         return false;
    }else{

        Utils::saveBookInfo(m_bookDir,Add_Book_Title,Add_Autor_Name,Add_Betaka,m_pathCostm);


    }
     return true;
}

bool  DialogAddBooks::creat_dir()
 {
    QString newBooName=  Utils::geniratNewBookName(m_addGroupeId);
       QDir newdir= m_pathCostm+"/"+ newBooName;
       if (!newdir.exists()){
           QDir dir(m_pathCostm );
           dir.mkdir( newBooName);
         m_bookDir  =newBooName;
            m_newPathDir=m_pathCostm+"/"+ newBooName;
           return true;
          }

     bool exit=false;
     int i=0;
    while(exit==false)
     {
         i=i+1;
         QVariant m=i;
         QString path=m_pathCostm;
          QDir newdir =path+"/"+ newBooName+"_"+m.toString();
         if (newdir.exists())
         {
             exit=false;
         }
         else{
             QDir dir(path );
             if(dir.mkdir( newBooName+"_"+ m.toString())==false) {
                 return false;
             }
            m_bookDir  =newBooName+"_"+ m.toString();
             m_newPathDir=path+"/"+ newBooName+"_"+ m.toString();
             exit=true;
         }
     }
    return true;
 }
void DialogAddBooks::on_toolButton_fileNam_clicked()
{

    QString homeDir=QDir::homePath () ;
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QString(),
                                                    QFileDialog::ShowDirsOnly
                                                    |QFileDialog::DontResolveSymlinks);
    if(QFile::exists(dirName+"/book.xml")){
     ui->listWidget->addItem(dirName);
     return;
     }

   QString subdir;
    QDir dir(dirName);
    foreach ( subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot |
                                    QDir::Hidden )){
        if(QFile::exists(dirName+"/"+subdir+"/book.xml"))
        ui->listWidget->addItem(dirName+"/"+subdir);

    }

}
void DialogAddBooks::loadBookInfo(QString path)
{
    //    ui->lineEdit_fileNam->setText(path);
    if (  Utils::chekBookInfo(path)==false) {
     Utils::removeTempDirs(QDir::homePath()+tempDirs);
        QMessageBox::information(this,"info.xml","this info.xml no valid");
        return;
    }
    QHash<QString,QString>hash=Utils::loadBookInfo(path);
    Add_Book_Title=hash.value("title");
   Add_Autor_Name=hash.value("author");
   Add_Betaka=hash.value("betaka");

}
// --***************tar.gz
void DialogAddBooks::on_toolButton_zipFileNam_clicked()
{
 // removeTempDirs(QDir::homePath()+"/.kirtasse/download/");
   // QFileDialog dlg;

    QStringList fn = QFileDialog::getOpenFileNames(0, tr("Open  Files..."),
                                                 QString(), trUtf8("ملفات الكتب (*tar.gz *.krts *.epub *.pdf);;krts (*.krts);;tar.gz (*.tar.gz);;epub (*.epub) ;;pdf (*.pdf);;كل الملفات (*)"));
    if (!fn.isEmpty())
    {
      ui->listWidget->addItems(fn);
      //  loadTarGz(fn);

    }
}




void DialogAddBooks::on_toolButton_delete_clicked()
{
    if (ui->listWidget->count()==0){return;}
    QListWidgetItem *item=ui->listWidget->currentItem();
    if (item->isSelected()){

        int x=ui->listWidget->currentRow();
        ui->listWidget->takeItem(x);
    }
}

void DialogAddBooks::on_toolButtonInfo_clicked()
{
     if (ui->listWidget->count()==0){return;}
    QListWidgetItem *item=ui->listWidget->currentItem();
    if (!item->isSelected())
       return;

    QString path=ui->listWidget->currentItem()->text();

   if (path.contains(".tar.gz")||path.contains(".Krts")){

       #ifdef   Q_OS_WIN
       QMessageBox::information(this,trUtf8("معلومات الكتاب"),
                                trUtf8("الكتاب حزمة مضغوطة"));
        return ;
       #endif
       QProcess prosses;
       prosses.start("tar -tf  "+path);
       prosses.waitForFinished();
       QByteArray result = prosses.readAll();
       QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QString string = codec->toUnicode(result);
        if (!string.contains("book.xml") && !string.contains("bookinfo.info")){
           QMessageBox::critical(this,trUtf8("خطأ"),trUtf8("الارشيف لا يحتوي على كتاب\n"));
            return ;
        }else{
             QString titles=string.section( "/",0,0);
            QMessageBox::information(this,trUtf8("معلومات الكتاب"),titles+"\n"+
                                     trUtf8("الكتاب حزمة مضغوطة"));

        }

   }
    if (  Utils::chekBookInfo(path)==false) {

        return;
    }
    QHash<QString,QString>hash=Utils::loadBookInfo(path);

    QString title=hash.value("title");
    QString autor=hash.value("author");
    QString betaka=hash.value("title");
    QMessageBox::information(this,trUtf8("معلومات الكتاب"),title+"\n"+
                             autor+"\n"+
                             betaka);
}
bool DialogAddBooks::copyPdfFile(const QString &filname)
{
   //PdfWidget *documentWidget =new PdfWidget;
 //  if (documentWidget->setDocument(filname)) {
       QFileInfo fi(filname);
       QString newName=fi.fileName();
       newName.replace(" ","_");
       QDir dir;
       dir.mkpath(m_pathCostm+"/pdfs/");
       if(QFile::exists(m_pathCostm+"/pdfs/"+newName)){
           QMessageBox msgBox;
           msgBox.setIcon(QMessageBox::Question);
           msgBox.setText(trUtf8( "  الكتب  موجود في هذا الدليل \n ")+m_pathCostm+"/pdfs/"+newName);
           msgBox.setInformativeText(trUtf8( "هل تريد الكتابة عليه ؟  "));
           msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
           msgBox.setDefaultButton(QMessageBox::No);
           int ret = msgBox.exec();
           switch (ret) {

           case QMessageBox::Yes:
               QFile::remove(m_pathCostm+"/pdfs/"+newName);
               break;
           case QMessageBox::No:
               return false;
               break;
           default:
               // should never be reached
               break;
           }

       }
       if ( QFile::copy(filname,m_pathCostm+"/pdfs/"+newName)==false)return false;
       QString betaka="";
       QString title=fi.baseName();
       QString auth="";
       m_newPathDir=m_pathCostm+"/pdfs/"+newName;
       Add_Book_Title=title;
       Add_Autor_Name=auth;
       Add_Betaka=betaka;
 //  QPixmap pix((documentWidget->pixmap()->width()/100)*10,(documentWidget->pixmap()->height()/100)*10);
//       QPixmap pix(70,110);

//       QPainter p;
//       p.begin(&pix);
//       p.drawImage(pix.rect(),documentWidget->pixmap(),documentWidget->pixmap().rect());
//       p.end();
//       QString pixname=newName;
//       pix.save(m_pathCostm+"/pdfs/"+pixname.replace("pdf","png"));

//       QStringList tlist= documentWidget->infoKeys();


//       for (int i=0;i<tlist.count();i++){
//           QString name=tlist.at(i);
////           if(name.toLower()=="title"){
////               title=documentWidget->info(name);
////           }
//           if(name.toLower()=="author"){
//               auth=documentWidget->info(name);
//           }
//           QString txte= documentWidget->info(name);
//           betaka=betaka+name+"  = "+txte+"\n";

//       }

//       if(title.isEmpty())title=fi.baseName();


// delete  documentWidget;
    loadPdfPlugin(filname);
       if (Utils::addNewBook("pdfs/"+newName,Add_Book_Title,Add_Autor_Name,Add_Betaka,m_addGroupeId,false)==false)
       {
           QMessageBox::information(this,trUtf8("خطأ"), trUtf8("بيانات الكتاب خاطئة"));
           return false;
       }
       return true;
 //  }
  // delete  documentWidget;
  //  return false;
}

bool DialogAddBooks::loadPdfPlugin(const QString &filname)
{

QDir pluginsDir(Utils::getPluginDir());
    pluginsDir.cd("plugins");
      PdfInterface *pdfDoc;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();

qDebug()<<fileName;
        if (plugin) {
                pdfDoc = qobject_cast<PdfInterface *>(plugin);
                if (pdfDoc){

                    if(pdfDoc->setDocument(filname)==false){
                         qDebug()<<filname;
                        return false;
                    }
                     // QPixmap pix((pdfDoc->pixmap()->width()/100)*10,(pdfDoc->pixmap()->height()/100)*10);

                        QPixmap pix(70,110);
QImage image=pdfDoc->pixmap();
if(image.isNull())return false;
                          QPainter p;
                          p.begin(&pix);
                          p.drawImage(pix.rect(),image,image.rect());
                          p.end();
                              qDebug()<<"open"<<filname;
                          QString pixname=m_newPathDir;
                             qDebug()<<"pix"<<pixname.replace(".pdf",".png");
                          pix.save(pixname.replace(".pdf",".png"));

//                          QStringList tlist= pdfDoc->infoKeys();


//                          for (int i=0;i<tlist.count();i++){
//                              QString name=tlist.at(i);
//                   //           if(name.toLower()=="title"){
//                   //               title=pdfDoc->info(name);
//                   //           }
//                              if(name.toLower()=="author"){
//                                  Add_Autor_Name=pdfDoc->info(name);
//                              }
//                              QString txte= pdfDoc->info(name);
//                              Add_Betaka=name+"  = "+txte+"\n";

//                          }
                 //         delete pdfDoc;
                    return true;
                }
            }
        }

    return false;
}

