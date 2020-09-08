#include "tabbook.h"
#include "dialoggoto.h"
#include "utils.h"
#include <QLayout>
#include <QTextBrowser>
#include <QDir>
#include <QApplication>
TabBook::TabBook(QWidget *parent) :
    QWidget(parent),
    toolEdite(0),
    m_isTefsir(0),
    m_FahrasIsModified(0),
    m_PageIsModified(0),
    m_isEditable(0),
    m_isPdf(0),
    m_isBook(0),
    m_isRemoveTechkile(0),
    m_fitInwidth(0)
{

    setAttribute(Qt::WA_DeleteOnClose,true);

    DataBook=new databook();
#ifdef Q_OS_HAIKU
	m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
    QDir appDir(qApp->applicationDirPath());
    appDir.cd(".");
    m_pathApp=  appDir.absolutePath()+"/data";
#else
	m_pathUser=QDir::homePath()+"/.kirtasse";
    QDir appDir(qApp->applicationDirPath());
    appDir.cdUp();
    m_pathApp=  appDir.absolutePath()+"/share/elkirtasse";
#endif
    txtBrowserBook = new QTextBrowser(this);
    treeViewFahras =new QTreeWidget;

     setupTools();
     loadSettings();
    connect(this,SIGNAL(settingsChanged()),this,SLOT(loadSettings()));
}
TabBook::~TabBook()
{
    qDebug()<<"databok cleared-----------------------------";
    DataBook->claerBook();
    if(toolEdite){
        toolEdite->close();
        delete toolEdite;
        toolEdite=0;
    }
    delete DataBook;
}

void TabBook::loadSettings()
{
    //--------------------------------------------------------------------
#ifdef  Q_OS_HAIKU
    //QSettings settings(m_pathUser+"/setting.ini",QSettings::IniFormat);
	QSettings  settings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/setting.ini",QSettings::IniFormat);
#else
	QSettings settings(m_pathUser+"/data/setting.ini",QSettings::IniFormat);
#endif
    settings.beginGroup("MainWindow");
    bool isCadre=settings.value("WebCadre",true).toBool();
    QString cadreFolder=settings.value("CadrFolder","default").toString();
    m_pathCostm=settings.value("pathCostm",m_pathUser+"/books").toString();

#ifdef  Q_WS_X11
    m_WebFontTitle=settings.value("fontTitle","KacstTitle").toString();
    m_WebFontHachia=settings.value("fontHachia","KacstBook").toString();
#else
    m_WebFontTitle=settings.value("fontTitle","Andalus").toString();
    m_WebFontHachia=settings.value("fontHachia","Traditional Arabic").toString();
#endif

    QFont font;
    font.fromString(settings.value("font",trUtf8("Traditional Arabic,20,-1,5,50,0,0,0,0,0")).toString());

    m_WebFontKhase=settings.value("FontKhase","Jameel Noori Nastaleeq").toString();
    m_WebFontTitleSize=settings.value("fontTitleSize","20").toString();

    m_WebColorBack=settings.value("bcolor","#F9F9B9").toString();
    m_WebFontColor=  settings.value("WebFontColor","#000000").toString();
    m_WebFontFindColor=settings.value("WebFontFindColor","#aa5500").toString();
    m_WebFontPrefertColor=settings.value("WebFontPrefertColor","#264c00").toString();
    m_WebFontTitleColor=settings.value("WebFontTitleColor","#ff0000").toString();
    m_arabicNmber=settings.value("arabicNmber",0).toInt();

    pdfWidget->setCostmColor(settings.value("isPdfColor",false).toBool(),m_WebColorBack);

    settings.endGroup();
    txtBrowserBook->setFont(font);
    //تحديث الني عد التغيرات
    convertTextToHtml(txtBrowserBook->toPlainText());

    // انشاء اطار النص
    ceatCadre(cadreFolder,isCadre);

}

void TabBook::ceatCadre(const QString &cadreFolder, bool isCadre)
{
    foreach (QWidget *fw, m_listFrames) {
        m_listFrames.removeOne(fw);
        delete fw;
    }
    if(isCadre){
         txtBrowserBook->setFrameShape(QFrame::NoFrame);
    }else{
         txtBrowserBook->setFrameShape(QFrame::Panel);
         return;
    }
        QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
    appDir.cd(".");
    m_pathApp=  appDir.absolutePath()+"/data";
    QString imgPath=m_pathApp+"/images/"+cadreFolder;
#else
    appDir.cdUp();
    m_pathApp=  appDir.absolutePath()+"/share/elkirtasse";
    QString imgPath=m_pathApp+"/data/images/"+cadreFolder;
#endif
    if(cadreFolder==trUtf8("الافتراضي")){
        imgPath=":/images/image";
    }else{
        QFile file;
        if (!file.exists(imgPath+"/top.png") || !file.exists(imgPath+"/top_right.png")||!file.exists(imgPath+"/left.png")||
                !file.exists(imgPath+"/right.png")||!file.exists(imgPath+"/bot.png")||!file.exists(imgPath+"/bot_right.png")||
                !file.exists(imgPath+"/top_left.png")||!file.exists(imgPath+"/bot_left.png"))
        {imgPath=":/images/image";}

    }

    QPixmap pix(imgPath+"/top_right.png");
    QSize size(pix.width(),pix.height());

    QWidget *frame_top,*frame_topRight,*frame_left, *frame_right,*frame_bot,*frame_botRight,*frame_topLeft,*frame_botLeft;
    frame_top = new QWidget();
    frame_top->setStyleSheet("background-image: url("+imgPath+"/top.png);");

    gridLayout->addWidget(frame_top, 0, 1, 1, 1);

    frame_topRight = new QWidget();
    frame_topRight->setMinimumSize(size);
    frame_topRight->setStyleSheet("background-image: url("+imgPath+"/top_right.png);");

    gridLayout->addWidget(frame_topRight, 0, 0, 1, 1);

    frame_left = new QWidget();
    frame_left->setStyleSheet("background-image: url("+imgPath+"/left.png);");

    gridLayout->addWidget(frame_left, 1, 2, 1, 1);

    frame_right = new QWidget();
    frame_right->setStyleSheet("background-image: url("+imgPath+"/right.png);");

    gridLayout->addWidget(frame_right, 1, 0, 1, 1);

    frame_bot = new QWidget();
    frame_bot->setStyleSheet("background-image: url("+imgPath+"/bot.png);");


    gridLayout->addWidget(frame_bot, 2, 1, 1, 1);

    frame_botRight = new QWidget();
    frame_botRight->setMinimumSize(size);
    frame_botRight->setStyleSheet("background-image: url("+imgPath+"/bot_right.png);");

    gridLayout->addWidget(frame_botRight, 2, 0, 1, 1);

    frame_topLeft = new QWidget();
    frame_topLeft->setMinimumSize(size);
    frame_topLeft->setStyleSheet("background-image: url("+imgPath+"/top_left.png);");

    gridLayout->addWidget(frame_topLeft, 0, 2, 1, 1);

    frame_botLeft = new QWidget();
    frame_botLeft->setMinimumSize(size);
    frame_botLeft->setStyleSheet("background-image: url("+imgPath+"/bot_left.png);");

    gridLayout->addWidget(frame_botLeft, 2, 2, 1, 1);
    m_listFrames<<frame_top<<frame_topRight<<frame_left<<frame_right<<frame_bot<<frame_botRight<<frame_topLeft<<frame_botLeft;

}

