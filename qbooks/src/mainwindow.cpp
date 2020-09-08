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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "classwizard.h"
#include <QtGui>
#include  "dialog.h"
#include  "about.h"
#include  "dialogfind.h"
#include  "dialogconvert.h"
#include  "dialogupdat.h"
#include  "dialogfavo.h"
#include  "dialogconfigbooks.h"
#include  "dialogactions.h"
#ifdef   Q_OS_WIN
#include  "dialogmdb.h"
#else
#include  "mdbexport.h"
#endif
#include  "dialoggoto.h"
#include  "dialogoption.h"
#include  "print.h"
#include "dialogimportdvd.h"

#include <QMessageBox>

#define NET_PLUG 0
#define SHAMILA_PLUG 1
#define ROWAT_PLUG 2

//#define DOC_FORMAT_NOAVAILABLE  0
//#define PDF_PLUG_NOEXIST   1
//#define DOC_FILE_NOEXIST  2
//#define DOC_CANCELED 3
//#define DOC_AVAILABLE 4
//تحميل البرنامج
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass),curentBook(0)
{
    ui->setupUi(this);
    recentBooks = new RecentBooks();
    Findbook=new findbook();

    //Rowat=new rowat();
#if QT_VERSION >= 0x040600
    view= new View(this);
#endif

    QDir appDir(qApp->applicationDirPath());
#ifdef Q_OS_HAIKU
	appDir.cd(".");
	m_pathApp=  appDir.absolutePath()+"/data";
#else
    appDir.cdUp();
    m_pathApp=  appDir.absolutePath()+"/share/elkirtasse";
#endif

    m_treeGroupIsModified=false;

  //  ui->lineEdit->setVisible(false);
  //  m_listTab.append("0");
    m_findIsTitle=true;
 //   txtBrowserBook = ui->tabWidget->currentWidget()->findChild<QTextBrowser *>();
    //تحميل ملف السمة وبيانات الطول و العرض ووو
    //**********************عمليات التحميل*******

    loadSettings();
    loadLayout();
    updateIconEndAction();

     BookIntro=new TabBook;
     m_listTabBook.append(BookIntro);

      setupPageIntro();
        chargeIconsThemes();
  //  }
  //  m_listActions=new   QList<QAction *>;
    m_listActions.append(ui->menu_file->actions());

    m_listActions.append(ui->menuEdit->actions());
    m_listActions.append(ui->menuView->actions());
    m_listActions.append(ui->menuTools->actions());
    m_listActions.append(ui->menuHelp->actions());
    m_listActions.append(AC_prevHistorie);
    m_listActions.append(AC_nextHistorie);
   m_listActions.append(AC_recent);
    toolRubon =new toolrubon();
    addToolRubonBar(m_isStandardToolBar);
    ui->menuBar->setVisible(m_isStandardToolBar);


    Utils::favoriteCharge(ui->treeWidget_fav,m_icondir,m_icondfile);

   Utils::treeChargeJozaa(ui->treeWidget_curaan);
       //تحميل اجزاء القرءان
    recentBooks->recentCharge();               //تحميل قائمة الكتب السابقة
    chargeGroupe();
    emit bookAvailable(false);


    ui->actionDownloadBooks->setEnabled(loadPlugin(NET_PLUG));
    ui->actionShamilaCdrom->setEnabled(loadPlugin(SHAMILA_PLUG));

    const QStringList args = QCoreApplication::arguments();
    if (args.count() == 2){
        QString   temeIndex = args.at(1);

        if (temeIndex=="-f"){

            this->showFullScreen();
        }
    }else if(args.count() == 3){
        QString   arg = args.at(1);
        QString  argStyle=args.at(2);
        qDebug()<<"arg :"<<argStyle;
        if(arg.toLower()=="-s"){
//            m_isStyleCostum=true;
//            m_styleCostum=argStyle;
//            qDebug()<<"arg :"<<m_styleCostum;

        }
    }



}
//عند خروج البرنامج
MainWindow::~MainWindow()
{
    recentBooks->recentSave(); //حفظ الكتب السابقة
    if(!this->isFullScreen()){
        saveLayou();
    }
Utils::favoriteSave(ui->treeWidget_fav);

    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    Findbook ->stopFind=true;
    if (m_treeGroupIsModified==true){
        int reponse = QMessageBox::warning(this, trUtf8("تنبيه"), trUtf8("هل تريد حفظ التغييرات المدخلة على مجموعة الكتب ") ,QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reponse == QMessageBox::Save){
            if ( Utils::treeSaveGroupe(ui->treeWidget_books)){
                QMessageBox::information(this,trUtf8("معلومات"), trUtf8("تمت عملية حفظ مجموعة الكتب بنجاح"));
                m_treeGroupIsModified=false;
            }
        }else if (reponse == QMessageBox::Cancel){
            e->ignore();
            return;
        }
    }

   // int  tabcount=ui->tabWidget->count();
    //حفظ الكتب في حالة تغيرها عند الخروج
    foreach (TabBook *tab, m_listTabBook) {
        if(!tab->getBookName().isEmpty()){
            if(tab->closeRequested()==false){
                e->ignore();
                return;
            }
            int idbk=tab->getBookPos();
            recentBooks->recentChange(tab->getBookName(),
                                     tab->getTitle(),
                                      tab->getAuthor(),
                                     QString::number(idbk),
                                      m_recentNbr,
                                      tab->getBookIsTefsir());

        }
    }

     e->accept();
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    int b=listLineEditName.indexOf(obj->objectName());

    if (obj->objectName() == listLineEditName.at(b)) {

        if (event->type() == QEvent::FocusIn && obj->property("text")==trUtf8("بحث")) {
            obj->setProperty("text","");
            obj->setProperty("styleSheet","");
            return true;
        }else if (event->type() == QEvent::FocusOut && obj->property("text").toString().isEmpty()){
            obj->setProperty("text",trUtf8("بحث"));
            obj->setProperty("styleSheet","QLineEdit {font: italic;color: rgb(115, 115, 115);}");
            return true;
        }

    }
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {

    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        if(m_layouDir==true){
            QApplication::setLayoutDirection(Qt::LeftToRight);
            QLocale::setDefault(QLocale(QLocale::Latin));
        }else{
            QApplication::setLayoutDirection(Qt::RightToLeft);
            QLocale::setDefault(QLocale(QLocale::Arabic));
        }
        //  qDebug()<<m_layouDir;
        break;

    default:
        break;
    }
}
void MainWindow::updateIconEndAction()
{

    //*******************menu tool**************
    ui->menuDockTooBar->addAction(ui->dockWidget_books->toggleViewAction());
    ui->dockWidget_books->toggleViewAction()->setShortcut(QKeySequence(tr("Ctrl+Shift+b")));
    ui->dockWidget_books->toggleViewAction()->setIcon(QIcon(":/images/image/view-books.png"));
    ui->menuDockTooBar->addAction(ui->dockWidget_curaan->toggleViewAction());
    ui->dockWidget_curaan->toggleViewAction()->setShortcut(QKeySequence(tr("Ctrl+Shift+c")));
      ui->dockWidget_curaan->toggleViewAction()->setIcon(QIcon(":/images/image/view-curan.png"));
    ui->menuDockTooBar->addAction(ui->dockWidget_fahras->toggleViewAction());
    ui->dockWidget_fahras->toggleViewAction()->setShortcut(QKeySequence(tr("Ctrl+Shift+l")));
     ui->dockWidget_fahras->toggleViewAction()->setIcon(QIcon(":/images/image/view-fahrasse.png"));
    ui->menuDockTooBar->addAction(ui->dockWidget_favorite->toggleViewAction());
    ui->dockWidget_favorite->toggleViewAction()->setShortcut(QKeySequence(tr("Ctrl+Shift+v")));
         ui->dockWidget_favorite->toggleViewAction()->setIcon(QIcon(":/images/image/view-favo.png"));
    ui->menuDockTooBar->addAction(ui->dockWidget_find->toggleViewAction());
    ui->dockWidget_find->toggleViewAction()->setShortcut(QKeySequence(tr("Ctrl+Shift+f")));
     ui->dockWidget_find->toggleViewAction()->setIcon(QIcon(":/images/image/view-find.png"));
    ui->menuDockTooBar->addSeparator();
     //*******************icons******************


    //********انشاء شجرة البحوث*************
    ui->treeWidget_books->setColumnHidden(2,true);
    ui->treeWidget_books->setColumnWidth(0,200);
    ui->treeWidget_books->header()->setVisible(true);
    ui->  treeWidget_fav->setColumnWidth(0,300);
    ui->treeWidget_fav->header()->setVisible(true);
    //******اضافات الى شريط المعلومات*******

    ui->action_stopFind->setVisible(false);

    ui->action_stopFind->setIconText( trUtf8("توقيف البحث"));
    connect(ui->action_stopFind, SIGNAL(triggered()), this, SLOT(stopFind()));
    labelProgress=new QLabel;
    ui->statusBar->addWidget(labelProgress);
    progressBarFind=new QProgressBar();
    labelProgressImg=new QLabel;
    ui->statusBar->addWidget(labelProgressImg);
    ui->statusBar->addWidget(progressBarFind);

    labelProgressInfo=new QLabel;
    ui->statusBar->addWidget(labelProgressInfo);
    labelProgress->setVisible(false);
    labelProgressImg->setVisible(false);
    //  BtnStopFind->setVisible(false);
    progressBarFind->setVisible(false);
    //ui->frameEdit->setVisible(false);
    ui->widgetEditBook->setVisible(false);
    //************favotite شريط ادوات المفضلة**********
    QLineEdit *lineEditFindFavo = new QLineEdit(this) ;
    lineEditFindFavo->setMaximumWidth(200);
    ui->toolBarFavo->insertWidget(ui->actionFavSave,lineEditFindFavo);
    ui->toolBarFavo->insertSeparator(ui->actionFavSave);

    ui->horizontalLayoutFavo->addWidget(ui->toolBarFavo);
    //*******شريط ادوات شجرة نتائج البحث

    labelTextFind=new QLabel();
    ui->toolBarFind->insertWidget(ui->actionFindItemUp,labelTextFind);
    ui->toolBarFind->insertSeparator(ui->actionFindItemUp);
    ui->  horizontalLayoutFind->addWidget(ui->toolBarFind);
    //******* اضافة القوائم الى شجرة الكتب menu************
    AC_bookOpenInNewTab=new QAction(style()->standardPixmap(QStyle::SP_FileDialogNewFolder),trUtf8("فتح في لسان جديد"), this);
    AC_bookOpen = new QAction(style()->standardPixmap(QStyle::SP_DirOpenIcon),trUtf8("فتح الكتاب المحدد"), this);
    AC_bookRemove = new QAction(style()->standardPixmap(QStyle::SP_DialogCancelButton),trUtf8("حذف الكتاب المحدد"), this);
    AC_bookUpdat= new QAction(trUtf8("تحرير بيانات الكتاب المحدد"), this);
    AC_bookPath= new QAction(trUtf8("مسار الكتاب المحدد"), this);
    AC_bookRename= new QAction(trUtf8("اعادة تسمية دليل الكتاب"), this);
  //  AC_bookDownload= new QAction(trUtf8(" تحميل الكتاب المحدد"), this);
    AC_groupeRemove = new QAction(style()->standardPixmap(QStyle::SP_DialogCancelButton),trUtf8("حذف العنصر المحدد"), this);
    AC_groupeAdd = new QAction(style()->standardPixmap(QStyle::SP_FileDialogNewFolder),trUtf8("إظافة مجموعةالى القسم المحدد"), this);
    AC_categorieAdd = new QAction(style()->standardPixmap(QStyle::SP_FileDialogNewFolder),trUtf8("إظافة قسم جديد"), this);
    AC_groupeRename= new QAction(trUtf8("اعادة تسمية العنصر المحدد"), this);
    //****اضافات الى شريط الادواتت المؤثرات toolbar****
    labelAnim=new QLabel(ui->toolBarAnim);
    labelAnim->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->toolBarAnim->addSeparator();
    ui->toolBarAnim->addWidget(labelAnim);
    ui->frame_5->layout()->addWidget(ui->toolBarAnim);


    //*******شريط ادوات الرئيسي


    lineEditSearchInDoc=new QLineEdit();
    lineEditSearchInDoc->setMaximumWidth(250);
    AC_lineEditSearchInDoc=new QWidgetItem(this);
    //AC_lineEditSearchInDoc->setDefaultWidget(lineEditSearchInDoc);
    AC_GoSearchInDoc=new QAction(QIcon(":/images/image/arrow-left.png"),trUtf8("البحث في الصفحة"), this);
    AC_GoSearchInCurBook=new QAction(QIcon(":/images/image/FIN_book.png"),trUtf8("البحث في الكتاب الحالي"), this);
    //**القوائم المنسدلة للكتب السابقة combobox recent**
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
        ui->menuRecent->addAction(recentFileActs[i]);
        recentBooks->recentFileActs[i]=recentFileActs[i];
    }

    AC_recent=new QAction(ui->menuRecent->title(),this);

    AC_recent->setMenu(ui->menuRecent);
    connect(AC_recent,SIGNAL(triggered()),recentFileActs[1],SIGNAL(triggered()));
    //**********شريط ادوات التنقل بين الصفحات
  //  labelPagePart = new QLabel("");

    AC_nextHistorie= new QAction(QIcon(":/images/image/undo-next.png"),trUtf8("التنقل التالي"), this);
    AC_prevHistorie=new QAction(QIcon(":/images/image/undo-back.png"),trUtf8("التنقل السابق"), this);
    AC_prevHistorie->setEnabled(false);
    AC_nextHistorie->setEnabled(false);

    view->setTreeViw(ui->treeWidget_books);
    //view->lineEdit=ui->lineEdit;
    connect(view,SIGNAL(openBook(QString,QString,QString,int)),
            this,SLOT(open_dataBase(QString,QString,QString,int)));
