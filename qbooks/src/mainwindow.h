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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tabbook.h"
#include "recentbooks.h"
#include "findbook.h"
#include "booksInterface.h"
#include "toolrubon.h"
#include "animation.h"

#include <QMainWindow>
#include <QWhatsThis>
#include <QPrinter>
#include <QPrintDialog>
#include <QAbstractPrintDialog>
#include <QPrintPreviewDialog>
#include <QProgressBar>
#include <QStyleFactory>
#include <QFileDialog>
#include <QInputDialog>
#include <QWidgetAction>
#include <QProgressDialog>

class QLabel;
class QTreeWidget;
class QLabel;
class QProgressBar;
class QComboBox;
class QPushButton;
class QLineEdit;


namespace Ui
{
class MainWindowClass;
}
/*!
 *  \brief     The MainWindow class نافذة البرنامج الرئيسية
 *  \details   هذه الفئة خاصة بمكونات نافذة البرنامج الرئيسية
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();


protected:
    virtual void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *obj, QEvent *ev);
    void changeEvent(QEvent *e);
signals:
    //! اشارة الى توفر الكتاب
    void bookAvailable(bool );
    //! اشارة الى تغير الاعدادات
    void settingsChanged();
private:
    Ui::MainWindowClass *ui;
#if QT_VERSION >= 0x040600
    View *view;
#endif
    toolrubon *toolRubon; /*!< لوحة الادوات */
    TabBook *curentBook;/*!< اللسان الحالي */
    RecentBooks *recentBooks;
    findbook *Findbook;
    NetInterface *netInterface;
    RowatInterface *rowatInterface;
    ShamilaInterface *shamilaInterface;
    TabBook *BookIntro;

    static const int MaxRecentFiles=16 ;
    int m_recentNbr;/*!< عدد الكتب السابقة المسموح بها */

    QIcon m_icondir;
    QIcon m_icondfile;
    QIcon m_iconbook;
    QIcon m_iconaut;
    QIcon m_iconGroup;
    QWidgetItem *AC_lineEditSearchInDoc;       /*!< امر يحمل مربع نص اخال البحث */
    QLineEdit *lineEditSearchInDoc;              /*!< مربع ادخال نص البحث في الصفحة على لوحة الادوات */
    QLineEdit *lineEditFindFah;                  /*!< مربع ادهال نص البحث في الفهارس على لوحة الفهارس */

    QAction *AC_nextHistorie;                   /*!< امر النمقل التالي */
    QAction *AC_prevHistorie;                   /*!< امر التنقل السابق */
    QAction *AC_recent;                         /*!< امر يحمل قائمة الكتب السايقة على اللوحة */
    QAction *recentFileActs[MaxRecentFiles];    /*!< اوامر بالكتب السابقة */
    QAction *AC_GoSearchInDoc;                  /*!< امرالبحث في النص الحالي */
    QAction *AC_GoSearchInCurBook;              /*!< امر البحث في الكتاب الحالي */
    QAction *AC_bookRemove;                     /*!< امر حذف كتاب */
    QAction *AC_bookOpenInNewTab;               /*!< امر فتح الكتابالمحدد في لسان جديد */
    QAction *AC_bookRename;                     /*!< امر تغيير معرف الكتاب المحدد */
    QAction *AC_bookOpen;                      /*!< امر فتح الكتاب المحدد */
    QAction *AC_bookUpdat;                      /*!< امر تعديل معلومات الكتاب المحدد */
    QAction *AC_bookPath;                       /*!< امر جلب مسار الكتاب المحدد */
    QAction *AC_groupeRemove;                   /*!< امر حذف المجموعة المحددة */
    QAction *AC_groupeAdd;                      /*!< امر اضافة مجموعة */
    QAction *AC_categorieAdd;                   /*!< امر اضافة قسم جديد */
    QAction *AC_groupeRename;                   /*!< امر اعادة تسمية المجموعة */
    QAction *AC_colapsTreeFah;                  /*!< امر توسيع شجرة الفهرسة الحالية */
    QAction *AC_expandTreeFah;                  /*!< امر تحجيم شجرة الفهرسة الحالية */

    QPushButton *BtnStopFind;                   /*!< زر لوقف غملية البحث */

    QLabel *labelProgressInfo;                  /*!< معلومات عن الكتاب المفتوح */
    QLabel *labelProgress;                      /*!< عنوان شريط المعلومات */
    QLabel *labelProgressImg;                   /*!< لعرض تقدم البحث على شريط المعلومات */
    QLabel *labelAnim;                          /*!< لعرض مستوى المجموغات في الايقونات المتحركة */
    QLabel *labelTextFind;                      /*!< لعرض نص البحث على لوحة البحث */


    QProgressBar *progressBarFind;/*!< شريط الحالة للبحث */

    QString m_textfind;/*!< النص الذي يتم البحث عنه */
    QString m_pathUser;/*!< مسار المستخدم */
    QString m_pathApp;/*!< مسار البرنامج */
    QString m_pathCostm;/*!< مسار مخصص للكتب */
    QString m_WebFontFindColor;/*!< لون نص البحث */

    bool isNassToFind;/*!< اشارة لفئة البحث هل البحث عن نص ام فهرسة */
    bool m_treeGroupIsModified;/*!< هل تم تغيير المجموعة */
    bool m_findIsTitle;/*!< هل سيتم البحث في الشجرة عن اسم الكتاب ام المؤلف */
    bool m_layouDir;/*!< اتجاه البرنامج */
    bool  m_isStandardToolBar;/*!< هل لوحة الادواة القياسية */
    bool m_textUnderIcon;/*!< هل النص تحت الايقونة */

    int m_toolIconSize;/*!< حجم الايقونات */

    QList<TabBook *>m_listTabBook;/*!< قائمة بكل الالسن */
    QList<QAction *>m_listActions;/*!< قائمة بجميع الاوامر المتوفرة */
    QList<QString > listLineEditName;/*!<  اسماء مربعات النص لادخال البحث */
    QList<QVariant >m_listActToAdd;/*!< قائمة بكل الاوامر التي ستضاف الى لوحة الادوات */
    // QStringList m_listUrls;