void TabBook::setupTools()
{


    this->setLayoutDirection(Qt::RightToLeft);
    hLayoutPage = new QVBoxLayout(this);
    hLayoutPage->setSpacing(3);
    hLayoutPage->setContentsMargins(3, 3, 3, 3);
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);

    //---------------------------------
    //setup stackedWidget page for book page for pdf
    stackedWidget = new QStackedWidget(this);
     pageBook = new QWidget();
    stackedWidget->addWidget(pageBook);
    pagePdf = new QWidget();
    stackedWidget->addWidget(pagePdf);
    //setup page pdf
    QVBoxLayout     *verticalLayoutPdf = new QVBoxLayout(pagePdf);
    verticalLayoutPdf->setContentsMargins(0, 0, 0, 0);
    scrollArea=new QScrollArea(pagePdf);
    verticalLayoutPdf->addWidget(scrollArea);
    pdfWidget = new PdfWidget();
    scrollArea->setWidgetResizable(true);
    scrollArea->setContentsMargins(0, 0, 0, 0);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(pdfWidget);
    //setup page book
    QVBoxLayout     *verticalLayoutBook = new QVBoxLayout(pageBook);
    verticalLayoutBook->setContentsMargins(0, 0, 0, 0);
    verticalLayoutBook->addWidget(txtBrowserBook);


    gridLayout->addWidget(stackedWidget, 1, 1, 1, 1);

    stackedWidget->setCurrentWidget(pageBook);
     //----------------------------------
    hLayoutPage->addLayout(gridLayout);
//    mActionFitInwidth=new QAction(trUtf8("ناسب عرض الصفحة"),this);
    mActionNext=new QAction(trUtf8("الصفحة التالية"),this);
    mActionPrev=new QAction(trUtf8("الصفحة السابقة"),this);
    mActionFirst=new QAction(trUtf8("الصفحة الاولى"),this);
    mActionLast=new QAction(trUtf8("الصفحة الاخيرة"),this);
//mActionFitInwidth->setCheckable(true);

    if(this->layoutDirection()==Qt::RightToLeft){
        mActionFirst->setIcon(QIcon::fromTheme("go-last", QIcon(":/images/image/go-first.png")));
        mActionPrev->setIcon(QIcon::fromTheme("go-next", QIcon(":/images/image/go-back.png")));
        mActionNext->setIcon(QIcon::fromTheme("go-previous", QIcon(":/images/image/go-next.png")));
        mActionLast->setIcon(QIcon::fromTheme("go-first", QIcon(":/images/image/go-last.png")));

    }else{
        mActionFirst->setIcon(QIcon::fromTheme("go-first", QIcon(":/images/image/go-last.png")));
        mActionPrev->setIcon(QIcon::fromTheme("go-previous", QIcon(":/images/image/go-next.png")));
        mActionNext->setIcon(QIcon::fromTheme("go-next", QIcon(":/images/image/go-back.png")));
        mActionLast->setIcon(QIcon::fromTheme("go-last", QIcon(":/images/image/go-first.png")));

    }
//    mActionFitInwidth->setIcon(QIcon::fromTheme("zoom-fit-page", QIcon(":/images/image/zoom-fit-width.png")));

    connect(mActionFirst,SIGNAL(triggered()),this,SLOT(firstPage()));
    connect(mActionNext,SIGNAL(triggered()),this,SLOT(nextPage()));
    connect(mActionPrev,SIGNAL(triggered()),this,SLOT(prevPage()));
    connect(mActionLast,SIGNAL(triggered()),this,SLOT(lastPage()));
//    connect(mActionFitInwidth,SIGNAL(triggered(bool)),this,SLOT(actionFitInwidth(bool)));

    //connect(this,SIGNAL(bookAvailable(bool)),mActionFirst,SLOT(setEnabled(bool)));