//    view->label=labelAnim;
    connect(view,SIGNAL(labelInfoChanged(QString)),labelAnim,SLOT(setText(QString)));
    view->init();
    ui->horizontalLayout_7->addWidget(view);

      /////////////////عناصر شجرة الكتب
    QLineEdit *lineEditFind = new QLineEdit(this);
    ui->toolBarBooks->insertWidget(ui->actionFindBkOrHt,lineEditFind);
    ui->horizontalLayoutBook->addWidget(ui->toolBarBooks);
    //--------------عناصر الفهرسة
    QToolBar *barFahras = new QToolBar;
    barFahras->setIconSize(QSize(18,18));
    lineEditFindFah = new QLineEdit(this);
    AC_expandTreeFah = new QAction(QIcon(":/images/image/btnfolder.png"),trUtf8("توسيع الكل"), this);
    AC_colapsTreeFah = new QAction(QIcon(":/images/image/colapse.png"),trUtf8("غلق الكل"), this);
    barFahras->addWidget(lineEditFindFah);
    barFahras->addSeparator();
    barFahras->addAction(AC_expandTreeFah);
    barFahras->addAction(AC_colapsTreeFah);
    ui->horizontalLayoutFahras->addWidget(barFahras);
     //**********************************
    // tabWidget
    QToolButton *btnAddtab=new QToolButton(this);
    btnAddtab->setIcon(QIcon::fromTheme("tab-new", QIcon(":/images/image/tab-new.png")));
    btnAddtab->setStyleSheet(" QToolButton  {  border: 0px solid #8f8f91;}"
                             " QToolButton:hover  {  border: 1px solid #8f8f91; border-radius: 3px;}");
    btnAddtab->setToolTip(trUtf8("لسان جديد"));
    connect(btnAddtab, SIGNAL(clicked()), this, SLOT(tabCreatTabNew()));
    ui->tabWidget->setCornerWidget(btnAddtab,Qt::TopLeftCorner);

    //**************whatsthis**************
#ifndef QT_NO_WHATSTHIS
    AC_prevHistorie ->setWhatsThis(trUtf8("الرجوع الى التنقلات التي قمت بها سابقا وليس الى الصفحة السابقة"));
    AC_nextHistorie->setWhatsThis(trUtf8("الرجوع الى التنقلات التالية وليس الى الصفحة التالية"));
    AC_GoSearchInDoc->setWhatsThis(trUtf8("انقر على الزر للبحث في الصفحة الحالية عن النص المدخل في مربع النص"));
    AC_GoSearchInCurBook->setWhatsThis(trUtf8("انقر على الزر للبحث في الكتاب الحالي عن النص المدخل في مربع النص"));
    lineEditFindFah->setWhatsThis(trUtf8("ادخل نص البحث هنا ليتم فلترة الفهرسة كلما ادخلت حرفا جديدا"));
    lineEditFind->setWhatsThis(trUtf8("ادخل نص البحث هنا ليتم فلترة الفهرسة كلما ادخلت حرفا جديدا"));

#endif // QT_NO_WHATSTHIS
    //***********الاشارات signal end slot****
    connect(AC_bookOpen, SIGNAL(triggered()), this, SLOT(menuOpenBook()));
    connect(AC_bookOpenInNewTab, SIGNAL(triggered()), this, SLOT(tabOpenInNewTab()));
    connect(AC_bookRemove, SIGNAL(triggered()), this, SLOT(menu_BookRemove()));
    connect(AC_bookUpdat, SIGNAL(triggered()), this, SLOT(menu_BookUpdat()));
    connect(AC_bookPath, SIGNAL(triggered()), this, SLOT(menu_BookPath()));
    connect(AC_bookRename, SIGNAL(triggered()), this, SLOT(menu_renameBook()));
  //  connect(AC_bookDownload, SIGNAL(triggered()), this, SLOT(menu_downloadBook()));
    connect(AC_groupeRemove, SIGNAL(triggered()), this, SLOT(menu_GroupeRemove()));
    connect(AC_groupeAdd, SIGNAL(triggered()), this, SLOT(menu_GroupeAdd()));
    connect(AC_categorieAdd, SIGNAL(triggered()), this, SLOT(menu_CategorieAdd()));
    connect(AC_groupeRename, SIGNAL(triggered()), this, SLOT(menu_GroupeRename()));
    connect(AC_GoSearchInDoc, SIGNAL(triggered()), this, SLOT(searchIndoc()));
    connect(AC_GoSearchInCurBook, SIGNAL(triggered()), this, SLOT(findInCurrentBook()));
    connect(lineEditSearchInDoc, SIGNAL(returnPressed()), this, SLOT(searchIndoc()));
    connect(ui->textBrowser_0, SIGNAL(highlighted(const QUrl)), this, SLOT(textBookHighlighted(QUrl)));
    connect(ui->textBrowser_0, SIGNAL(anchorClicked(const QUrl)), this, SLOT(textBookAnchorClicked(QUrl)));

    connect(lineEditFind, SIGNAL(textChanged(QString)), this, SLOT(searchInTreeBook(QString)));
    connect(ui->actionBookExpAll, SIGNAL(triggered()), ui->treeWidget_books, SLOT(expandAll()));
    connect(ui->actionBookColpAll, SIGNAL(triggered()), ui->treeWidget_books, SLOT(collapseAll()));
    connect(ui->actionFindBkOrHt, SIGNAL(triggered()), this, SLOT(findBookOrHautor()));
    connect(ui->treeWidgetFind, SIGNAL(itemActivated(QTreeWidgetItem* ,int)), this, SLOT(showResultFind(QTreeWidgetItem* )));
    connect(lineEditFindFavo, SIGNAL(textChanged(QString)), this, SLOT(find_favo_textChanged(QString)));
    connect(AC_nextHistorie, SIGNAL(triggered()), this, SLOT(historieNext()));
    connect(AC_prevHistorie, SIGNAL(triggered()), this, SLOT(historiePrev()));

    //****************
 connect(ui->actionNewTab, SIGNAL(triggered()), this, SLOT(tabCreatTabNew()));
    connect(this, SIGNAL(bookAvailable(bool)),ui->action_next, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->action_prev, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->action_first, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->action_last, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->action_edit, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->action_favorit, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->actionGotoPage, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->actionOrganizeFahrasse, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->actionBookInfo, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->actionHiperlink, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),AC_GoSearchInCurBook, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->actionArchive, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(bookAvailable(bool)),ui->actionExport, SLOT(setEnabled(bool)));
    // connect(this, SIGNAL(bookAvailable(bool)),AC_GoSearchInDoc, SLOT(setEnabled(bool)));
    listLineEditName <<lineEditSearchInDoc->objectName() << ui->lineEdit_find_cur->objectName()<< ui->lineEdit_FindRowat->objectName()
                    << lineEditFindFah->objectName()<< lineEditFindFavo->objectName()<< lineEditFind->objectName();
    lineEditSearchInDoc->installEventFilter(this);
    lineEditSearchInDoc->setText(trUtf8("بحث"));
    lineEditSearchInDoc->setStyleSheet("QLineEdit {font: italic;color: rgb(115, 115, 115);}");

    ui->lineEdit_find_cur->installEventFilter(this);
    ui->lineEdit_find_cur->setText(trUtf8("بحث"));
    ui->lineEdit_find_cur->setStyleSheet("QLineEdit {font: italic;color: rgb(115, 115, 115);}");

    ui->lineEdit_FindRowat->installEventFilter(this);
    ui->lineEdit_FindRowat->setText(trUtf8("بحث"));
    ui->lineEdit_FindRowat->setStyleSheet("QLineEdit {font: italic;color: rgb(115, 115, 115);}");

    lineEditFindFah->installEventFilter(this);
    lineEditFindFah->setText(trUtf8("بحث"));
    lineEditFindFah->setStyleSheet("QLineEdit {font: italic;color: rgb(115, 115, 115);}");

    lineEditFindFavo->installEventFilter(this);
    lineEditFindFavo->setText(trUtf8("بحث"));
    lineEditFindFavo->setStyleSheet("QLineEdit {font: italic;color: rgb(115, 115, 115);}");

    lineEditFind->installEventFilter(this);
    lineEditFind->setText(trUtf8("بحث"));
    lineEditFind->setStyleSheet("QLineEdit {font: italic;color: rgb(115, 115, 115);}");
    //***********

    //-------***************icons--------------------
    m_icondfile.addPixmap (QPixmap(style()->standardPixmap(QStyle::SP_FileIcon)), QIcon::Normal, QIcon::Off);
    m_icondir.addPixmap(QPixmap(style()->standardPixmap(QStyle::SP_DirIcon)), QIcon::Normal, QIcon::Off);
    m_iconGroup .addPixmap(QPixmap(QString::fromUtf8(":/images/image/groupb.png")), QIcon::Normal, QIcon::Off);


}
void MainWindow::setupActionsPdf()
{
  ui->action_next->setEnabled(true);
  ui->action_prev->setEnabled(true);
    ui->action_first->setEnabled(true);
    ui->action_last->setEnabled(true);
      ui->action_favorit->setEnabled(true);
      ui->actionGotoPage->setEnabled(true);
          ui->actionBookInfo->setEnabled(true);

   // connect(this, SIGNAL(bookAvailable(bool)),AC_GoSearchInCurBook, SLOT(setEnabled(bool)));

}

void MainWindow::chargeIconsThemes()
{
    //ui->pushButtonEditClose->setIcon(QIcon::fromTheme("edit-delete",QIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton))));
    AC_nextHistorie->setIcon( QIcon::fromTheme("edit-undo", QIcon(":/images/image/undo-back.png")));
    AC_prevHistorie->setIcon( QIcon::fromTheme("edit-redo", QIcon(":/images/image/undo-next.png")));
    AC_GoSearchInDoc->setIcon(QIcon::fromTheme("dialog-ok", QIcon(":/images/image/arrow-left.png")));
    AC_groupeRename->setIcon(QIcon::fromTheme("edit-rename", QIcon(":/images/image/edit-rename.png")));
    AC_GoSearchInCurBook->setIcon(QIcon::fromTheme("edit-find", QIcon(":/images/image/FIN_book.png")));
    AC_expandTreeFah->setIcon(QIcon::fromTheme("view-sort-ascending", QIcon(":/images/image/btnfolder.png")));
    AC_colapsTreeFah->setIcon(QIcon::fromTheme("view-sort-descending", QIcon(":/images/image/colapse.png")));
    ui->actionBookExpAll->setIcon(QIcon::fromTheme("view-sort-ascending", QIcon(":/images/image/btnfolder.png")));
    ui->actionBookColpAll->setIcon(QIcon::fromTheme("view-sort-descending", QIcon(":/images/image/colapse.png")));
    ui->action_fulscreen->setIcon(QIcon::fromTheme("view-fullscreen", QIcon(":/images/image/view-fullscreen.png")));
    ui->actionOption->setIcon(QIcon::fromTheme("document-properties", QIcon(":/images/image/config-background.png")));
    ui->actionHiperlink->setIcon(QIcon::fromTheme("insert-link", QIcon(":/images/image/insert-link.png")));
    ui->action_favorit->setIcon(QIcon::fromTheme("bookmark-new", QIcon(":/images/image/bookmark-new.png")));
    ui->actionNewTab->setIcon(QIcon::fromTheme("tab-new", QIcon(":/images/image/tab-new.png")));
    ui->action_edit->setIcon(QIcon::fromTheme("document-edit", QIcon(":/images/image/bookEdit.png")));
    ui->menuRecent->setIcon(QIcon::fromTheme("document-open-recent", QIcon(":/images/image/recent.png")));
    AC_recent->setIcon(QIcon::fromTheme("document-open-recent", QIcon(":/images/image/recent.png")));

    if(this->layoutDirection()==Qt::RightToLeft){
        ui->action_first->setIcon(QIcon::fromTheme("go-last", QIcon(":/images/image/go-first.png")));
        ui->action_prev->setIcon(QIcon::fromTheme("go-next", QIcon(":/images/image/go-back.png")));
        ui->action_next->setIcon(QIcon::fromTheme("go-previous", QIcon(":/images/image/go-next.png")));
        ui->action_last->setIcon(QIcon::fromTheme("go-first", QIcon(":/images/image/go-last.png")));

    }else{
        ui->action_first->setIcon(QIcon::fromTheme("go-first", QIcon(":/images/image/go-last.png")));
        ui->action_prev->setIcon(QIcon::fromTheme("go-previous", QIcon(":/images/image/go-next.png")));
        ui->action_next->setIcon(QIcon::fromTheme("go-next", QIcon(":/images/image/go-back.png")));
        ui->action_last->setIcon(QIcon::fromTheme("go-last", QIcon(":/images/image/go-first.png")));

    }

    ui->actionGotoPage->setIcon(QIcon::fromTheme("go-top", QIcon(":/images/image/go-page.png")));
    ui->action_import->setIcon(QIcon::fromTheme("stock_new-text", QIcon(":/images/image/address-book-new.png")));
    ui->actionMdb->setIcon(QIcon::fromTheme("stock_new-dir", QIcon(":/images/image/address-book-new.png")));
    ui->action_addBoook->setIcon(QIcon::fromTheme("stock_new-dir", QIcon(":/images/image/address-book-new.png")));
    ui->actionArchive->setIcon(QIcon::fromTheme("add-folder-to-archive", QIcon(":/images/image/tar-gz.png")));
    ui->actionExport->setIcon(QIcon::fromTheme("document-export"));
    ui->actionPrint->setIcon(QIcon::fromTheme("document-print"));
    ui->actionPrintPreview->setIcon(QIcon::fromTheme("gtk-print-preview"));
    ui->actionZoumIn->setIcon(QIcon::fromTheme("zoom-in"));
    ui->actionZoomOut->setIcon(QIcon::fromTheme("zoom-out"));
 //   ui->btnGoFindFah->setIcon(QIcon::fromTheme("dialog-ok", QIcon(":/images/image/arrow-left.png")));
      ui->actionBookInfo->setIcon(QIcon::fromTheme("documentinfo",QIcon(style()->standardPixmap(QStyle::SP_MessageBoxInformation))));
//--------------------------------
      ui->btnRowaInfo->setIcon(QIcon::fromTheme("documentinfo",QIcon(style()->standardPixmap(QStyle::SP_MessageBoxInformation))));

      ui->actionFavItemDown->setIcon(QIcon::fromTheme("stock_down",QIcon(style()->standardPixmap(QStyle::SP_ArrowDown))));
      ui->actionFavItemUp->setIcon(QIcon::fromTheme("stock_up",QIcon(style()->standardPixmap(QStyle::SP_ArrowUp))));
      ui->actionFindItemDown->setIcon(QIcon::fromTheme("stock_down",QIcon(style()->standardPixmap(QStyle::SP_ArrowDown))));
      ui->actionFindItemUp->setIcon(QIcon::fromTheme("stock_up",QIcon(style()->standardPixmap(QStyle::SP_ArrowUp))));
      ui->actionFindLoad->setIcon(QIcon::fromTheme("document-open",QIcon(style()->standardPixmap(QStyle::SP_DirOpenIcon))));
      ui->action_stopFind->setIcon(QIcon::fromTheme("media-playback-stop",QIcon(style()->standardPixmap(QStyle::SP_MediaStop))));


    //  ui->actionFavItemDown->setIcon(style()->standardPixmap(QStyle::SP_ArrowDown));
   //   ui->actionFavItemUp->setIcon(style()->standardPixmap(QStyle::SP_ArrowUp));
     //ui->actionFindItemDown->setIcon(style()->standardPixmap(QStyle::SP_ArrowDown));
    //  ui->actionFindItemUp->setIcon(style()->standardPixmap(QStyle::SP_ArrowUp));
     // ui->actionFindLoad->setIcon(style()->standardPixmap(QStyle::SP_DirOpenIcon));

  //ui->action_stopFind->setIcon(style()->standardPixmap(QStyle::SP_MediaStop));

}