public slots:

private slots:
    //! تنفيذ تحميل الكتب
    void on_actionDownloadBooks_triggered();
    //! تنفيذ تحميل كتاب
    void on_actionCreatNewBook_triggered();
    //! استراد ملفات نصية
    void on_action_import_triggered();
    //! كتب الشاملة
    void on_actionMdb_triggered();
    //! تنفيذ نافذة اظافة كتاب
    void on_action_addBoook_triggered();
    //! تصدير الكتب
    void on_actionExport_triggered();
    //! ضغط الكتاب
    void on_actionArchive_triggered();
    //! معلومات الكتاب الحالي
    void on_actionBookInfo_triggered();
    //! الطباعة
    void on_actionPrint_triggered();
    //! معاينة قبل الطباعة
    void on_actionPrintPreview_triggered();
    //! خروج
    void on_action_close_triggered();
    //**التنقل بين الصفحات**

    void on_action_first_triggered();/*!< تستدعي TabBook::firstPage() للذهاب الى الصفحة الاولى */

     void on_action_next_triggered();  /*!< تستدعي TabBook::nextPage() للذهاب الى الصفحة التالية */

    void on_action_prev_triggered(); /*!< تستدعي TabBook::prevPage() للذهاب الى الصفحة السابقة */

    void on_action_last_triggered();/*!< تستدعي TabBook::lastPage() للذهاب الى الصفحة الاخيرة */
    //! التنقل الى صفحة معينة
    void on_actionGotoPage_triggered();
    //! الذهاب الى التنقل التالي
    void historieNext();
    //! الذهاب الى التنقل السابق
    void historiePrev();
    //! تنفيذ البحث في مجموعة كتب
    void on_actionFind_all_triggered();

    //! زر وقائمة تفعيل التحرير
    void on_action_edit_triggered(bool checked);
    //! اضف للمفضلة
    void on_action_favorit_triggered();
    //! ارتباط تشعبي
    void on_actionHiperlink_triggered();
    //! ضبط روابط الفهرسة
    void on_actionOrganizeFahrasse_triggered();
    //! حذف الكتب الغير متوفرة
    void on_actionUpdatGroupRemove_triggered();
    //! تمييز الكتب الغير متوفرة
    void on_actionUpdatGroup_triggered();

    //! ملأ الشاشة
    void on_action_fulscreen_triggered();
    //! خيارات
    void on_actionOption_triggered();

    /**
     * @brief on_actionRemoveTechkil_triggered حذف التيكيل
     * @param checked موجب يتم الحذف
     */
    void on_actionRemoveTechkil_triggered(bool checked);
    //! تكبير النص
    void on_actionZoomOut_triggered();
    //! تصغير النص
    void on_actionZoumIn_triggered();
    //! صفحة البداية
    void setupPageIntro();
    //! حول البرنامج
    void on_action_about_triggered();
    //! التعليمات
    void on_actionHelp_triggered();
    //! تنفيذ زر ما هذا
    void on_actionWhatsThis_triggered();
    //! عند تغير التحديد في شجرة الكتب
    void on_treeWidget_books_itemSelectionChanged();
    //! تفعيل عنصر شجرة  الكتب
    void on_treeWidget_books_itemActivated(QTreeWidgetItem* item);
    //! انشاء قائمة لشجرة الكتب
    void on_treeWidget_books_customContextMenuRequested();
    //! تنفيذ قائمة فتح الكتاب
    void menuOpenBook();
    //! تنفيذ عملية حذف الكتاب
    void menu_BookRemove();

    //! تحرير بيانات الكتاب
    void menu_BookUpdat();
    //! معرفة مسار الكتاب
    void menu_BookPath();
    //! اعادة تسمية دليل الكتاب
    void menu_renameBook();
    //! اضافة مجموعة جديدة
    void menu_GroupeAdd();
    //! اضافة قسم جديد
    void menu_CategorieAdd();
    /**
     * @brief menu_Add اضافة قسم او مجموعة للشجرة
     * @param categorie اذا كان موجب يتم اضافة قسم والا مجموعة
     */
    void menu_Add(bool categorie);
    //! حذف المجموعة
    void menu_GroupeRemove();
    //! تسمية المجموعة
    void menu_GroupeRename();
    /**
     * @brief menu_downloadBook جلب كتاب من النت عند النقر على فتح كتاب
     * @param bkname معرف الكتاب
     */
    void menu_downloadBook(const QString &bkname);
    //! تفعيل عنصر المفضلة
    void on_treeWidget_fav_itemActivated(QTreeWidgetItem* item);
    //! اعادة تسمية المفصلة
    void on_actionFavItemRen_triggered();
    //! نقل عنصر المفضلة للاسفل
    void on_actionFavItemDown_triggered();
    //! نقل عنصر المفضلة للاعلى
    void on_actionFavItemUp_triggered();
    //! حذف عنثر المفضلة
    void on_actionFavDelete_triggered();
    //! حفظ المفضلة
    void on_actionFavSave_triggered();
    //! البحث في شجرة المفضلة
    void find_favo_textChanged(QString );

    //**treeVeiwCuran**
    //! تفعيل عنصر شجرة القرءان الكريم
    void on_treeWidget_curaan_itemActivated(QTreeWidgetItem* item);
    //! النقر على زر البحث في القرءان
    void on_pushButton_gofind_clicked();
    //! تغير النص في مربع نس البحث في شجرة القرءان
    void on_lineEdit_find_cur_textChanged(QString );
    /**
     * @brief on_comboBox_currentIndexChangedتحميل الاجزاء او السور
     * @param index 0 او 1
     *0 الاجزاء
     *1 السور
      */
    void on_comboBox_currentIndexChanged(int index);
    //! تحمبل كتاب التفسير
    void on_pushButton_gotafsir_clicked();
    //**treeViewFind**
    //! النقر على عنصر شجرة نتائج البحث
    void showResultFind(QTreeWidgetItem* item);
    //! حذف عنصر من شجرة البحث
    void on_actionFindDelete_triggered();
    //! نقل عنصر للاسفل
    void on_actionFindItemDown_triggered();
    //! نقل عنصر للاعلى
    void on_actionFindItemUp_triggered();
    //! فتح ملف بحث
    void on_actionFindLoad_triggered();
    //! حفظ نتائج البحث
    void on_actionFindSave_triggered();

    //**Animation**
    //! تغيير حركة الايقونات من ثلاثي الى ايقوني او العكس
    void on_actionTreeD_toggled(bool );
    //!  النقر على زر الكتب السابقة في مشهد الحركة
    void on_actionAnimHistorie_triggered();
    //! النقر على زر المجموعات في مشهد الايقونات الحركية
    void on_actionAnimGroup_triggered();
    //  void on_lineEdit_textChanged(QString );
    /*************************************
       *عمليات الالسن
       ***************************************/
    /**
     * @brief on_tabWidget_tabCloseRequested عند النقر على زﻻ غلق اللسان
     * @param index معرف اللسان
     */
    void on_tabWidget_tabCloseRequested(int index);
    /**
     * @brief on_tabWidget_currentChanged عند تغير اللسان
     * @param index معرف اللسان
     */
    void on_tabWidget_currentChanged(int index);
    /**
   * @brief tabCreatTabNew انشار لسان جديد
   */
    void tabCreatTabNew();
    /**
     * @brief tabOpenInNewTab فتح الكتاب في لسان جديد
     */
    void tabOpenInNewTab();
    /*************************************
     *عمليات الرواة
     ***************************************/
    void on_btnRowaInfo_clicked();
    void on_treeWidget_rowatFind_itemActivated(QTreeWidgetItem* item, int );
    void showfind(bool isrowat);
    void on_treeWidget_rowatTabia_itemActivated(QTreeWidgetItem* item, int );
    void openRawi(QTreeWidgetItem* item,bool addToTataboa);
    void on_treeWidget_rowat_itemActivated(QTreeWidgetItem* item, int );
    void on_lineEdit_FindRowat_textChanged(QString searchString);
    void on_comboBox_rowat_currentIndexChanged(int index);

    //-********functions**************

    //! فتح الكتب السابقة
    void openRecentFile();

    /**
     * @brief addToolRubonBar تكوين لوحة الادوات
     * @param rubon هل لوحة عادية ام لا
     */
    void addToolRubonBar(bool rubon);
    //! تحميل سمة الايقزنات
    void chargeIconsThemes();
    /**
     * @brief loadPlugin تحميل الاضافات
     * @param plug الاضافات
     * NET_PLUG 0
     * SHAMILA_PLUG 1
     * ROWAT_PLUG 2
     * @return موجب اذا تم التحميل
     */
    bool loadPlugin(int plug);
    /**
     * @brief printPreview معاينة الطباعة
     * @param printer
     */
    void printPreview(QPrinter *printer);

    /**
     * @brief open_dataBase فتح الكتاب
     * رسالة الى
     * TabBook::openDataBase()
     * @param name اسم الكتاب
     * @param title عنوان الكتاب
     * @param Autor المؤلف
     * @param position الموضع
     */
    void open_dataBase(const QString &name,const QString &title,const QString &Autor,int position=0);  //فتح الكتاب المحدد

    //! البحث في الكتاب الحالي
    void findInCurrentBook();              //البحث في الكتاب الحالي

    //! البحث عن مؤلف او كتاب في الشجرة
    void findBookOrHautor();                    // مؤلف البحث عن كتاب
    //! البحث في شجرة الفهرسة
    void searchInTreeFah(QString );//
    //! البحث في شجرة الكتب
    void searchInTreeBook(const QString &);        //
    //! توقيف عملية البحث
    void stopFind();
    //! البحث في الصفحة الحالية
    void searchIndoc();                 //
    //! تحميل شجرة الكتب
    void chargeGroupe();
    //! تحميل الادوات والايقونات
    void updateIconEndAction();
    void setupActionsPdf();
    //عمليات السمات والموضع والاحجام
    //! حفظ حجم و موضع النافذة و الادوات
    void saveLayou();
    //!  تحميل حجم و موضع النافذة و الادوات
    void loadLayout();
    //!  تحميل بيانات المستخدم
    void loadSettings();
    //عملية تحويل النصوص
    //! امرار المؤشر على ارتباط في النص
    void textBookHighlighted(QUrl );
    //! النقر على ارتباط في النص
    void textBookAnchorClicked(QUrl );

    //! جلب نافذة تحويل قرص الشاملة
    void on_actionShamilaCdrom_triggered();
    //! جلب نافذة لوحة التحكم
    void on_actionControle_triggered();
    //! جلب نافذة تحميل قرص القرطاس
    void on_actionDvd_triggered();
    //!  جلب نافذة تخصيص ادوات لوحة الادوات
    void on_actionCostumActions_triggered();

};

#endif // MAINWINDOW_H