//    connect(this,SIGNAL(bookAvailable(bool)),mActionNext,SLOT(setEnabled(bool)));
//    connect(this,SIGNAL(bookAvailable(bool)),mActionPrev,SLOT(setEnabled(bool)));
//    connect(this,SIGNAL(bookAvailable(bool)),mActionLast,SLOT(setEnabled(bool)));
//    connect(this,SIGNAL(bookAvailable(bool)),mActionFitInwidth,SLOT(setEnabled(bool)));

    emit bookAvailable(false);

    txtBrowserBook->setContextMenuPolicy(Qt::CustomContextMenu);
    pdfWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(txtBrowserBook, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenu()));
    connect(pdfWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenu()));
    connect(pdfWidget, SIGNAL(posChanged(int,int)), this, SLOT(scrollByMouse(int,int)));
    connect(pdfWidget, SIGNAL(requestZoomIn()), this, SLOT(zoomIn()));
    connect(pdfWidget, SIGNAL(requestZoomOut()), this, SLOT(zoomOut()));

    connect(txtBrowserBook, SIGNAL(highlighted(const QUrl)), this, SLOT(textBookHighlighted(QUrl)));
    connect(txtBrowserBook, SIGNAL(anchorClicked(const QUrl)), this, SLOT(textBookAnchorClicked(QUrl)));

    //شجرة الفهارس
    treeViewFahras->headerItem()->setText(0,"");

    connect(treeViewFahras, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(fahrasItemClicked(QTreeWidgetItem*)));
    connect(treeViewFahras, SIGNAL(itemClicked (QTreeWidgetItem* , int )), this, SLOT(fahrasItemClicked(QTreeWidgetItem*)));

    widgetFahras=new QWidget;
    vLayoutFahras = new QVBoxLayout(widgetFahras);

    vLayoutFahras->setContentsMargins(3, 3, 3, 3);

    vLayoutFahras->addWidget(treeViewFahras);

}

void TabBook::customContextMenu()
{

    if(m_isPdf){
        QAction *a;
        QMenu menu;

        menu.addAction(mActionNext);
        menu.addAction(mActionPrev);
        menu.addAction(mActionFirst);
        menu.addAction(mActionLast);
        menu.addSeparator();
        a=menu.addAction(QIcon::fromTheme("zoom-fit-page", QIcon(":/images/image/zoom-fit-width.png"))
                          ,trUtf8("ناسب عرض الصفحة"));
        connect(a,SIGNAL(triggered(bool)),this,SLOT(actionFitInwidth(bool)));
        a->setCheckable(true);
        a->setChecked(m_fitInwidth);
        a=menu.addAction(QIcon::fromTheme("zoom-in"),trUtf8("تكبير"),this,SLOT(zoomIn()));
        a=menu.addAction(QIcon::fromTheme("zoom-out"),trUtf8("تصغير"),this,SLOT(zoomOut()));

        menu.exec(QCursor::pos());
        return;
    }
    QMenu *menu=txtBrowserBook->createStandardContextMenu();
    menu->setLayoutDirection(Qt::RightToLeft);
    menu->addSeparator();
    if(m_isEditable==false){
        menu->addAction(mActionNext);
        menu->addAction(mActionPrev);
        menu->addAction(mActionFirst);
        menu->addAction(mActionLast);
    }else{
        if(toolEdite){
            menu->addAction(toolEdite->actionFahAddItem);
            menu->addAction(toolEdite->actionFahAddChild);
            menu->addSeparator();
            menu->addAction(toolEdite->actionNewPageAfter);
            menu->addAction(toolEdite->actionNewPageBefor);
            menu->addAction(toolEdite->actionRemovePage);
            menu->addSeparator();
            menu->addAction(toolEdite->actionFontCostum);
        }

    }

    menu->exec(QCursor::pos());
}
void TabBook::textBookAnchorClicked(QUrl txtUrl)
{
//    ui->textBrowser_0->setOpenExternalLinks(false);
//    ui->textBrowser_0->setOpenLinks(false);
    QString textCode=txtUrl.toString();
    if (textCode.isEmpty()) {return;}



        // QString nameTexte=textCode.section('/', 0,0);
        QString bookTexte=textCode.section('/', 1,1);
        QString authorTexte=textCode.section('/', 2,2);
        QString idBookTexte=textCode.section('/', 3,3);
        QString pageTexte=textCode.section('/', -1);
        QVariant pos=pageTexte;
        if (m_bookName==idBookTexte){
          bookValueChanged(pos.toInt());
        }else{
            openDataBase(idBookTexte,bookTexte,authorTexte,pos.toInt());
        //    bookValueChanged(pos.toInt());
        }

}

void TabBook::textBookHighlighted(QUrl txtUrl)
{
    QString textCode=txtUrl.toString();


        QString nameTexte=textCode.section('/', 0,0);
        QString bookTexte=textCode.section('/', 1,1);
        QString authorTexte=textCode.section('/', 2,2);
     //   ui->statusBar->showMessage(nameTexte +trUtf8(" كتاب :")+bookTexte+trUtf8(" المؤلف :")+authorTexte);

}

//! السجل الاول------------------------------------
void TabBook::firstPage()//
{
    if(m_isPdf){
        setPdfPage(1);
        return;
    }
    bookValueChanged(0);
}
//! السجل التالي-----------------------------------
void TabBook::nextPage()//
{
    if(m_isPdf){
        int i=  pdfWidget->pageCurent();
        setPdfPage(i+1);
        return;
    }
    if (DataBook->currentPosition()==DataBook->count()){return;}
    bookValueChanged(DataBook->currentPosition()+1);
}
//! السجل السابق------------------------------------
void TabBook::prevPage()//
{
    if(m_isPdf){
        int i=  pdfWidget->pageCurent();
        setPdfPage(i-1);
        return;
    }
    if (DataBook->currentPosition()==0){return;}
    bookValueChanged(DataBook->currentPosition()-1);
}
//! السجل الاخير-------------------------------------
void TabBook::lastPage()//
{
    if(m_isPdf){

        setPdfPage(pdfWidget->document()->numPages());
        return;
    }
    bookValueChanged(DataBook->count());
}