void MainWindow::addToolRubonBar(bool rubon)
{

    this->addToolBar(Qt::TopToolBarArea,toolRubon);
    toolRubon->setToolIconSize(m_toolIconSize,m_textUnderIcon);
    if(rubon==true){
        foreach (QAction *act,  toolRubon->actions()) {
            toolRubon->removeAction(act);
        }


      //    if(lineEditSearchInDoc->isVisible())
       //       lineEditSearchInDoc->setVisible(false);

        int count=  m_listActToAdd.count();
        for(int i=0;i<count;i++){
            int indx=m_listActToAdd.at(i).toInt();
            if(indx==1000){
                toolRubon->addSeparator();
            }else if (indx==2000){
                toolRubon->addSeparator();
                //blm tau hrs bagaimana mazbrili
				//toolRubon->addAction(AC_lineEditSearchInDoc);

                //toolRubon->addAction(AC_GoSearchInDoc);
                //toolRubon->addAction(AC_GoSearchInCurBook);
              // toolRubon->addAction(ui->actionFind_all);

             //   toolRubon->insertWidget(AC_GoSearchInDoc,lineEditSearchInDoc);

              //  lineEditSearchInDoc->setVisible(true);
            //   lineEditSearchInDoc->show();
            }else{
                toolRubon->addAction(m_listActions.at(indx));
            }
        }

        return;
    }

    toolRubon->setRubonStyle();

    toolRubon->toolFile->addActions(ui->menu_file->actions());
    //  toolRubon->toolView->addActions(ui->menuView->actions());
    QWidget *widget=new QWidget;
    QHBoxLayout *horizontalLayoutTool = new QHBoxLayout(widget);
    QSpacerItem *horizontalSpacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayoutTool->addItem(horizontalSpacer);
    toolRubon->toolFile->insertWidget(ui->action_close,widget);

    ui->action_last->setIcon(QIcon(":/images/image/go-first.png"));

    toolRubon->toolEdit->addActions(ui->menuEdit->actions());

    toolRubon->toolEdit->addSeparator();
    toolRubon->toolEdit->addAction(AC_prevHistorie);
    toolRubon->toolEdit->addAction(AC_nextHistorie);
    toolRubon->toolEdit->addSeparator();
    toolRubon->toolEdit->addAction(ui->actionZoumIn);
    toolRubon->toolEdit->addAction(ui->actionZoomOut);
    toolRubon->toolEdit->removeAction(ui->actionFind_all);

    toolRubon->tooTools->addAction(ui->actionOption);
    toolRubon->tooTools->addAction(ui->actionControle);
    toolRubon->tooTools->addSeparator();
    toolRubon->tooTools->addActions(ui->menuTools->actions());

    toolRubon->toolFind->addAction(ui->actionFind_all);
    toolRubon->toolFind->addSeparator();
	//remark by mazbrili because still confuse
    //toolRubon->toolFind->addAction(AC_lineEditSearchInDoc);
   // toolRubon->toolFind->addAction(AC_GoSearchInDoc);
    toolRubon->toolFind->addSeparator();
    //toolRubon->toolFind->addAction(AC_GoSearchInCurBook);
    toolRubon->toolView->addAction(ui->action_fulscreen);
    toolRubon->toolView->addSeparator();
    toolRubon->toolView->addActions(ui->menuDockTooBar->actions());

    toolRubon->toolHelp->addActions(ui->menuHelp->actions());


}



bool MainWindow::loadPlugin(int plug)
{
    /*
    QDir pluginsDir(m_pathApp);

#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    if (!QFile::exists(m_pathApp+"/plugins")){
        if(QFile::exists("/usr/lib/elkirtasse")){
            pluginsDir.setPath("/usr/lib/elkirtasse");
        }else if(QFile::exists("/usr/lib64/elkirtasse")){
            pluginsDir.setPath("/usr/lib64/elkirtasse");
        }
    }
    */
QDir pluginsDir(Utils::getPluginDir());
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();


        if (plugin) {
            switch (plug) {
            case NET_PLUG:
                netInterface = qobject_cast<NetInterface *>(plugin);
                if (netInterface){ return true; }
                break;
            case SHAMILA_PLUG:
               shamilaInterface = qobject_cast<ShamilaInterface *>(plugin);
                if (shamilaInterface){ return true; }
                break;
            case ROWAT_PLUG:
                rowatInterface    = qobject_cast<RowatInterface *>(plugin);
                if (rowatInterface){ return true; }
                break;
            default:
                break;
            }


        }
    }
    return false;
}


//open book****فتح الكتب*****************
void MainWindow:: open_dataBase(const QString &name, const QString &title, const QString &Autor, int position)       //فتح الكتاب
{ 

    //اذا كان نفس الكتاب مفتوحا
    int  tabcount=m_listTabBook.count();

    for(int r=0;r<tabcount;++r){
        TabBook *booktab=m_listTabBook.at(r);
        if (booktab->getBookName()==name){

            ui->tabWidget->setCurrentIndex(r);
            return;
        }
    }
    //حفظ الكتاب السابق
    if (curentBook){
        if(!curentBook->getBookName().isEmpty())
            recentBooks->recentChange(curentBook->getBookName(),
                                      curentBook->getTitle(),
                                      curentBook->getAuthor(),
                                      QString::number(curentBook->getBookPos()),
                                      m_recentNbr,
                                      curentBook->getBookIsTefsir());
        recentBooks->recentCharge();
    }
    if (ui->tabWidget->currentWidget()==ui->tabIntro)
        tabCreatTabNew();

    if(m_listTabBook.count()>0&&ui->tabWidget->currentWidget()!=ui->tabIntro)
        curentBook=m_listTabBook.at(ui->tabWidget->currentIndex());
    //الفتح
    int open=curentBook->openDataBase(name,title,Autor,position);

    switch (open) {

    case TabBook::DOC_FORMAT_NOAVAILABLE:
        QMessageBox::information(this,trUtf8("خطأ"),name + trUtf8("التنسيق غير صحيح ") );
        return;

    case TabBook::PDF_PLUG_NOEXIST:
        QMessageBox::information(this,"",trUtf8("تاكد من وجود الاصافة لقتح ملفات بي ديف"));
        return;

    case TabBook::DOC_FILE_NOEXIST:
        if(name.endsWith("pdf")){
            QMessageBox::information(this,"",trUtf8("تاكد من وجود الكتاب في احد مسارات الكتب"));
        }else{
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText(trUtf8( "  الكتب غير موجود في هذا الدليل \n ")+m_pathCostm + "/" + name);
            msgBox.setInformativeText(trUtf8( "هل تريد تحميله من الانترنت ؟  "));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int ret = msgBox.exec();
            switch (ret) {

            case QMessageBox::Yes:
                menu_downloadBook(name);
                break;
            case QMessageBox::Cancel:
                // Cancel was clicked
                break;
            default:
                // should never be reached
                break;
            }

        }
        return;

    case TabBook::DOC_CANCELED:
        return;

    default:

        break;


    }
    /*
    if(curentBook->openDataBase(name,title,Autor,position)==false){
        qDebug()<<"no open"<<name;
        if(name.endsWith("pdf")){
                     return;
        }
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(trUtf8( "  الكتب غير موجود في هذا الدليل \n ")+m_pathCostm + "/" + name);
        msgBox.setInformativeText(trUtf8( "هل تريد تحميله من الانترنت ؟  "));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {

        case QMessageBox::Yes:
            menu_downloadBook(name);
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
        return;
    }
    */
    //ايقونة اللسان
    if(curentBook->getIsPdf())
        ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(),
                                  QIcon::fromTheme("application-pdf",QIcon(":/images/image/application-pdf")));
    else
        ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(),
                                  QIcon(":/images/image/booki.png"));

    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),curentBook->getTitle());
    this->tabifyDockWidget(ui->dockWidget_books,ui->dockWidget_fahras);

}

//menufile----------------------------
void MainWindow::on_action_import_triggered()//استراد ملفات نصية
{
    dialogconvert *dlgc=new dialogconvert(this);
    dlgc->m_pathCostum=m_pathCostm;
    if ( dlgc->exec() == QDialog::Accepted ){
        chargeGroupe();
    }
    delete dlgc;
}

void MainWindow::on_actionMdb_triggered()
{

#ifdef   Q_OS_WIN
    Dialogmdb *dlg=new Dialogmdb(this);
    dlg->m_path=m_pathCostm;
    if ( dlg->exec() == QDialog::Accepted ){
        chargeGroupe();
    }
#else
    mdbexport *dlg=new mdbexport(this);
    dlg->m_path=m_pathCostm;
    if ( dlg->exec() == QDialog::Accepted ){
        chargeGroupe();
    }
#endif
    delete dlg;
}

void MainWindow::on_action_addBoook_triggered()//اظافة كتاب
{
    DialogAddBooks* dlg=new DialogAddBooks(this);
    dlg->m_pathCostm=m_pathCostm;
    if ( dlg->exec() == QDialog::Accepted ){
        chargeGroupe();
    }
    delete dlg;
}

// تصدير الكتاب
void MainWindow::on_actionExport_triggered()//
{
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty())   return;
    Print* dlg=new Print(this);
    dlg->treeviw=curentBook->treeViewFahras;
    dlg->setpathApp(m_pathApp);
    dlg->setBookCurentPath(curentBook->getBookPath());
    dlg-> setBookTitle(curentBook->getTitle());
    dlg-> setBookAuthor(curentBook->getAuthor());
    dlg->setBookBetaka(curentBook->getBetaka());
    dlg->exec();
    delete dlg;
}
// ضغط الكتاب
void MainWindow::on_actionArchive_triggered()
{
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty())   return;

    QString fileName=curentBook->getBookPath();
    QDir dirF(fileName);
    if (!dirF.exists()){
        QMessageBox::information(this,trUtf8("معلومات"),trUtf8("لا يوجد كتاب لارشفته"));
        return ;
    }
     QString dirName=dirF.dirName();
    dirF.cdUp();
    QString dirParent=dirF.absolutePath();

//    dirParent.replace(dirName,QString::null);
//    dirParent.replace("//",QString::null);



    QString title=curentBook->getTitle();
    title.replace(" ","_");
    QProcess prosses;
    QProgressDialog progress(trUtf8("الرجاء الانتظار...."), trUtf8("الغاء"), 0, 0, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.setEnabled(false);
    progress.show();
    qApp->processEvents();
#ifdef   Q_OS_WIN
    prosses.setWorkingDirectory(QApplication::applicationDirPath());
    QString setTar="\""+ QDir::homePath()+"/"+title+".tar\"";
    QString setGz="\""+ QDir::homePath()+"/"+title+".tar.gz\"";

    prosses.start("7z a "+setTar+" \""+ dirParent+dirName+"/\"");
    while(prosses.state()==QProcess::Running){
        qApp->processEvents();
        progress.setValue(progress.value()+1);
    }
    prosses.start("7z a "+setGz+" "+ setTar);
    while(prosses.state()==QProcess::Running){
        qApp->processEvents();
        progress.setValue(progress.value()+1);
    }
    QDir mydir(QDir::homePath());
    mydir.remove(title+".tar");
#else
    QDir::setCurrent(dirParent);
    prosses.execute("tar -cvzf "+ QDir::homePath()+"/"+title+".tar.gz "+ dirName);
    while(prosses.state()==QProcess::Running){
        qApp->processEvents();
        progress.setValue(progress.value()+1);
    }
#endif
    QMessageBox::information(this,trUtf8("معلومات"),trUtf8("لقد تم انشاء الارشيف بنجاح")+"\n"+QDir::homePath()+"/"+title+".tar.gz");
}

// معلومات الكتاب
void MainWindow::on_actionBookInfo_triggered()
{
   // TabBook *tab=m_listTabBook.at(0);
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty()) return;


    QString imgScreenPath= curentBook->getBookPath()+"/screenshot.png";
    if(!QFile::exists(imgScreenPath)){
        imgScreenPath=":/images/image/groupbook.png";
    }

    if(curentBook->getBookName().endsWith("pdf")){
        imgScreenPath= curentBook->getBookPath().replace(".pdf",".png");
        if(!QFile::exists(imgScreenPath)){
            imgScreenPath=":/images/image/application-pdf";
        }
    }else{
        imgScreenPath= curentBook->getBookPath()+"/screenshot.png";
        if(!QFile::exists(imgScreenPath)){
            imgScreenPath=":/images/image/groupbook.png";
        }
    }
    QPixmap   pixmapG(imgScreenPath) ;
    QMessageBox msgBox(this);
    msgBox.setLayoutDirection(Qt::RightToLeft);
    msgBox.setIconPixmap(pixmapG);
    msgBox.setWindowTitle(trUtf8("معلومات عن الكتاب"));
    QString tit=" <span style=' font-size:14pt; color:red;'>"+curentBook->getTitle()+"</span>";
    msgBox.setText(tit);
    msgBox.setInformativeText(trUtf8("المؤلف : ")+curentBook->getAuthor()+"\n"+curentBook->getBetaka()
                              +trUtf8("\n المسار : ")+curentBook->getBookPath() );
    msgBox.exec();


}
// طباعة الصفحة
void MainWindow::on_actionPrint_triggered()
{


#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    QTextBrowser *textBrowser;

    if(curentBook){
        if(curentBook->getIsPdf())
            return;
         textBrowser=curentBook->txtBrowserBook;
     }else{
         textBrowser=ui->textBrowser_0;
     }
    if (textBrowser->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
        dlg->setWindowTitle(tr("Print Document"));

     if (dlg->exec() == QDialog::Accepted) {
        textBrowser->print(&printer);

    }
    delete dlg;
#endif
}

void MainWindow::on_actionPrintPreview_triggered()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
     preview.setWindowState(Qt::WindowMaximized);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
#endif
}
void MainWindow::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    if(curentBook){
        if(curentBook->getIsPdf())
            return;
        curentBook->txtBrowserBook->print(printer);
    }else{
         ui->textBrowser_0->print(printer);
    }

#endif
}

void MainWindow::on_action_close_triggered()
{
    // qApp->quit();
    if(this->close()==true)
        qApp->quit();
}

//menuEdit----------------------------

void MainWindow::on_action_first_triggered()//السجل الاول
{
      if(curentBook) curentBook->firstPage();

}
void MainWindow::on_action_next_triggered()//السجل التالي
{
     if(curentBook) curentBook->nextPage();

}
void MainWindow::on_action_prev_triggered()//السجل السابق
{
   if(curentBook) curentBook->prevPage();

}
void MainWindow::on_action_last_triggered()//السجل الاخير
{
   if(curentBook)  curentBook->lastPage();

 }

void MainWindow::historieNext()
{
    if(curentBook)   curentBook->historieNext();


}

void MainWindow::historiePrev()
{
    if(curentBook)curentBook->historiePrev();


}

void MainWindow::on_actionGotoPage_triggered()
{
    if(curentBook) curentBook->gotoPageDlg();


}
// تنفيذ البحث في مجموعة الكتب
void MainWindow::on_actionFind_all_triggered()  //
{
    if (Findbook->isFindRuned==true){ return;}
    Dialogfind* dlg=new Dialogfind(this);
    dlg->bookPath=m_pathCostm;
    if ( dlg->exec() == QDialog::Accepted ){
        m_textfind=dlg->findString;
        if ( m_textfind.length()<3){ return;}

        showfind(false);

        labelTextFind->setText(trUtf8("نص البحث  : ")+m_textfind);
        labelProgress->setVisible(true);
        labelProgressImg->setVisible(true);
        //  BtnStopFind->setVisible(true);
        ui->action_stopFind->setVisible(true);
        Findbook->pathCostum=m_pathCostm;
        Findbook->findFirst=dlg->findFirst;
        Findbook->findToList=dlg->findList;
        Findbook->findMultiFind=dlg->MultiFind;
        Findbook->findAnd=dlg->findAnd;
        Findbook->findSawabik=dlg->findSawabik;
        Findbook->findText= m_textfind;
        Findbook->resultCount=0;
        Findbook->progressBar1=  progressBarFind;
        //  Findbook->pushButtonStop=BtnStopFind;
        Findbook->buttonStop= ui->action_stopFind;
        Findbook->treeView=ui->treeWidgetFind;
        Findbook->labelProgress=labelProgress;
        Findbook->label_progressImg=labelProgressImg;
        Findbook->isNassToFind=dlg->findIsNass;
       Findbook->noHamza=dlg->noHamza;
        this->isNassToFind=dlg->findIsNass;
        //m_isRawiToFind=false;
        ui->treeWidgetFind->clear();
        Findbook->findInAllBook(dlg->findIsNass);


    }
    delete dlg;
}

// المفضلة
void MainWindow::on_action_favorit_triggered()
{
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty()) return;

    dialogfavo *dlg=new dialogfavo(this);
    dlg->iconFolder=m_icondir;
    dlg->iconFile=m_icondfile;
    dlg->view=ui->treeWidget_fav;
    dlg->bookTitle=curentBook->getTitle();
    dlg->bookName=curentBook->getBookName();
    dlg->bookAut=curentBook->getAuthor();

    dlg->bookId=QString::number(curentBook->getBookPos());
    dlg->charge_combo();
    dlg->exec() ;
    delete dlg;
}
// ارتباط مغ كتاب او صفحة
void MainWindow::on_actionHiperlink_triggered()
{
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty()) return;

    if (curentBook->txtBrowserBook->textCursor().selectedText().isEmpty()){ return;}
    QString oldtxt=curentBook->txtBrowserBook->textCursor().selectedText();

  ClassWizard *wizard=new ClassWizard(this);
    wizard->setWizardStyle(QWizard::ModernStyle);
    if (wizard->exec()==QDialog::Accepted) {
        // QMessageBox::information(this,"", Wizard->urlTexte);
        QString urltext="<a href='" +wizard->urlTexte+"'>"+oldtxt+"</a>";
        curentBook->txtBrowserBook->textCursor().insertHtml(urltext);
        curentBook->updatPage(oldtxt,urltext,false);
         curentBook->txtBrowserBook->document()->setModified(false);
    }
    delete wizard;
}
// ترتيب الفهرسة
void MainWindow::on_actionOrganizeFahrasse_triggered()
{
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty()) return;

   curentBook->organizFahras();

}

void MainWindow::on_actionUpdatGroupRemove_triggered()
{
    Utils::treeUpdateGroupe(ui->treeWidget_books,true,m_pathCostm);
    m_treeGroupIsModified=true;
}

void MainWindow::on_actionUpdatGroup_triggered()
{
      Utils::treeUpdateGroupe(ui->treeWidget_books,true,m_pathCostm);
}


// ملأ الشاشة
void MainWindow::on_action_fulscreen_triggered()//
{
    if (this->windowState()==Qt::WindowFullScreen){
        ui->statusBar->setVisible(true);
        ui->menuBar->setParent(this);
        this->setMenuBar(ui->menuBar);
        ui->menuBar->setStyleSheet("");
        ui->menuBar->setVisible(m_isStandardToolBar);

        loadLayout();

        this->showNormal();
    } else {
        saveLayou();
        ui->dockWidget_books->setVisible(false);
        ui->dockWidget_curaan->setVisible(false);
        ui->dockWidget_fahras->setVisible(false);
        ui->dockWidget_favorite->setVisible(false);
        ui->dockWidget_find->setVisible(false);
        toolRubon->setVisible(false);
      //  ui->mainToolBar->setVisible(false);
        ui->statusBar->setVisible(false);

        ui->menuBar->setParent(ui->tabWidget);
        ui->menuBar->setVisible(true);
        ui->menuBar->setStyleSheet("QMenuBar {background-color: rgba(255, 255, 255, 0);}");
        ui->menuBar->setGeometry(0,0,23,23);

        this->showFullScreen();
    }
}
// الخيارات
void MainWindow::on_actionOption_triggered()
{
    Dialogoption *dlgop=new Dialogoption(this);

    if ( dlgop->exec() == QDialog::Accepted ){

        loadSettings();
     //   saveLayou();
        emit settingsChanged();

//        if (m_iconsThemes==true){
//            chargeIconsThemes();
//        }
    }

    delete dlgop;
}

// نزع التشكيل
void MainWindow::on_actionRemoveTechkil_triggered(bool checked)
{
    if(!curentBook)return;
    if (curentBook->getBookName().isEmpty()) return;
    curentBook->removeTechkil(checked);
}

void MainWindow::setupPageIntro()
{
    if( ui->tabIntro->isHidden())  {

        ui->tabWidget->insertTab(0,ui->tabIntro,trUtf8("مكتبة القرطاس"));
        ui->stackedWidget->insertWidget(0,ui->pageIntro);
        m_listTabBook.insert(0,BookIntro);
        ui->tabWidget->setCurrentWidget(ui->tabIntro);
    }
    ui->stackedWidget_intro->setCurrentWidget(ui->page_anim);
    labelAnim->setText(trUtf8(""));
    view->chargeEnrto();
}
// تكبير النص
void MainWindow::on_actionZoumIn_triggered()
{
    if(!curentBook)return;

    curentBook->zoomIn();
}
// تصغير النص
void MainWindow::on_actionZoomOut_triggered()
{
    if(!curentBook)return;

      curentBook->zoomOut();
}

// حول البرنامج-----------------------------------
void MainWindow::on_action_about_triggered()//حول البرنامج
{
    About *dlg=new About(this);

    if ( dlg->exec() == QDialog::Accepted|| QDialog::Rejected){
    }
    delete dlg;
}
// التغليمات -----------------------------------
void MainWindow::on_actionHelp_triggered()
{
    if( ui->tabIntro->isHidden())  {

        ui->tabWidget->insertTab(0,ui->tabIntro,trUtf8("مكتبة القرطاس"));
        ui->stackedWidget->insertWidget(0,ui->pageIntro);
        m_listTabBook.insert(0,BookIntro);
    }
    ui->stackedWidget_intro->setCurrentWidget(ui->page_text);
    ui->treeWidget_rowatFind->setVisible(false);

    ui->tabWidget->setCurrentWidget(ui->tabIntro);
    labelAnim->setText(trUtf8("التعليمات"));
    QFile file(m_pathApp+"/data/help.html");
    if (!file.exists()){
        ui->textBrowser_0->setText(trUtf8("ملف التعليمات  : ")+m_pathApp+"/data/help.html\n"+trUtf8("غير موجود"));
        return;
    }
    if (file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        QTextCodec *codec = Qt::codecForHtml(data);
        QString str = codec->toUnicode(data);
        str.replace("<img src=\"","<img src=\""+m_pathApp+"/data/");
        if (Qt::mightBeRichText(str)) {
            ui->textBrowser_0->setHtml(str);
            ui->textBrowser_0->setOpenLinks(true);
        }
        file.close();
    }
}

void MainWindow::on_actionWhatsThis_triggered()
{
	QWhatsThis::enterWhatsThisMode();
}// زر ما هذا ؟-----------------------------------