void TabBook::setPdfPage(int index)
{
    if(!pdfWidget->document())return;
    int count=pdfWidget->document()->numPages();
    if(index>count||index<1)return;

       pdfWidget->setPage(index);

       if (m_listHistoriePrev.isEmpty()||m_listHistoriePrev.last()!=index){
           m_listHistoriePrev.append(index);
       }

        emit prevHistorieEnabled(m_listHistoriePrev.count()>1);
         emit nextHistorieEnabled(m_listHistorieNext.count()>1);


 scrollArea->ensureVisible(0, 0);
 emit pagePartChanged(trUtf8(" كتاب : ")+m_title+trUtf8(".  المؤلف : ")+m_author+
                      trUtf8("الصفحة :")+QString::number(index));

}
//!
void TabBook::gotoPageDlg()
{
    if(m_isPdf){
        bool ok;
          int i = QInputDialog::getInt(this, trUtf8("الذهاب الى صفحة"),
                                       trUtf8("Enter number:"), pdfWidget->pageCurent(),
                                       1, pdfWidget->document()->numPages(), 1, &ok);
          if (ok)
            setPdfPage(i);
          return;
    }
    if (m_bookName.isEmpty()){  return; }
    dialoggoto *dlggo=new dialoggoto(this);
    dlggo->pathBook= m_bookPath+"/book.xml";
    dlggo->rowCount=DataBook->count();
    dlggo->positon=DataBook->currentPosition();
    dlggo->updateValue();
    if (dlggo->exec() == QDialog::Accepted) {
        int id=dlggo->positon-1;
        bookValueChanged(id);
    }
    delete dlggo;
}

void TabBook::gotoPage(int pos)
{
     bookValueChanged(pos);
}

bool TabBook::gotoSoraAya(const QString &mysora,const QString &myaya)
{

    if (DataBook->findAya(mysora,myaya)==true)  {
        bookValueChanged(DataBook->currentPosition());
        return true;

    }
    return false;
}

//! navigation---التقل بين السجلات---------------------
void TabBook::bookValueChanged(int pos)//التنقل بين السجلات حسب قيمة
{
    /*
    if (m_bookName[m_currentIndex].isEmpty()){
        AC_prevHistorie->setEnabled(false);
        AC_nextHistorie->setEnabled(false);
        return; }
   */
       if(txtBrowserBook->document()->isModified())
        updatPage("",txtBrowserBook->document()->toPlainText().trimmed(),true);



    DataBook->moveToPosition(pos);
    if (m_listHistoriePrev.isEmpty()||m_listHistoriePrev.last()!=DataBook->currentPosition()){
        m_listHistoriePrev.append(DataBook->currentPosition());
    }

     emit prevHistorieEnabled(m_listHistoriePrev.count()>1);
      emit nextHistorieEnabled(m_listHistorieNext.count()>1);





    convertTextToHtml(DataBook->currentNass());

    QString page=DataBook->currentPage();
    QString part=DataBook->currentPart();
   emit pagePartChanged(trUtf8(" كتاب : ")+m_title+trUtf8(".  المؤلف : ")+m_author+
                        trUtf8("الصفحة :")+page +"  "+trUtf8("الجزء :")+part);

}
void TabBook::historieNext()
{
    if (m_bookName.isEmpty()){  return; }
    if ( m_listHistorieNext.isEmpty() ||  m_listHistorieNext.count()<1){
       emit nextHistorieEnabled(false);
        return; }
    int p= m_listHistorieNext.last();
    m_listHistorieNext.removeLast();
    if ( m_listHistorieNext.count()<1 ){
        emit nextHistorieEnabled(false);
    }
    m_listHistoriePrev.append(p);
    emit prevHistorieEnabled(true);
    if(m_isPdf){
        setPdfPage(p);
        return;
    }
    DataBook->moveToPosition(p);
 convertTextToHtml(DataBook->currentNass());
    QString page=DataBook->currentPage();
    QString part=DataBook->currentPart();
       emit pagePartChanged(trUtf8("الصفحة :")+page +"  "+trUtf8("الجزء :")+part);
}

void TabBook::historiePrev()
{
    if (m_bookName.isEmpty()){  return; }
    if ( m_listHistoriePrev.isEmpty() || m_listHistoriePrev.count()<2){
        emit prevHistorieEnabled(false);
        return; }

    int p=  m_listHistoriePrev.last();
    m_listHistoriePrev.removeLast();
    if ( m_listHistoriePrev.count()<2)
    {
        emit prevHistorieEnabled(false);
    }
    m_listHistorieNext.append(p);
     emit nextHistorieEnabled(true);

    int i=  m_listHistoriePrev.last();
    if(m_isPdf){
        setPdfPage(i);
        return;
    }
    DataBook->moveToPosition(i);
 convertTextToHtml(DataBook->currentNass());
    QString page=DataBook->currentPage();
    QString part=DataBook->currentPart();
    emit pagePartChanged(trUtf8("الصفحة :")+page +"  "+trUtf8("الجزء :")+part);
}