// تغير التحديد في شجرة الكتب-----------------------------------
void MainWindow::on_treeWidget_books_itemSelectionChanged()//تحديد عنصر الكتاب في الشجرة
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    if (!item->data(1,1).isNull()){
        QString imgScreenPath;

        if(item->data(1,1).toString().endsWith("pdf")){
            imgScreenPath= m_pathCostm + "/" +item->data(1,1).toString().replace(".pdf",".png");
            if(!QFile::exists(imgScreenPath)){
                imgScreenPath=":/images/image/application-pdf";
            }
        }else{
            imgScreenPath= m_pathCostm + "/" +item->data(1,1).toString()+"/screenshot.png";
            if(!QFile::exists(imgScreenPath)){
                imgScreenPath=":/images/image/booki.png";
            }
        }
        QString imgScreen=QString("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src= %1 /></p>").arg(imgScreenPath);

        QString   txtBookInfo=imgScreen
                +  trUtf8("الكتاب :") +  item->text(0) +"<br>"
                + trUtf8("المؤلف :") +  item->text(1) +"<br>"
                +   item->text(2);

        ui->textEditInfoBook->setText(txtBookInfo);
        //   startAnimationBitaka();
    }else{

        QString txtGroupInfo=  "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"+item->text(0)+"</p>"
                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/images/image/groupbook.png\" /></p>";
        ui->textEditInfoBook->setText(txtGroupInfo);
        //   labelBetaka->setPixmap(QPixmap(QString::fromUtf8(":/images/image/groupbook.png")));

        //           startAnimationPixmap();
    }
}
// تفعيل التحديد في شجرة الكتب-----------------------------------
void MainWindow::on_treeWidget_books_itemActivated(QTreeWidgetItem* item)//
{
    QString x = item->data(1,1).toString();
    if (!x.isEmpty())
    {
        open_dataBase(x,item->text(0),item->text(1),0);
     }
}
// قائمة منسدلة فرعية لشجرة الكتب--------------------------------
void MainWindow::on_treeWidget_books_customContextMenuRequested()//
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    if (!item->data(1,1).isNull()){//اذا كان كتابا
        QMenu menu(this);

        menu.addAction(AC_bookOpen);
        menu.addAction(AC_bookOpenInNewTab);
        menu.addAction(AC_bookUpdat);
        menu.addSeparator();
        menu.addAction(AC_bookRemove);
        menu.addSeparator();
        menu.addAction(AC_bookPath);
        menu.addAction(AC_bookRename);
      //  menu.addAction(AC_bookDownload);

        menu.exec(QCursor::pos());
    }else {                          //اذا كان مجموعة
        QMenu menu(this);

        menu.addAction(AC_categorieAdd);
        menu.addAction(AC_groupeAdd);
        menu.addAction(AC_groupeRename);
        menu.addSeparator();
        menu.addAction(AC_groupeRemove);
        menu.exec(QCursor::pos());
    }
}
// النقر على قائمة فتح كتاب-----------------------------------
void MainWindow::menuOpenBook()//
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    if (!item->data(1,1).isNull()){
        open_dataBase(item->data(1,1).toString(),item->text(0),item->text(1),0);
    }
}
// حذف الكتاب--------------------------------------------------
void MainWindow::menu_BookRemove()//
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    if (!item->data(1,1).isNull()){
        QString myiddata=item->data(1,1).toString();
        QMessageBox msgBox;
        msgBox.setText(trUtf8("هل تريد حذف : ") + item->text(0)+trUtf8(" ؟"));
        msgBox.setInformativeText(trUtf8("انقر على نعم لحذفه من المكتبة فقط \n")+trUtf8("او نعم للكل لحذفه من المكتبة ومن جهازك"));
        msgBox.setDetailedText(trUtf8("اذا نقرت على نعم للكل سيتم حذف الكتاب من الدليل التالي ومن المكتبة\n")+m_pathCostm+myiddata);
        msgBox.setIcon(QMessageBox::Question);
        //   msgBox.setLayoutDirection(Qt::RightToLeft);
        msgBox.setWindowTitle(trUtf8("تأكيد الحذف"));
        msgBox.setStandardButtons(QMessageBox::YesAll | QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::YesAll);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::YesAll:
            Utils::treeMenuRemoveBook(myiddata,true,m_pathCostm);
            chargeGroupe();
            break;
        case QMessageBox::Yes:
             Utils::treeMenuRemoveBook(myiddata,false,m_pathCostm);
            chargeGroupe();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}
// تغيير بيانات الكتاب---------------------------------------------
void MainWindow::menu_BookUpdat()  //
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    dialogupdat *dlgc=new dialogupdat(this);
    dlgc->creatNewBook="update";
    dlgc->m_pathCostum=m_pathCostm;
    dlgc->BookName=item->data(1,1).toString();
    dlgc->addGroupId=item->parent()->data(2,1).toString();
    dlgc->bookTitle=item->text(0);
    dlgc->bookAutor=item->text(1);
    dlgc->bookBetaka=item->text(2);


    dlgc->recharge();
    if ( dlgc->exec() == QDialog::Accepted ){
        chargeGroupe();
    }
    delete dlgc;
}
// معرفة مسار الكتاب----------------------------------------
void MainWindow::menu_BookPath()//
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    if (!item->data(1,1).isNull()){
        QString name=item->data(1,1).toString();
        QString path;
        QString idparent=name.section("_",0,0);
        idparent=idparent.remove("bk");
        QFile file;
        if(file.exists(m_pathCostm + "/" + name)){
            path=m_pathCostm + "/" + name;
        }else if(file.exists(m_pathCostm + "/"+idparent+ "/"  + name)){
            path=m_pathCostm + "/"+idparent+ "/"  + name;
        }else if(file.exists(m_pathApp + "/books/" + name)){
            path=m_pathApp + "/books/" + name;
        }else{
            path=name + trUtf8(" : غير موجود في الجهاز");
        }
        QMessageBox::information(this,trUtf8("مسار الكتاب"),path);
    }
}

// اعادة تسمية الكتاب  ----------------------------------------
void MainWindow:: menu_renameBook()
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    QString name=item->data(1,1).toString();
    QString path;
    QFile file;
    if(file.exists(m_pathCostm + "/" + name)){
        path=m_pathCostm;
    }else if(file.exists(m_pathApp + "/books/" + name)){
        path=m_pathApp + "/books/";
    }

    QDir dir(path);
    QInputDialog *dff=new QInputDialog(this);
    dff->setOkButtonText(trUtf8("موافق"));
    //    dff->setLayoutDirection(Qt::RightToLeft);
    dff->setCancelButtonText(trUtf8("الغاء"));
    dff->setLabelText(trUtf8("الاسم القديم : ")+name+"\n"+trUtf8("يحبذ ان لا يحتوي الاسم على فراغات"));
    dff->setWindowTitle(trUtf8("اعادة تسمية دليل الكتاب"));
    dff->setTextValue(item->text(0).replace(" ","_"));
    if (dff->exec()==QInputDialog::Accepted ){
        if (dff->textValue().isEmpty()){
            return;
        }
        if(file.exists(path + "/" + dff->textValue())){
            QMessageBox::critical(this,trUtf8("خطأ"),trUtf8("يوجد مجلد بنفس الاسم"));
            return;
        }
        if(dir.rename(name,dff->textValue())==true){
            item->setData(1,1,dff->textValue());
            Utils::treeSaveGroupe(ui->treeWidget_books);

            QMessageBox::information(this,trUtf8("معلومات"),trUtf8("تمت عملية اعادة تسمية دليل الكتاب بنجاح"));
        }
    }
}

void MainWindow::menu_CategorieAdd()
{
    menu_Add(true);
}

void MainWindow::menu_GroupeAdd() //اضافة مجمعة جديدة الى الشجرة
{
    menu_Add(false);
}

void MainWindow:: menu_Add(bool categorie)
{
    QInputDialog *dff=new QInputDialog(this);
    dff->setOkButtonText(trUtf8("موافق"));
    dff->setLayoutDirection(Qt::RightToLeft);
    dff->setCancelButtonText(trUtf8("الغاء"));
    if (categorie==true){
        dff->setLabelText(trUtf8("ادخل اسما للقسم الجديد"));
        dff->setWindowTitle(trUtf8("قسم جديد"));
    }else{
        dff->setLabelText(trUtf8("ادخل اسما للمجموعة الجديدة"));
        dff->setWindowTitle(trUtf8("مجموعة جديدة"));
    }

    QString text;
    if (dff->exec()==QInputDialog::Accepted )
    {
        text=dff->textValue();
        //***********
        QVariant newId;
        newId=Utils::genirateId(ui->treeWidget_books);
        //*************
        QTreeWidgetItem *curentitem;//العنصر المحدد
        QTreeWidgetItem *item;//العنصر الذي سنظيفه
        curentitem= ui->treeWidget_books->currentItem();
        if (categorie==true){                   //اذا لم يكن هناك تحديد سيظاف الى الجذر
            item= new QTreeWidgetItem(ui->treeWidget_books);

        }else{
            if (!curentitem->parent()){                   //اذا لم يكن هناك تحديد سيظاف الى الجذر
                item= new QTreeWidgetItem(curentitem);
            }else{        // اما ان كان هناك عنصر محدد سيظاف بعده
                item= new QTreeWidgetItem(curentitem->parent());
            }
        }
        item->setText(0,text );
        item->setData(2,1,newId.toString());
        item->setIcon(0,m_iconGroup);
        item->setSelected(true);
        //  curentitem->setSelected(false);
        ui->treeWidget_books->setCurrentItem(item);//تفعيل العنصر المظاف
        m_treeGroupIsModified=true;
        if (m_treeGroupIsModified==true){
            int reponse = QMessageBox::warning(this, trUtf8("تنبيه"), trUtf8("هل تريد حفظ التغييرات المدخلة على مجموعة الكتب ") , QMessageBox::Yes | QMessageBox::No);
            if (reponse == QMessageBox::Yes){
                if ( Utils::treeSaveGroupe(ui->treeWidget_books)){
                    QMessageBox::information(this,trUtf8("معلومات"), trUtf8("تمت عملية حفظ مجموعة الكتب بنجاح"));
                    m_treeGroupIsModified=false;
                }
            }
        }
    }
}

// حذف المجموعة
void MainWindow::menu_GroupeRemove()//
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    if (item->childCount()>0){
        QMessageBox::information(this,trUtf8("تنبيه"),trUtf8("المحموعة تحتوي على عناصر تحتهاقم بحذف هذه العناصر أولا"));
    }else{
        int reponse = QMessageBox::question(this, trUtf8("معلومات"), trUtf8("هل تريد حذف المجموعة المحددة؟ ") , QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes){
            if (Utils::treeviewItemRemove(ui->treeWidget_books)==true){
                m_treeGroupIsModified=true;}
        }
    }
}
// اعادة تسمية المجموعة
void MainWindow::menu_GroupeRename()  //
{
    QTreeWidgetItem *item=ui->treeWidget_books->currentItem();
    QInputDialog *dff=new QInputDialog(this);
    dff->setOkButtonText(trUtf8("موافق"));
    dff->setLayoutDirection(Qt::RightToLeft);
    dff->setCancelButtonText(trUtf8("الغاء"));
    dff->setLabelText(trUtf8("ادخل النص الجديد"));
    dff->setWindowTitle(trUtf8("اعادة تسمية المجموعة"));
    dff->setTextValue(item->text(0));
    if (dff->exec()==QInputDialog::Accepted ){
        if (dff->textValue().isEmpty()){return;}
        item->setText(0,dff->textValue());
        m_treeGroupIsModified=true;
    }
}
//EDIT ***  تحرير الفهرسةوالكتاب**********************************
void MainWindow::on_action_edit_triggered(bool checked)  //تحرير الفهرسة
{
    if(!curentBook)return;
    if(curentBook->getBookName().isEmpty())return;
    curentBook->setEditBook(checked);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),trUtf8("تحرير : ")+curentBook->getTitle());


}
//EDIT ***

void MainWindow::on_actionCreatNewBook_triggered()
{

    dialogupdat *dlgc=new dialogupdat(this);
    dlgc->creatNewBook="creat";
    dlgc->m_pathCostum=m_pathCostm;
    dlgc->bookTitle=trUtf8("كتاب جديد");
    dlgc->bookAutor=trUtf8("مؤلف جديد");

    dlgc->recharge();
    if ( dlgc->exec() == QDialog::Accepted ){
        chargeGroupe();
    }
    delete dlgc;
}

// تفعيل عنصر المفضلة--------------------------------
void MainWindow::on_treeWidget_fav_itemActivated(QTreeWidgetItem* item)
{
    if (item->data(1,1).toString().isEmpty()){return;}
    int bkpage=item->data(2,1).toInt();
    QString bknam=item->data(1,1).toString();
    foreach (TabBook *tab, m_listTabBook) {
        if (bknam==tab->getBookName()){

            tab->gotoPage(bkpage);
            ui->tabWidget->setCurrentIndex(m_listTabBook.indexOf(tab));
            return;
        }
    }

        open_dataBase(item->data(1,1).toString(),item->text(1),item->text(2),bkpage);



}

void MainWindow::on_actionFavSave_triggered()
{
    Utils::favoriteSave(ui->treeWidget_fav);
}

void MainWindow::on_actionFavItemRen_triggered()
{
    QTreeWidgetItem *item=ui->treeWidget_fav->currentItem();
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
    }
}

void MainWindow::on_actionFavItemUp_triggered()
{
    QTreeWidgetItem *item=ui->treeWidget_fav->currentItem();
    if(!item){ return;}
    Utils::treeviewItemUp(ui->treeWidget_fav);
}

void MainWindow::on_actionFavItemDown_triggered()
{
    QTreeWidgetItem *item=ui->treeWidget_fav->currentItem();
    if(!item){ return;}
   Utils::treeviewItemDown(ui->treeWidget_fav);
}

void MainWindow::on_actionFavDelete_triggered()
{
    QTreeWidgetItem* item=ui->treeWidget_fav->currentItem();
    if(!item){ return;}
    QString text;
    if (item->childCount()>0){
        text= trUtf8("العنصر الذي تريد حذفه يحتوي على عناصر اخرى هل تريد حذفه رغم ذلك ؟");
    }else{
        text= trUtf8("هل تريد حذف العنصر المحدد");
    }
    int reponse = QMessageBox::warning(this, trUtf8("تنبيه"),text , QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::No){return;}
    Utils::treeviewItemRemove(ui->treeWidget_fav);
}

//**treeVeiwCuran-----------------------------
void MainWindow::on_treeWidget_curaan_itemActivated(QTreeWidgetItem* item)//تفعيل عنصر شجرة القرءان
{

    if(!curentBook)return;
    if(curentBook->getBookIsTefsir()==false)return;

        ui->treeWidget_curaan->setCursor(QCursor(Qt::WaitCursor));

        QString mysora = item->data(1,1).toString().section('_', 0, 0);
        QString myaya = item->data(1,1).toString().section('_', -1);

        if (curentBook->gotoSoraAya(mysora,myaya)==true)  {

            ui->treeWidget_curaan->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

    ui->treeWidget_curaan->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_pushButton_gofind_clicked()
{
    if (ui->lineEdit_find_cur->text()!=trUtf8("بحث")){
        ui->pushButton_gofind->setCursor(QCursor(Qt::WaitCursor));
        Findbook->searchInTreeview(ui->treeWidget_curaan,ui->lineEdit_find_cur->text(),0);
        ui->pushButton_gofind->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void MainWindow::on_lineEdit_find_cur_textChanged(QString searchString)
{
    if (searchString.isEmpty()){
        Findbook->searchInTreeview(ui->treeWidget_curaan,ui->lineEdit_find_cur->text(),0);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)//codexml souara  //فتح السور فتح الاجزاء
{
    ui->treeWidget_curaan->clear();
    if (index==0){
        Utils::treeChargeJozaa(ui->treeWidget_curaan);
    }else {
        Utils::treeChargeSoura(ui->treeWidget_curaan);
    }
}

void MainWindow::on_pushButton_gotafsir_clicked()//فتح كتاب التفسير
{
    int index=ui->comboBox_tafasir->currentIndex();
    QVariant comboR_data=ui->comboBox_tafasir->itemData(index);
    QString myname = comboR_data.toString().section('/', 0, 0);
    QString myaut = comboR_data.toString().section('/', -1);
    if (!myname.isEmpty())
    {

        open_dataBase(myname,ui->comboBox_tafasir->currentText(),myaut,0);
        if(curentBook) curentBook->setBookIsTefsir(true);
     }
}

//**treeViewFind---------------------------

void MainWindow::on_actionFindItemUp_triggered()
{
    QTreeWidgetItem *item=ui->treeWidgetFind->currentItem();
    if(!item){ return;}
Utils::treeviewItemUp(ui->treeWidgetFind);
}

void MainWindow::on_actionFindItemDown_triggered()
{
    QTreeWidgetItem *item=ui->treeWidgetFind->currentItem();
    if(!item){ return;}
    Utils::treeviewItemDown(ui->treeWidgetFind);
}

void MainWindow::on_actionFindDelete_triggered()
{
    QTreeWidgetItem* item=ui->treeWidgetFind->currentItem();
    if(!item){ return;}
    QString text= trUtf8("هل تريد حذف العنصر المحدد");

    int reponse = QMessageBox::warning(this, trUtf8("تنبيه"),text , QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::No){return;}
   Utils::treeviewItemRemove(ui->treeWidgetFind);
}

void MainWindow::on_actionFindSave_triggered()
{

    if (ui->treeWidgetFind->topLevelItemCount()>0){
        QString homeDir=QDir::homePath () ;
        QString fn = QFileDialog::getSaveFileName(this, tr("Save File..."),
                                                  homeDir, trUtf8("xml files (*.xml );"));
        Findbook->findText=m_textfind;
        Findbook->isNassToFind=isNassToFind;
        if( Findbook->saveResultFind(ui->treeWidgetFind,fn)==true){
            QMessageBox::information(this,trUtf8("معلومات"), trUtf8("تمت العملية الحفظ بنجاح"));
        }else{
            QMessageBox::information(this,trUtf8("خطأ"), trUtf8("لا يمكن كتابة الملف"));
        }
    }
}

void MainWindow::on_actionFindLoad_triggered()
{

    QString homeDir=QDir::homePath () ;
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              homeDir, tr("XML files (*.xml);;All files(*)"));
    QFile file(fn);
    if (file.exists()){
        ui->treeWidgetFind->setColumnWidth(0,500);
        ui->treeWidgetFind->setColumnWidth(1,30);
        ui->treeWidgetFind->setColumnWidth(2,50);
        ui->treeWidgetFind->clear();
        Findbook->loadResultFind(ui->treeWidgetFind,fn);
        m_textfind=  Findbook->findText;
        isNassToFind=Findbook->isNassToFind;
        labelTextFind->setText(trUtf8("نص البحث  : ")+m_textfind);
    }
}

//**Animation----------------------------------------

void MainWindow::on_actionAnimGroup_triggered()
{
    ui->stackedWidget_intro->setCurrentWidget(ui->page_anim);

    view->chargeLevelOne();
    view->setFocus();

}

void MainWindow::on_actionAnimHistorie_triggered()
{
    ui->stackedWidget_intro->setCurrentWidget(ui->page_anim);
    int count=0;
    for (int i=0;i<16;i++){
        if (recentFileActs[i]->isVisible()){
            count++;
            view->recentFileActs[i]= recentFileActs[i];
        }
    }
    view->recentNbr=count+1;
    view->chargeRecent();
    view->setFocus();
}

void MainWindow::on_actionTreeD_toggled(bool checked)
{
    view->threeD=checked;
    view->setFocus();
}


//tabs---------------------------
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{

    if(curentBook){
        if(!curentBook->getBookName().isEmpty()){
            if(curentBook->closeRequested()==false)
                return;
            else
                qDebug()<<"true returned";

            int idbk=curentBook->getBookPos();
            recentBooks->recentChange(curentBook->getBookName(),
                                      curentBook->getTitle(),
                                      curentBook->getAuthor(),
                                      QString::number(idbk),
                                      m_recentNbr,
                                      curentBook->getBookIsTefsir());
            recentBooks->recentCharge();
        }

    }



    if(ui->tabWidget->widget(index)!=ui->tabIntro
            &&index>-1
            && index<m_listTabBook.count())
    {
       TabBook *Book=m_listTabBook.at(index);

        m_listTabBook.removeOne(Book);
         Book->close();
   delete Book;
        qDebug()<<"delete"<<index;

        QWidget *wid=ui->stackedWidget->widget(index);
        delete wid;
       // ui->stackedWidget->removeWidget(ui->stackedWidget->widget(index));
    }else{
         ui->stackedWidget->removeWidget(ui->stackedWidget->widget(index));
         ui->tabWidget->removeTab(index);
         m_listTabBook.removeAt(index);
         qDebug()<<"delete"<<index;
         qDebug()<<"m_listTabBook"<<m_listTabBook.count();
    }
 //
 ui->tabWidget->setTabsClosable(ui->tabWidget->count()-1);
 if(ui->tabWidget->count()==1)
 on_tabWidget_currentChanged( 0 );

}

void MainWindow::on_tabWidget_currentChanged(int index )
{


    ui->stackedWidget->setCurrentIndex(index);
    ui->tabWidget->setCurrentIndex(index);

    if(ui->tabWidget->widget(index)!=ui->tabIntro
            &&index>-1
            && index<m_listTabBook.count())
    {

        curentBook=m_listTabBook.at(index);
     curentBook-> updateSignals();

        ui->action_edit->setChecked(curentBook->getIsEditable());
        ui->actionRemoveTechkil->setChecked(curentBook->getIstechkileRemoved());

    }else{
        curentBook=0;
        emit bookAvailable(false);
        ui->action_edit->setChecked(false);
        ui->actionRemoveTechkil->setChecked(false);
    }

    ui->tabWidget->setTabsClosable(ui->tabWidget->count()-1);

  qDebug()<<"tabWidget_currentChanged"<<index;
   qDebug()<<"m_listTabBook.count"<<m_listTabBook.count();
}


void MainWindow::tabCreatTabNew()
{
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/image/booki.png")), QIcon::Normal, QIcon::Off);

     TabBook *tab=new TabBook;
    connect(tab,SIGNAL(bookAvailable(bool)),this,SIGNAL(bookAvailable(bool)));
    connect(tab,SIGNAL(pdfAvailable()),this,SLOT(setupActionsPdf()));

   m_listTabBook.append(tab);
   ui->tabWidget->addTab(tab,icon,trUtf8("مكتبة القرطاس"));

   ui->stackedWidget->addWidget(tab->widgetFahras);

    ui->tabWidget->setCurrentWidget(tab);
    ui->stackedWidget->setCurrentWidget(tab->widgetFahras);

   connect(lineEditFindFah, SIGNAL(textChanged(QString)), this, SLOT(searchInTreeFah(QString)));
   connect(AC_expandTreeFah, SIGNAL(triggered()),tab->treeViewFahras, SLOT(expandAll()));
   connect(AC_colapsTreeFah, SIGNAL(triggered()),tab->treeViewFahras, SLOT(collapseAll()));
   connect(tab, SIGNAL(nextHistorieEnabled(bool)), AC_nextHistorie, SLOT(setEnabled(bool)));
   connect(tab, SIGNAL(prevHistorieEnabled(bool)), AC_prevHistorie, SLOT(setEnabled(bool)));
   connect(tab, SIGNAL(pagePartChanged(QString)), labelProgressInfo, SLOT(setText(QString)));



   connect(this ,SIGNAL(settingsChanged()),tab,SIGNAL(settingsChanged()));


   curentBook=tab;
}


void MainWindow::tabOpenInNewTab()
{
    tabCreatTabNew();
    menuOpenBook();
}

void MainWindow::chargeGroupe()//تحميل شجرة الكتب
{
    Utils::treeChargeGroupe(ui->treeWidget_books,0,false,ui->comboBox_tafasir);
}



void MainWindow::saveLayou()//حفظ البيانات الى ملف
{
#ifdef Q_OS_HAIKU
	QSettings settings(m_pathUser+"/setting.ini",
                       QSettings::IniFormat);
#else
    QSettings settings(m_pathUser+"/data/setting.ini",
                       QSettings::IniFormat);
#endif
    settings.beginGroup("MainWindow");

    settings.setValue("geo_data", saveGeometry());
    settings.setValue("layout_data", saveState(0));


    settings.endGroup();

}

void MainWindow::loadLayout()//load layou
{

    QByteArray layo_data=QByteArray::fromHex("(000000ff00000000fd00000002000000010000010600000276fc0200000001fc0000004000000276000001330007"
                                             "fffffa000000020100000003fb000000220064006f0063006b005700690064006700650074005f0063007500720061006100"
                                             "6e0100000000ffffffff000000b600fffffffb000000220064006f0063006b005700690064006700650074005f00660061006"
                                             "80072006100730100000000ffffffff000000970007fffffb000000200064006f0063006b005700690064006700650074005f0"
                                             "062006f006f006b0073010000045200000106000000780000058c0000000300000558000000c7fc0100000001fc0000000000000"
                                             "5580000000000fffffffaffffffff0200000002fb000000260064006f0063006b005700690064006700650074005f0066006100760"
                                             "06f00720069007400650000000000ffffffff000000aa00fffffffb0000001e0064006f0063006b005700690064006700650074005"
                                             "f00660069006e006400000002490000006d0000006d00ffffff0000044c0000027600000004000000040000000800000008fc000000"
                                             "01000000020000000300000016006d00610069006e0054006f006f006c0042006100720100000000ffffffff00000000000000000000"
                                             "00220074006f006f006c004200610072005f006e006100760065006700610074006f00720100000203ffffffff000000000000000000"
                                             "00001a0074006f006f006c0042006100720072006500630065006e007401000002a9ffffffff0000000000000000)");
#ifdef Q_OS_HAIKU
	QSettings settings(m_pathUser+"/setting.ini",QSettings::IniFormat);
#else
    QSettings settings(m_pathUser+"/data/setting.ini",QSettings::IniFormat);
#endif
    settings.beginGroup("MainWindow");

    this->restoreGeometry(settings.value("geo_data").toByteArray());
    this->restoreState( settings.value("layout_data",layo_data).toByteArray());
    settings.endGroup();


}

void MainWindow::loadSettings()
{

#ifdef Q_OS_HAIKU
	QSettings settings(m_pathUser+"/setting.ini",QSettings::IniFormat);
#else
    QSettings settings(m_pathUser+"/data/setting.ini",QSettings::IniFormat);
#endif
    settings.beginGroup("MainWindow");

    QString   m_myStyleName=settings.value("style","").toString();

    m_WebFontFindColor=settings.value("WebFontFindColor","#aa5500").toString();
    m_recentNbr=settings.value("recentNbr",7).toInt();
    m_pathCostm=settings.value("pathCostm",m_pathUser+"/books").toString();
    ui->actionTreeD->setChecked(settings.value("isThreeD",false).toBool())   ;
    m_layouDir=settings.value("layouDir",false).toBool();
    //   m_arabicNmber=settings.value("arabicNmber",0).toInt();
      QString    m_styleCostum=settings.value("styleCostum","").toString();
    bool   m_isStyleCostum=settings.value("isStyleCostum",false).toBool();

    if(m_layouDir==false) {
        qApp->setLayoutDirection(Qt::RightToLeft);
    }else{
        qApp->setLayoutDirection(Qt::LeftToRight);
    }
#if QT_VERSION >= 0x040600
    view->threeD=settings.value("isThreeD",false).toBool();
#endif
    settings.endGroup();//---MainWindow

    settings.beginGroup("ToolBar");
    m_listActToAdd=settings.value("listActToAdd",m_listActToAdd<<
                                  29<<28<<50<<1000<<15<<16<<17<<18<<1000<<20<<21<<22<<23<<24<<1000<<26<<48<<49<<2000<<27).toList();
    m_isStandardToolBar=settings.value("isStandardToolBar",true).toBool();
    m_toolIconSize=settings.value("toolIconSize",22).toInt();
    m_textUnderIcon=settings.value("textUnderIcon",false).toBool();

    settings.endGroup();//---ToolBar
    if(m_isStyleCostum==true){
        qApp->setStyle(("Cleanlooks" ));
        QFile qss(m_styleCostum);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();

    }else{
        qApp->setStyleSheet("");
        QApplication::setStyle(QStyleFactory::create( m_myStyleName));

    }
}



//text browser***عمليات على النص**********************************
void MainWindow::textBookAnchorClicked(QUrl txtUrl)
{
    ui->textBrowser_0->setOpenExternalLinks(false);
    ui->textBrowser_0->setOpenLinks(true);
    QString textCode=txtUrl.toString();
    if (textCode.isEmpty()) {return;}
    if (textCode.contains("javascript")){
        ui->textBrowser_0->setOpenLinks(true);
        return;
    }
    if (textCode.contains("#*")){
        textCode.remove("#*");
        if (textCode.contains("(")||textCode.contains(":")){
            int t=textCode.indexOf("(",0);
            textCode.remove(t,textCode.length());
            int d=textCode.indexOf(":",0);
            textCode.remove(d,textCode.length());
        }

        rowatInterface->rawiFind(textCode.trimmed(),ui->treeWidget_rowat,ui->treeWidget_rowatFind);
        showfind(true);

        return;
    }
    if (textCode.contains("http://")){
        ui->textBrowser_0->setOpenLinks(false);
        ui->textBrowser_0->setOpenExternalLinks(false);
        QString nameBk= ui->textBrowser_0->textCursor().block().text();
        QMessageBox::information(this,"name",nameBk +"\n"+ textCode);
return;

    }else if(textCode.contains("file://")||textCode.contains("javascript")){

        ui->textBrowser_0->setOpenLinks(true);
    }

}

void MainWindow::textBookHighlighted(QUrl txtUrl)
{
    QString textCode=txtUrl.toString();
     ui->statusBar->showMessage(textCode);

    if (textCode.isEmpty()|| textCode.contains("www.intro")) {
        ui->statusBar->showMessage("");
        return;
    }
    if (textCode.contains("#*")){
        ui->statusBar->showMessage(textCode.remove("#*"));
        return;
    }
    if (textCode.contains("http://")){
        ui->textBrowser_0->setOpenExternalLinks(false);
        ui->textBrowser_0->setOpenLinks(false);
        ui->statusBar->showMessage(txtUrl.toString());


    }else if(textCode.contains("file://")){

        ui->textBrowser_0->setOpenLinks(true);
    }
}

//find***عمليات البحث***********************************

void MainWindow::find_favo_textChanged(QString searchString)//البحث في شجرة المفضلة
{
    if (searchString!=trUtf8("بحث"))
        Findbook->searchInTreeview(ui->treeWidget_fav,searchString,0);
}
void MainWindow::searchInTreeFah(QString searchString)//البحث في شجرة الفهرسة
{
    if(!curentBook)return;
    if (searchString!=trUtf8("بحث"))
        Findbook->searchInTreeview(curentBook->treeViewFahras,searchString,0);
}
void MainWindow::searchInTreeBook(const QString &searchString)//البحث في شجرة الكتب
{
    int colum=0;
    if (m_findIsTitle==false){
        colum=1;
    }
    if (searchString!=trUtf8("بحث"))
        Findbook->searchInTreeview(ui->treeWidget_books,searchString,colum);
}
// النقر على ششجرة البحث
void MainWindow::showResultFind(QTreeWidgetItem* item)//تفعيل عناصر شجرة البحث
{

    QVariant bkpage=item->data(0,1);

    QString x = item->data(3,1).toString();
    open_dataBase(x,item->text(3),item->text(4),bkpage.toInt()-1);
    if(!curentBook)return;

    curentBook->setBookIsTefsir(item->data(2,1).toBool());

    curentBook->gotoPage(bkpage.toInt()-1);
    if(isNassToFind==true){
        bool modified=curentBook->txtBrowserBook->document()->isModified();
        Findbook->searchInDoc(curentBook->txtBrowserBook->document(),m_textfind,QColor(m_WebFontFindColor));
  curentBook->txtBrowserBook->document()->setModified(modified);
    }else{
        QTreeWidgetItem *itemF=new QTreeWidgetItem;
        QList<QTreeWidgetItem *> found = curentBook-> treeViewFahras->findItems(  item->text(0),Qt::MatchExactly | Qt::MatchRecursive,0);
        foreach (itemF, found){curentBook-> treeViewFahras->setCurrentItem(itemF); }
    }
}
// البحث في الكتاب الحالي
void MainWindow::findInCurrentBook()//البحث في الكتاب الحالي
{
    if(!curentBook)return;
     if(curentBook->getBookName().isEmpty())return;

    Findbook->pathCostum= curentBook->getBookPath();
    if (Findbook->isFindRuned==true){ return;}

    if (lineEditSearchInDoc->text().isEmpty()){ return;}
    if (lineEditSearchInDoc->text()==trUtf8("بحث")){ return;}
    m_textfind=lineEditSearchInDoc->text();

    showfind(false);

    labelTextFind->setText(trUtf8("نص البحث  : ")+m_textfind);
    progressBarFind->setMaximum(curentBook->getRowCount());
    progressBarFind->setVisible(true);
    //  BtnStopFind->setVisible(true);
    ui->action_stopFind->setVisible(true);
    Findbook->findMultiFind=false;
    Findbook->findSawabik=true;
    Findbook->findToList=false;
    Findbook->resultCount=0;
    Findbook->isTefsir=curentBook->getBookIsTefsir();
    Findbook-> findTitle=curentBook->getTitle();
    Findbook-> findAuthor=curentBook->getAuthor();
    Findbook-> findName=curentBook->getBookName();
    Findbook->findText=m_textfind;
    Findbook->progressBar1=progressBarFind;
    Findbook->label_progressImg=labelProgressImg;
    // Findbook->pushButtonStop=BtnStopFind;
    Findbook->buttonStop= ui->action_stopFind;
    Findbook->treeView=ui->treeWidgetFind;
    Findbook->isNassToFind=true;
    isNassToFind=true;

    ui->treeWidgetFind->clear();
    // QToolTip::showText(QPoint (ui->statusBar->width(),ui->statusBar->pos().y()),trUtf8("يمكنك ايقاف عملية البحث هنا"),BtnStopFind);
    //  m_isRawiToFind=false;
    Findbook->findOneBook(curentBook->getBookPath());

}
void MainWindow::findBookOrHautor()//البحث عن كتاب
{
    if (ui->actionFindBkOrHt->isChecked()){
        ui->actionFindBkOrHt->setToolTip(trUtf8("البحث عن مؤلف"));
        m_findIsTitle=false;
    }else{
        ui->actionFindBkOrHt->setToolTip(trUtf8("البحث عن كتاب"));
        m_findIsTitle=true;
    }
}

void MainWindow::stopFind()//توقيف عملية البحث
{
    Findbook ->stopFind=true;
}

void MainWindow::searchIndoc()
{
        if (lineEditSearchInDoc->text()==trUtf8("بحث")){ return;}
         if(!curentBook)return;


    if (!lineEditSearchInDoc->text().isEmpty())
    {
        if(curentBook->getIsPdf()){
            curentBook-> serchInPdf(lineEditSearchInDoc->text());
            return;
        }
        bool modified=curentBook->txtBrowserBook->document()->isModified();
        Findbook->findToList=false;
        if (Findbook->searchInDoc(curentBook-> txtBrowserBook->document(),lineEditSearchInDoc->text(),QColor(m_WebFontFindColor))==false)
        {
            QMessageBox::information(this,trUtf8("معلومات"), trUtf8("النص غير موجود في الصفحة"));
        }
        curentBook->txtBrowserBook->document()->setModified(modified);

    }
}


void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){

        QVariant txtdata=action->data();
        QString myid = txtdata.toString().section('|', 0, 0);
        QString myaut= txtdata.toString().section('|', 2,2);
        QString  myname=txtdata.toString().section('|', 1, 1);
        QVariant  mytefsir=txtdata.toString().section('|', -1);

        if(myname.isEmpty())return;

        open_dataBase(myname,action->text(),myaut,myid.toInt());

        if(curentBook)
                  curentBook->setBookIsTefsir(mytefsir.toBool());


    }
}
//--------------------plugins--------------------------------------------


void MainWindow::on_comboBox_rowat_currentIndexChanged(int index)
{
    if (loadPlugin(ROWAT_PLUG)==false)
    {
        QMessageBox::information(this,trUtf8("تعليمات"),trUtf8("تأكد من ان الاضافة موجودة في دليل البرنامج"));
        ui->comboBox_rowat->setEnabled(false);
        return;
    }
    QFile file(m_pathApp+"/data/rowaInfo.xml");

    if (index==1){
        if(!file.exists()){
            QMessageBox::information(this,trUtf8("تعليمات"),trUtf8("قاعدة بيانات تراجم الرواة غير موجدة الرجاء تحميل قاعدة البيانات من موقع المكتبة وقم بنسخها الى مجلد data في دليل البرنامج"));
            ui->comboBox_rowat->setCurrentIndex(0);
            return;
        }

        if(ui->treeWidget_rowat->topLevelItemCount()<1)
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));


        rowatInterface->treeChargeRowtName(ui->treeWidget_rowat);
        // Rowat->chargeRowat();
        // Rowat->openXml();
        QApplication::restoreOverrideCursor();
    }else{
       // on_actionPageIntro_triggered();
    }
    ui->stackedWidget_books->setCurrentIndex(index);

}