//! النقر على عنصر شجرة الفهرسة
void TabBook::fahrasItemClicked(QTreeWidgetItem* item)//
{
    if(m_isPdf){
        QString id=item->data(0,1).toString().section(";",1,1);
         setPdfPage(id.toInt());
         qreal rx=item->data(0,1).toString().section(";",2,2).toDouble();
          qreal ry=item->data(0,1).toString().section(";",5,5).toDouble();
       QRect rect=   pdfWidget->matrix().mapRect(QRectF(rx*1000,ry*1000,5,5)).toRect();
       scrollArea->ensureVisible(rect.x(),rect.y());
        return;
    }
        bookValueChanged(item->data(1,1).toInt()-1);

        int index=txtBrowserBook->toPlainText().indexOf(item->text(0));
        if (index>0){
            QTextCursor cursor(txtBrowserBook->document());
            cursor.setPosition(index,QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
            txtBrowserBook->setTextCursor(cursor);
        }
        treeViewFahras->setCurrentItem(item);
   // }
    //
}


//! open book---*فتح الكتب--------------------------------------------------------
bool TabBook:: openDataBase(const QString &name,const QString &title,const QString &Autor,int position)
{

    //اذا كان نفس الكتاب مفتوحا
    //    if(m_bookName==name)
    //    return false;
    //حفظ التغييرات اذا كان هناك تغيير
    if(txtBrowserBook->document()->isModified()){
        updatPage("",txtBrowserBook->document()->toPlainText(),true);

    }

    if (closeMessages()==false){

        return false;
    }


    //التأكد من وجود الكتاب في المسار
    QString bkpath;           //مسار الكتاب

    qDebug()<<name;
    m_bookPath=Utils::getBookPath(name,m_pathCostm);
    if(m_bookPath.isEmpty())
        return false;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if(name.startsWith("pdfs")&&name.endsWith("pdf")){

        if (pdfWidget->setDocument(m_bookPath)) {
            pdfWidget->loadToc(treeViewFahras);

            //   scalPdf=1.0;
            stackedWidget->setCurrentWidget(pagePdf);
            m_isBook=false;
            m_isPdf=true;
            emit bookAvailable(false);
            emit pdfAvailable();
            m_fitInwidth=false;
            scalPdf=1.0;
            pdfWidth=pdfWidget->pixmap()->width();
            setPdfPage(position);
//            mActionFitInwidth->setEnabled(true);
//               mActionNext->setEnabled(true);
//               mActionPrev->setEnabled(true);
//               mActionFirst->setEnabled(true);
//               mActionLast->setEnabled(true);
        }else{
            QApplication::restoreOverrideCursor();
            return false;
        }
    }else{

        bkpath=m_bookPath+"/book.xml";

        //فتح الكتاب
        if ( DataBook->openBook(bkpath,position)==true)
        {


            //        txtBrowserBook->setPlainText(DataBook->currentNass());
            //        convertTextToHtml(DataBook->currentNass())
            m_isTefsir=DataBook->isTefsir() ;
            stackedWidget->setCurrentWidget(pageBook);

            m_isBook=true;
            m_isPdf=false;
            pdfWidget->closeDoc();
              bookValueChanged(position);
            emit bookInfoChanged();

            emit bookAvailable(true);
        }else{
            QMessageBox::information(this,trUtf8("خطأ"),bkpath + trUtf8("التنسيق غير صحيح ") );
            QApplication::restoreOverrideCursor();
            return false;
        }

        //تحميل الشجرة الكتاب
        Utils::treeChargeFahrass(treeViewFahras,m_bookPath);
    }
    m_bookName=name;
    //استعادة اصل المتغيرات
    if(Utils::chekBookInfo(m_bookPath)==true){
        QHash<QString,QString>hash=Utils::loadBookInfo(m_bookPath);

        m_title=hash.value("title");
        m_author=hash.value("author");
        m_betaka=hash.value("betaka");

    }else{
        m_title=title;
        m_author=Autor;
        m_betaka="";
    }

    m_listHistoriePrev.clear();
    m_listHistorieNext.clear();
    m_listHistoriePrev.append(position);

    emit nextHistorieEnabled(false);
    emit prevHistorieEnabled(false);
    /*
 ui->dockWidget_fahras->setShown(true);
   */

    treeViewFahras->setColumnCount(2);
    treeViewFahras->setColumnHidden(1,true);
    treeViewFahras->setDragDropMode(QAbstractItemView::NoDragDrop);
    treeViewFahras->setSelectionMode(QAbstractItemView::SingleSelection);
    m_FahrasIsModified=false;
    m_PageIsModified=false;
    //  ui->frameEdit->setVisible(false);
    //  ui->widgetEditBook->setVisible(false);
    // m_toolEditFahrass=false;
    txtBrowserBook->setReadOnly(true);
    txtBrowserBook->setUndoRedoEnabled(false);


    treeViewFahras->headerItem()->setText(0,m_title);
    QApplication::restoreOverrideCursor();
    return true;
}
//! تحويل بيانات النص
void TabBook::convertTextToHtml(const QString &txt)//
{
    QString text=txt;
    //  QLocale::setDefault(QLocale(QLocale::Arabic));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    if(m_arabicNmber==1){
        txtBrowserBook->setLocale(QLocale(QLocale::Latin));
    }else if(m_arabicNmber==2){
        txtBrowserBook->setLocale(QLocale(QLocale::Arabic));
    }
    text.replace(QByteArray("\n"), QByteArray("<br>"));
    text.replace(QByteArray("\r"), QByteArray("<br>"));
    text.insert(text.length(),"</font></body>");
    //*******************************************
    text.replace("$", "");
    int id=DataBook->currentPosition()+1;
    QVariant dd=id;
    QTreeWidgetItem *item=new QTreeWidgetItem;
    QList<QTreeWidgetItem *> found =
            treeViewFahras->findItems(
                dd.toString(),Qt::MatchExactly | Qt::MatchRecursive,1);//تنشاء قائمة بكل العناصر
    QLocale lc(txtBrowserBook->locale().name());
    lc.setNumberOptions(QLocale::OmitGroupSeparator);
    QRegExp rxD("(\\d+)");

    int pos = 0;

    while ((pos = rxD.indexIn(text, pos)) != -1) {

        text.remove(pos,rxD.matchedLength());
        text.insert(pos,lc.toString(rxD.cap(1).toInt()));


        pos += rxD.matchedLength();
    }
    foreach (item, found){
        treeViewFahras->setCurrentItem(item);
        QString title= " <span style='font-family:"+m_WebFontTitle+";  font-size:"+m_WebFontTitleSize+"pt; color:"+m_WebFontTitleColor+";'>"+item->text(0)+"</span>";
        QString txtItem=item->text(0).trimmed();

        text.replace( txtItem+"<br>","<br>"+ title+"<br>");
        text.replace("  "+ txtItem,"<br>"+ title+"<br>");
        // txt.replace("<br> "+ txtItem,"<br>"+title);
        text.replace("<br>"+ txtItem+"</font>","<br>"+title+"</font>");
        text.replace( txtItem+" <br>","<br>"+ title+"<br>");
        text.replace( txtItem+" :","<br>"+ title+":<br>");
        text.replace( txtItem+":","<br>"+ title+":<br>");
        //  txt.replace( txtItem+".","<br>"+ title+"txt.<br>");
        text.replace( txtItem+" ؟","<br>"+title+" ؟<br>");
        text.replace("[ "+ txtItem+" ]","<br>"+title+"<br>");
        text.replace("["+ txtItem+"]","<br>"+title+"<br>");

        if(text.trimmed()== txtItem){ //
            text=title;
        }
    }
    //********************************************xtToHtml
    //-************
    text.replace("begen!§"," < font face='"+m_WebFontKhase+"'>");
    text.replace("end!§","</font>");
    //-***************
    QRegExp rxi("_____+");
    QRegExp rxd("ـــــ+");
    text.replace(rxi," < font face='"+m_WebFontHachia+"' size='1'><br>__________");
    text.replace(rxd," < font  face='"+m_WebFontHachia+"' size='1'><br>__________");
//    text.replace("صلى الله عليه وسلم","< font color='"+m_WebFontPrefertColor+"' >صلى الله عليه وسلم</font> ");
    text.replace("C تعالى","< font color='"+m_WebFontPrefertColor+"' >رحمه الله تعالى</font> ");
    text.replace("{","< font color='"+m_WebFontPrefertColor+"' >﴿ ");
    text.replace("}","﴾</font> ");

//    text.replace("رضي الله عنهم ",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'>رضي الله عنهم </font> "));
 //   text.replace("رضي الله عنه ",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'>رضي الله عنه </font> "));
    text.replace("رضي الله عنهما ",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("' >رضي الله عنهما </font> "));
   // text.replace(" A ",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'> صلى الله عليه سلم </font> "));
  //  text.replace("Bه",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'>رضي الله عنه </font> "));
    text.replace(" D ",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'>  تعالى </font> "));
 //   text.replace(" E ",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'> عليه السلام </font> "));
 //   text.replace("النبي e",trUtf8("< font color='")+m_WebFontPrefertColor+trUtf8("'> النبي صلى الله عليه سلم </font> "));


  text.replace(trUtf8("صلى الله عليه وسلم"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8(" A "),QString(" <  font face='AGA Arabesque' size='3' color=%2>%1</font> ").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8("النبي e"),QString("النبي <  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8("النبي A"),QString("النبي <  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8("النَّبِيِّ j"),QString("النبي <  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8("النبي j"),QString("النبي <  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8("رَسُولِ اللَّهِ j"),QString("رسول الله <  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8("رسول الله j"),QString("رسول الله <  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\262")).arg(m_WebFontPrefertColor));

  text.replace(trUtf8("عليه السلام"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\265")).arg(m_WebFontPrefertColor));
  text.replace(trUtf8(" E "),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\265")).arg(m_WebFontPrefertColor));


  text.replace(trUtf8("عز وجل"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\225")).arg(m_WebFontPrefertColor));
//  text.replace(trUtf8("رضي الله عنهما"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\264")).arg(m_WebFontPrefertColor));

//    text.replace(trUtf8("رضي الله عنهم"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\264")).arg(m_WebFontPrefertColor));

   text.replace(trUtf8("رضي الله عنه"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\264")).arg(m_WebFontPrefertColor));
   text.replace(trUtf8("Bه"),QString("<  font face='AGA Arabesque' size='3' color=%2>%1</font>").arg(trUtf8("\357\201\264")).arg(m_WebFontPrefertColor));

   text.replace(trUtf8("* * *"),QString("<p dir='rtl' align=\"center\" ><  font face='AGA Arabesque'  color=%2>%1%1%1</font></p>").arg(trUtf8("\357\200\241")).arg(m_WebFontPrefertColor));

    if (text.contains("**")){
        QRegExp rx("(\\d\\d? )[(]");
        text.replace(rx,"<br>(");
        QRegExp rx1("(\\d\\d?)[(]");
        text.replace(rx1,"<br>(");
        text.replace(") (",")<br>(");
        text.replace(")(",")<br>(");
    }
    text.replace("<br><br>","<br>");
    text.replace("<br> <br>","<br>");
    text.replace("<br>  <br>","<br>");



    QString bgrcolor = "<body bgcolor='"+m_WebColorBack+"'>< font color='"+m_WebFontColor+"' >";
   txtBrowserBook->setHtml(bgrcolor+ text.trimmed() );
//    if (txtBrowserBook->objectName()!="textBrowser_0"){
//        txtBrowserBook->setHtml(bgrcolor+ txt.trimmed() );
//    }
}

void  TabBook::saveFahrass()
{

    if ( Utils::fahrasSave(treeViewFahras,m_bookName,m_pathCostm)==true){
        QMessageBox::information(0,trUtf8("معلومات"), trUtf8("تمت عملية حفظ الفهرسة بنجاح"));

    }else{
        QMessageBox::information(0,trUtf8("خطأ"), trUtf8("خطأ في كتابة الملف"));
    }
    m_FahrasIsModified=false;
}
void TabBook::saveCurrentBook()
{

    if (DataBook->saveBook(m_bookPath)==true){
        QMessageBox::information(0,trUtf8("معلومات"), trUtf8("تمت عملية حفظ الكتاب بنجاح"));
    }
    m_PageIsModified=false;
}
int TabBook::getBookPos() const
  {
    if(m_isPdf)return pdfWidget->pageCurent();
    return DataBook->currentPosition();
}
int TabBook::getRowCount() const
  {
    return DataBook->count();
}
void TabBook::updatPage(const QString &oldText,const QString &newText,const bool fullText)
{
    DataBook->updatPage(oldText,newText,fullText);
    m_PageIsModified=true;
    qDebug()<<"m_PageIsModified----1"<<m_PageIsModified;
}
void TabBook::organizFahras()
{
     DataBook->treeOrganizFahrass(treeViewFahras,m_bookName);
      m_FahrasIsModified=true;
}
void TabBook::removeTechkil(const bool arg)
{

        DataBook->removeTechkile(arg);
m_isRemoveTechkile=arg;
    bookValueChanged(DataBook->currentPosition());
}
bool TabBook::closeRequested()
{
    if (txtBrowserBook->document()->isModified()){
        qDebug()<<"m_PageIsModified----2"<<m_PageIsModified;
        DataBook->updatPage("",txtBrowserBook->document()->toPlainText(),true);
        m_PageIsModified=true;
    }

    if (closeMessages()==false){

        return false;
    }


     return true;
}

bool TabBook::closeMessages()
{
    if (m_FahrasIsModified==true){
        int reponse = QMessageBox::warning(0, trUtf8("تنبيه"), trUtf8("هل تريد حفظ فهرسة : ") +m_title, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reponse == QMessageBox::Save){
            saveFahrass();


        }else if (reponse == QMessageBox::Cancel){

            return false;
        }
    }
    if (m_PageIsModified==true){
        int reponse = QMessageBox::warning(0, trUtf8("تنبيه"), trUtf8("هل تريد حفظ التغييرات على كتاب : ")+m_title+ " ؟" , QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reponse == QMessageBox::Save){
            saveCurrentBook();

        }else if (reponse == QMessageBox::Cancel){
            return false;
        }
    }

    return true;
}
/*********************************************************************
 *
 **********************************************************************/
//! EDITE
void TabBook::setEditBook(const bool edit)
{
    if(edit==true){
        if(!toolEdite){
            toolEdite=new ToolEdite;
            vLayoutFahras->insertWidget(0,toolEdite->frameEdit);
            hLayoutPage->addWidget(toolEdite->toolBarEditBookPage);
            m_isEditable=true;

            txtBrowserBook->setReadOnly(false);
            txtBrowserBook->setUndoRedoEnabled(true);
            treeViewFahras->setDragDropMode(QAbstractItemView::InternalMove);//تفعيل عملية النقل بالفأرة
            treeViewFahras->setSelectionMode(QAbstractItemView::ExtendedSelection);

            // connect(toolEdite,close(),this,SLOT(setEditBook(bool));

            connect(toolEdite, SIGNAL(itemFahrasMoveDown()), this, SLOT(itemFahrasMoveDown()));
            connect(toolEdite, SIGNAL(itemFahrasMoveUp()), this, SLOT(itemFahrasMoveUp()));
            connect(toolEdite, SIGNAL(itemFahrasRename()), this, SLOT(itemFahrasRename()));
            connect(toolEdite, SIGNAL(itemFahrasRemove()), this, SLOT(itemFahrasRemove()));
            connect(toolEdite, SIGNAL(itemFahrasAdd()), this, SLOT( itemFahrasAdd()));
            connect(toolEdite, SIGNAL(itemFahrasAddChild()), this, SLOT( itemFahrasAddChild()));

            connect(toolEdite, SIGNAL(newPageAfter()), this, SLOT( newPageAfter()));
            connect(toolEdite, SIGNAL(newPageBefort()), this, SLOT( newPageBefort()));

            connect(toolEdite, SIGNAL(removePage()), this, SLOT( removePage()));
            connect(toolEdite, SIGNAL(fontCostum()), this, SLOT( fontCostum()));
            connect(toolEdite, SIGNAL(saveBookEndFahras()), this, SLOT( saveBookEndFahras()));

            connect(toolEdite, SIGNAL(goFindFah(QRegExp,QRegExp)), this, SLOT( goFindFah(QRegExp,QRegExp)));
            connect(txtBrowserBook, SIGNAL(copyAvailable(bool)), toolEdite, SLOT(setCopyAvailable(bool)));

        }
    }else{
        if(toolEdite){
            toolEdite->close();
            //        delete toolEdite->frameEdit;
            //        delete toolEdite->toolBarEditBookPage;
            delete toolEdite;
            toolEdite=0;
            m_isEditable=false;

            txtBrowserBook->setReadOnly(true);
            txtBrowserBook->setUndoRedoEnabled(false);
            treeViewFahras->setDragDropMode(QAbstractItemView::NoDragDrop);
            treeViewFahras->setSelectionMode(QAbstractItemView::SingleSelection);
        }

    }
}
//EDIT ***  FAHRAS
void TabBook::itemFahrasAdd()
{
    addItemInTreeFahras(false);
}
//EDIT ***  FAHRAS
void TabBook::itemFahrasAddChild()
{
    addItemInTreeFahras(true);
}
//EDIT ***  FAHRAS
void TabBook::addItemInTreeFahras(bool child)//اظافة عنصر الى الفهرسة
{
    QString text=txtBrowserBook->textCursor().selectedText();
    if (text.isEmpty()){ return;}
    QTreeWidgetItem *curentitem;//العنصر المحدد
    QTreeWidgetItem *item;//العنصر الذي سنظيفه
    curentitem= treeViewFahras->currentItem();
    if (child==false){ //اظافة عنصر جديد بعد المحدد
        if (!curentitem){                   //اذا لم يكن هناك تحديد سيظاف الى الجذر
            item= new QTreeWidgetItem(treeViewFahras);
        }else{        // اما ان كان هناك عنصر محدد سيظاف بعده
            item= new QTreeWidgetItem(curentitem->parent(),curentitem);
            if (!curentitem->parent()){    //اذا لم يكن له اب سيضاف الى الجذر بعد المحدد
                item= new QTreeWidgetItem(treeViewFahras,curentitem);
            }
        }
    }
    if (child==true){             //اظافة ابن الى المحدد
        if (!curentitem){ return;}                   //اذا لم يكن هناك تحديد سيخرج
        item= new QTreeWidgetItem(curentitem);
        if ( item->parent()){
            item->parent()->setExpanded(true);
        }
    }
    item->setText(0,text );

    item->setData(1,1,DataBook->currentPosition()+1);
    item->setSelected(true);
    treeViewFahras->setCurrentItem(item);//تفعيل العنصر المظاف
    m_FahrasIsModified=true;
}
//EDIT ***
void TabBook::saveBookEndFahras()//زر حفظ الفهرسة المحررة
{
    if (m_FahrasIsModified==true){
        saveFahrass();
    }
    if (m_PageIsModified==true){
        saveCurrentBook();
    }
}
//EDIT ***  FAHRAS

//EDIT ***  FAHRAS
void TabBook::itemFahrasRename()
{
    QTreeWidgetItem *item=treeViewFahras->currentItem();
    if(!item){ return;}
    QInputDialog *dff=new QInputDialog(this);
    dff->setOkButtonText(trUtf8("موافق"));
    dff->setLayoutDirection(Qt::RightToLeft);
    dff->setCancelButtonText(trUtf8("الغاء"));
    dff->setLabelText(trUtf8("ادخل اسما جديدا"));
    dff->setWindowTitle(trUtf8("اعادة تسمية العنصر"));
    dff->setTextValue(item->text(0));
    if (dff->exec()==QInputDialog::Accepted ){
        QString  newtext = dff->textValue();
        if (newtext.isEmpty()){ return;}
        item->setText(0,newtext);
        m_FahrasIsModified=true;
    }
}
//EDIT ***  FAHRAS
void TabBook::itemFahrasRemove()//حذف عنصر من الفهرسة
{
    if (Utils::treeviewItemRemove(treeViewFahras)==true){
        m_FahrasIsModified=true;}
}
//EDIT ***  FAHRAS
void TabBook::itemFahrasMoveDown()//نقل عنصر للاسفل في الفهرسة
{
    if (Utils::treeviewItemDown(treeViewFahras)==true){
        m_FahrasIsModified=true;}
}
//EDIT ***  FAHRAS
void TabBook::itemFahrasMoveUp()//نقل عنصر للاعلى في الفهرسة
{
    if (Utils::treeviewItemUp(treeViewFahras)==true){
        m_FahrasIsModified=true;}
}
//EDIT ***  FAHRAS
void TabBook::goFindFah(QRegExp rx,QRegExp rx2)//انشاء فهارس
{


    Utils::findFahariss(treeViewFahras,m_bookPath,rx
                        ,rx2, DataBook->count()  );
    m_FahrasIsModified=true;

}

//EDIT ***  BOOk
void TabBook::newPageAfter()
{
    DataBook->insertPage(true);
  bookValueChanged(DataBook->currentPosition());
    m_PageIsModified=true;
}
//EDIT ***  BOOk
void TabBook::newPageBefort()
{
    DataBook->insertPage(false);
   bookValueChanged(DataBook->currentPosition());
    m_PageIsModified=true;
}
//EDIT ***  BOOk
void TabBook::removePage()
{
    DataBook->removeCurPage();
   bookValueChanged(DataBook->currentPosition());
    m_PageIsModified=true;
}
//EDIT ***  BOOk
void TabBook::fontCostum()
{
    QString text=txtBrowserBook->textCursor().selectedText();
    txtBrowserBook->setText( txtBrowserBook->document()->toPlainText().replace(text,"begen!§"+text+"end!§"));
    DataBook->updatPage("",txtBrowserBook->toPlainText(),true);
convertTextToHtml(DataBook->currentNass());
    m_PageIsModified=true;
}
//--------pdfs
void TabBook::fitInwidthPdf()
{
    if(m_fitInwidth==true){
    qreal w=scrollArea->width()-(scrollArea->verticalScrollBar()->width());


            scalPdf=w/   pdfWidth;;
            if(pdfWidget->document())
            pdfWidget->setScale(scalPdf);
    }
}
void TabBook::actionFitInwidth(bool checked)
{
    m_fitInwidth=checked;
    if(checked==true){
        fitInwidthPdf();
    }else{
         pdfWidget->setScale(1.0);
    }
}
void TabBook::scrollByMouse(int x,int y)
{
    int vx=scrollArea->horizontalScrollBar()->value();
    int vy=scrollArea->verticalScrollBar()->value();
    scrollArea->horizontalScrollBar()->setValue(vx+x);
    scrollArea->verticalScrollBar()->setValue(vy+y);
}
void TabBook::zoomOut()
{
    if(m_isPdf){
        if(scalPdf==0.25)return;
        scalPdf=scalPdf-0.25;
        pdfWidget->setScale(scalPdf);
        m_fitInwidth=false;
//        mActionFitInwidth->setChecked(false);
        return;
    }
    txtBrowserBook->zoomOut();
}

void TabBook::zoomIn()
{
    if(m_isPdf){
        if(scalPdf==4.0)return;
        scalPdf=scalPdf+0.25;
        pdfWidget->setScale(scalPdf);
        m_fitInwidth=false;
//        mActionFitInwidth->setChecked(false);
        return;

    }
        txtBrowserBook->zoomIn();
}
void TabBook::serchInPdf(const QString &txt)
{
     QRectF rectF;
     //اذا كان اتجاه النص من اليمين الى اليسار
    if(QApplication::keyboardInputDirection()==Qt::RightToLeft){
        QString str;
        for(int i=txt.length();i>-1;i--){
            str+=txt.mid(i,1);
            qDebug()<<str;
        }
         rectF=   pdfWidget->searchForwards(str);

    }
    else //اذا كان الاتجاه من اليسار الى اليمين
       rectF=  pdfWidget->searchForwards(txt);
    //جلب الموصع س وع للنص المبحوث عنه
    QRect rect=pdfWidget->matrix().mapRect(rectF).toRect();
    scrollArea->ensureVisible(rect.x(),rect.y());
    emit pagePartChanged(trUtf8(" كتاب : ")+m_title+trUtf8(".  المؤلف : ")+m_author+
                         trUtf8("الصفحة :")+QString::number(pdfWidget->pageCurent()));
}
void TabBook::updateSignals()
{
    if(m_isBook){
        emit bookAvailable(true);
        QString page=DataBook->currentPage();
        QString part=DataBook->currentPart();
       emit pagePartChanged(trUtf8(" كتاب : ")+m_title+trUtf8(".  المؤلف : ")+m_author+
                            trUtf8("الصفحة :")+page +"  "+trUtf8("الجزء :")+part);

    }else if(m_isPdf){
        emit bookAvailable(false);
        emit pdfAvailable();

        emit pagePartChanged(trUtf8(" كتاب : ")+m_title+trUtf8(".  المؤلف : ")+m_author+
                             trUtf8("الصفحة :")+QString::number(pdfWidget->pageCurent()));

    }else{
         emit bookAvailable(false);
         emit pagePartChanged(trUtf8(" كتاب : ")+trUtf8(".  المؤلف : ")+
                              trUtf8("الصفحة :"));

    }

}