void MainWindow::on_lineEdit_FindRowat_textChanged(QString searchString)
{
    if (searchString!=trUtf8("بحث"))
        Findbook->searchInTreeview(ui->treeWidget_rowat,searchString,0);
}


void MainWindow::on_treeWidget_rowat_itemActivated(QTreeWidgetItem* item, int )
{
    ui->treeWidget_rowatFind->setVisible(false);
    openRawi(item,true);

}
void MainWindow::openRawi(QTreeWidgetItem* item,bool addToTataboa)
{
    int pos=item->data(1,1).toInt();

    if( ui->tabIntro->isHidden())  {

        ui->tabWidget->insertTab(0,ui->tabIntro,trUtf8("مكتبة القرطاس"));
        ui->stackedWidget->insertWidget(0,ui->pageIntro);
          m_listTabBook.insert(0,BookIntro);
        ui->tabWidget->setCurrentWidget(ui->tabIntro);
    }
    ui->stackedWidget_intro->setCurrentWidget(ui->page_text);
    
    labelAnim->setText(trUtf8("تراجم الرواة"));
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ui->textBrowser_0->setHtml(rowatInterface->readxml(pos));
    //    ui->textBrowser_0->setHtml(Rowat->moveToPosition(pos));

    QApplication::restoreOverrideCursor();
    if (addToTataboa==true){

        QTreeWidgetItem *itemR=new QTreeWidgetItem;
        QList<QTreeWidgetItem *> foundR =
                ui->treeWidget_rowatTabia->findItems(
                    item->text(0),Qt::MatchExactly,0);//تنشاء قائمة بكل العناصر

        foreach (itemR, foundR){
            ui->treeWidget_rowatTabia->setCurrentItem(itemR);
            return;
        }
        QTreeWidgetItem *itemF=new QTreeWidgetItem(ui->treeWidget_rowatTabia);
        itemF->setText(0,item->text(0));
        itemF->setData(1,1,ui->textBrowser_0->toHtml());
        ui->treeWidget_rowatTabia->setCurrentItem(itemF);

    }
}

void MainWindow::on_treeWidget_rowatTabia_itemActivated(QTreeWidgetItem* item, int )
{
    ui->treeWidget_rowatFind->setVisible(false);
    ui->textBrowser_0->setHtml(item->data(1,1).toString());
}
void MainWindow::showfind(bool isrowat)
{
    //  ui->treeWidgetFind->clear();
    if (isrowat==true){
        if (ui->treeWidget_rowatFind->topLevelItemCount()==1){
            openRawi(ui->treeWidget_rowatFind->topLevelItem(0),true);
            ui->treeWidget_rowatFind->setVisible(false);
            ui->treeWidget_rowatFind->clear();
        }else if  (ui->treeWidget_rowatFind->topLevelItemCount()>1){
            ui->treeWidget_rowatFind->setVisible(true);
        }else{
            ui->treeWidget_rowatFind->setVisible(false);
            ui->treeWidget_rowatFind->clear();
            //              سعيد بن أبى هلال الليثى مولاهم//
            QMessageBox::information(this,trUtf8("تراجم"),trUtf8("ربما لم يترجم له في التهذيبين \n حاول البحث عنه في مربع البحث اعلى قائمة الرواة"));
        }
    }else{
        ui->treeWidgetFind->setColumnWidth(0,500);
        ui->treeWidgetFind->setColumnWidth(1,30);
        ui->treeWidgetFind->setColumnWidth(2,50);
        ui->toolBarFind->setVisible(true);
        ui->dockWidget_find->setVisible(true);
    }

}

void MainWindow::on_treeWidget_rowatFind_itemActivated(QTreeWidgetItem* item, int )
{
    openRawi(item,true);
}

void MainWindow::on_btnRowaInfo_clicked()
{
    QString romoz=trUtf8(
                "<span style=\"color:#ff0000;\">خ</span>=البخاري , <span style=\"color:#ff0000;\">م</span>=مسلم , <span style=\"color:#ff0000;\">د</span>=أبو داود , <span style=\"color:#ff0000;\">ت</span>=الترمذي , <span style=\"color:#ff0000;\">س</span> =النسائي , <span style=\"color:#ff0000;\"><span style=\"color:#ff0000;\">ق</span></span>=ابن ماجه<br>"
                "<span style=\"color:#ff0000;\">بخ</span> =البخاري في الأدب المفرد , <span style=\"color:#ff0000;\">عخ</span> =البخاري في خلق أفعال العباد<br>"
                "<span style=\"color:#ff0000;\">سي</span>=النسائي في عمل اليوم والليلة, <span style=\"color:#ff0000;\">ي</span> =البخاري في جزء رفع اليدين<br>"
                "<span style=\"color:#ff0000;\">عس</span>= النسائي في مسند علي                      , <span style=\"color:#ff0000;\">مد</span>=أبو داود في المراسيل<br>"
                "<span style=\"color:#ff0000;\">تمييز</span>=لم يخرج له أحد من الستة                 , <span style=\"color:#ff0000;\">فق</span>=ابن ماجه في التفسير<br>"
                "<span style=\"color:#ff0000;\">خت</span> = البخاري تعليقا                     ,  <span style=\"color:#ff0000;\">صد</span>=أبو داود في فضائل الأنصار<br>"
                "<span style=\"color:#ff0000;\">خد</span>=أبو داود في الناسخ والمنسوخ                ,  <span style=\"color:#ff0000;\">ل</span>=أبو داود في المسائل<br>"
                "<span style=\"color:#ff0000;\">ص</span>=النسائي في خصائص علي            , <span style=\"color:#ff0000;\">ر</span>=البخاري في جزء القراءة خلف الإمام<br>"
                "<span style=\"color:#ff0000;\">كن</span>=النسائي في مسند مالك , <span style=\"color:#ff0000;\">تم</span>=الترمذي في الشمائل , <span style=\"color:#ff0000;\">قد</span>=أبو داود في القدر"
                );
    QMessageBox::information(this,trUtf8("رموز كتب السنة"),romoz);
}


// جلب كتاب من الانترنت-----------------------------------------------------
void MainWindow::menu_downloadBook(const QString &bkname)
{
  QTreeWidgetItem *item=Utils::getItemByBook(ui->treeWidget_books,bkname);

    QString name=item->data(1,1).toString();

    QFile file;
    if(file.exists(m_pathCostm + "/" + name)){
        QMessageBox::information(this,"",trUtf8("الكتاب موجود في جهازك\n")+m_pathCostm + "/" + name);
        return;
    }else if(file.exists(m_pathApp + "/books/" + name)){
        QMessageBox::information(this,"",trUtf8("الكتاب موجود في جهازك\n")+m_pathApp + "/books/"+ "/" + name);
        return;
    }

//    int idGroup= item->parent()->data(2,1).toInt();
//    QString urlPath=m_listUrls.at(0);
//    if (urlPath.rightRef(1)!="/")
//        urlPath+="/";

//     urlPath+=QString::number(idGroup)+"/";

    QString idurl= Utils::getUrlInBookList(name);
qDebug()<<name<<"idurl=--------------------------"<<idurl;
    //---------------------------
    if(loadPlugin(NET_PLUG)==false)
        return;


    QString targzName= netInterface->loadFile(idurl);
    if (targzName.isEmpty())
        return;
#ifdef Q_OS_HAIKU
    QString orgPath=QDir::homePath()+"/config/settings/elkirtasse/download/"+targzName;
#else
    QString orgPath=QDir::homePath()+"/.kirtasse/download/"+targzName;
#endif

    QFile filex;
    if(!filex.exists(orgPath)){

        return;
    }
    if( Utils::loadTarGz(orgPath)==false)
        return;

    QString distPath;
#ifdef Q_OS_HAIKU
	QString dwnldPath=QDir::homePath()+"/config/settings/elkirtasse/download/";
#else
	QString dwnldPath=QDir::homePath()+"/.kirtasse/download/";
#endif

    if(file.exists(QDir::homePath()+dwnldPath+name+"/bookinfo.info")){
       distPath=QDir::homePath()+dwnldPath+name;
       qDebug()<<"distPath-------EXist-------------------------------"<<distPath;
     }else if(file.exists(QDir::homePath()+dwnldPath+"/bookinfo.info")){
           distPath=QDir::homePath()+dwnldPath;
          qDebug()<<"distPath-------EXist-------------------------------"<<distPath;
    }else{
        qDebug()<<"distPath-------NON EXist-------------------------------";
        return;
    }


    QDir dir(m_pathCostm);
    if(dir.mkdir(name)==true){
        QFile file;
        file.copy(distPath+"/book.xml",m_pathCostm+ "/" + name+"/book.xml");
        file.copy(distPath+"/title.xml",m_pathCostm+ "/" + name+"/title.xml");
        file.copy(distPath+"/bookinfo.info",m_pathCostm+ "/" + name+"/bookinfo.info");
    }
    if(filex.exists(m_pathCostm+ "/" + name+"/book.xml"))
        QMessageBox::information(this,"",trUtf8("تمت العملية بنجاح \n")+m_pathCostm + "/" + name);
#ifdef Q_OS_HAIKU
	QString tempDirs=QDir::homePath()+"/config/settings/elkirtasse/download/";
    m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
	QString tempDirs=QDir::homePath()+"/.kirtasse/download/";
	m_pathUser=QDir::homePath()+"/.kirtasse";
#endif

    Utils::removeTempDirs(QDir::homePath()+tempDirs);

    open_dataBase(name,item->text(0),item->text(1),0);
}

void MainWindow::on_actionDownloadBooks_triggered()
{
	QString tempDirs;
    if(loadPlugin(NET_PLUG)==false)
        return;

// QString urlPath=m_listUrls.at(0);
// if (urlPath.rightRef(1)!="/")
//     urlPath+="/";

 QString targzName= netInterface->execPlugin();
 qDebug()<<"recevier -------------------------------------"<<targzName;
       if (targzName.isEmpty())
        return;
#ifdef Q_OS_HAIKU
    QString tarGzPath=QDir::homePath()+"/config/settings/download/"+targzName;
#else
	QString tarGzPath=QDir::homePath()+"/.kirtasse/download/"+targzName;
#endif
    qDebug()<<"chemain download + targz nam----------------------------"<<tarGzPath;

    QFile file;
    if(!file.exists(tarGzPath)){
      qDebug()<<"chemain download + targz nam-----------Non Exist-------------"<<tarGzPath;
        return;
    }
    if( Utils::loadTarGz(tarGzPath)==false){
        qDebug()<<"loadTarGz-------NOn EXtacted-------------------------------"<<tarGzPath;

        return;
}
    qDebug()<<"loadTarGz-------EXtacted-------------------------------"<<tarGzPath;

     QString name=targzName.remove(".tar.gz");
     QString distPath;
#ifdef Q_OS_HAIKU
	QString dwnldPath=QDir::homePath()+"/config/settings/elkirtasse/download/";
#else
	QString dwnldPath=QDir::homePath()+"/.kirtasse/download/";
#endif

     if(file.exists(QDir::homePath()+dwnldPath+name+"/bookinfo.info")){
        distPath=QDir::homePath()+dwnldPath+name;
        qDebug()<<"distPath-------EXist-------------------------------"<<distPath;
     }else if(file.exists(QDir::homePath()+dwnldPath+"/bookinfo.info")){
           distPath=QDir::homePath()+dwnldPath;
           qDebug()<<"distPath-------EXist-------------------------------"<<distPath;
     }else{
         qDebug()<<"distPath-------NON EXist-------------------------------";
         return;
     }

    if (Utils::chekBookInfo(distPath)==false)
        return;

    qDebug()<<name+"*******************************";
    if(Utils::checkBookExist(name)){

        QDir dir(m_pathCostm);
        if(dir.mkdir(name)==true){
            QFile file;
            file.copy(distPath+"/book.xml",m_pathCostm+ "/" + name+"/book.xml");
            file.copy(distPath+"/title.xml",m_pathCostm+ "/" + name+"/title.xml");
            file.copy(distPath+"/bookinfo.info",m_pathCostm+ "/" + name+"/bookinfo.info");
        }



        if(file.exists(m_pathCostm+ "/" + name+"/book.xml"))
            QMessageBox::information(this,"",trUtf8("تمت العملية بنجاح \n")+m_pathCostm + "/" + name);

#ifdef Q_OS_HAIKU
	tempDirs=QDir::homePath()+"/config/settings/elkirtasse/download/";
    m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
#else
	tempDirs=QDir::homePath()+"/.kirtasse/download/";
	m_pathUser=QDir::homePath()+"/.kirtasse";
#endif

        Utils::removeTempDirs(QDir::homePath()+tempDirs);
        QTreeWidgetItem *item=Utils::getItemByBook(ui->treeWidget_books,name);
        if(item)
         open_dataBase(name,item->text(0),item->text(1),0);

         return;
     }

    QHash<QString,QString>hash=Utils::loadBookInfo(distPath);

    dialogupdat *dlgc=new dialogupdat(this);
    dlgc->creatNewBook="add";
    dlgc->setWindowTitle(trUtf8("اضافة كتاب جديد"));
    dlgc->m_pathCostum=m_pathCostm;
    dlgc->bookTitle=hash.value("title");
    dlgc->bookAutor=hash.value("author");
    dlgc->bookBetaka=hash.value("betaka");
    dlgc->recharge();
    if ( dlgc->exec() == QDialog::Accepted ){
        chargeGroupe();
        Utils::removeTempDirs(QDir::homePath()+tempDirs);
    }else{
        Utils::removeTempDirs(QDir::homePath()+tempDirs);
    }

    delete dlgc;

}

void MainWindow::on_actionShamilaCdrom_triggered()
{
QString tempDirs;	
#ifdef Q_OS_HAIKU
	//tempDirs=QDir::homePath()+"/config/settings/elkirtasse/download/";
	tempDirs=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/download";
    //m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
	m_pathUser=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
#else
	tempDirs=QDir::homePath()+"/.kirtasse/download/";
	m_pathUser=QDir::homePath()+"/.kirtasse";
#endif

	Utils::removeTempDirs(QDir::homePath()+tempDirs);

    if(loadPlugin(SHAMILA_PLUG)==false)
        return;
    QString newBooksPath= shamilaInterface->execPlugin();

    if(newBooksPath.isEmpty())
        return;
#ifdef Q_OS_HAIKU
		QMessageBox::information(this,trUtf8("معلومات"),trUtf8("لقد تم تحويل كتب الشاملة بنجاح\n"
                                                           "كما تم اعادة تسمية القائمة السابقة في المسار التالي اذا احتجت لاسترجاعها")+QDir::homePath()+trUtf8("/config/settings/Abouzakaria/elkirtasse/data/group.xml.old\n"
                                                                                     " كما سيتم استخدام المسار التالي للكتب \n"
                                                                                     )+newBooksPath);
#else
	QMessageBox::information(this,trUtf8("معلومات"),trUtf8("لقد تم تحويل كتب الشاملة بنجاح\n"
                                                           "كما تم اعادة تسمية القائمة السابقة في المسار التالي اذا احتجت لاسترجاعها")+QDir::homePath()+trUtf8("/.kirtasse/data/group.xml.old\n"
                                                                                     " كما سيتم استخدام المسار التالي للكتب \n"
                                                                                     )+newBooksPath);
#endif

    m_pathCostm=newBooksPath;
    chargeGroupe();

}

void MainWindow::on_actionControle_triggered()
{
    DialogConfigBooks *dlgc=new DialogConfigBooks(this);
    dlgc->setPathCostum(m_pathCostm);
    if ( dlgc->exec() == QDialog::Accepted ){
        m_pathCostm=dlgc->getPathCostum();
        chargeGroupe();
    }
    delete dlgc;
}

void MainWindow::on_actionDvd_triggered()
{
    DialogImportDvd *dlgc=new DialogImportDvd(this);
    dlgc->setBooksPath(m_pathCostm);
    dlgc->setAppPath(m_pathApp);
    if ( dlgc->exec() == QDialog::Accepted ){
        m_pathCostm=dlgc->getBooksPath();
        chargeGroupe();

    }
    delete dlgc;
}

void MainWindow::on_actionCostumActions_triggered()
{
    DialogActions *dlg=new DialogActions(this);
    dlg->setListActions(m_listActions,m_listActToAdd);
    if ( dlg->exec() == QDialog::Accepted ){
        m_listActToAdd=dlg->getListActToAdd();
        if(m_isStandardToolBar==true){
            addToolRubonBar(true);
        }
    }
    delete dlg;
}


