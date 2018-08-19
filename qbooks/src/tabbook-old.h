#ifndef TABBOOK_H
#define TABBOOK_H
#include "databook.h"
#include "tooledite.h"
#include "pdfwidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QTextBrowser>

/**
 * @brief The TabBook class خاص بكل كتاب وما يحتاجه من ادوات ومعلومات
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
*/
class TabBook : public QWidget
{
    Q_OBJECT
public:
    explicit TabBook(QWidget *parent=0);
    ~TabBook();
    QTextBrowser *txtBrowserBook ;
    QTreeWidget  *treeViewFahras;
    QWidget *widgetFahras;
    /**
     * @brief getTitle
     * @return اسم الكتاب
     */
    QString getTitle()const{return m_title;}
    /**
     * @brief getAuthor
     * @return اسم المؤلف
     */
    QString getAuthor()const{return m_author;}
    /**
     * @brief getBetaka
     * @return بطاقة الكتاب
     */
    QString getBetaka()const{return m_betaka;}
    /**
     * @brief getBookName
     * @return غنوان الكتاب الوحيد
     */
    QString getBookName()const{return m_bookName;}
    /**
     * @brief getBookPath
     * @return مسار الكتاب
     */
    QString getBookPath()const{return m_bookPath;}
    /**
     * @brief geBookPos
     * @return ارجاع موضع الكتاب الحالي
     */
    int getBookPos()const;
    /**
     * @brief getRowCount عدد السجلات في الكتاب
     * @return  العدد
     */
    int getRowCount()const;
    /**
     * @brief getBookIsTefsir
     * @return هل الكتاب كتب تفسير
     */
    bool getBookIsTefsir()const{return m_isTefsir;}
    /**
     * @brief setBookIsTefsir تعليم الكتاب هل هو تفسير
     * @param arg
     */
    void setBookIsTefsir(bool arg){m_isTefsir=arg;}
    /**
     * @brief getIsEditable مغرفة هل الكتاب في وضع التحرير
     * لازمة لتفعيل زر التشكيل اوعدمه
     * @return
     */
    bool getIsEditable()const{return m_isEditable;}
    /**
     * @brief getIstechkileRemoved معرفة هل نزع التشكيل مفعل في الكتاب
     * @return موجب ان تم نزع التشكيل
     */
    bool getIstechkileRemoved()const{return m_isRemoveTechkile;}
    //! ارجاع هل الكتاب متاح
    bool getIsBook(){return m_isBook;}
    //! هل بيدياف متاح
    bool getIsPdf(){return m_isPdf;}


protected:
    //! تغيير الحج اذا كان ملف بيديف على حجم الصفحة
    void resizeEvent ( QResizeEvent *  ) {fitInwidthPdf();}


signals:
    /**
     * @brief bookAvailable  اشارةهل الكتاب متاح
     */
    void bookAvailable(bool);
    /**
     * @brief bookAvailable  اشارة عندما يكون بيدي اف متاح
     */
    void pdfAvailable();
    //! اشارة الى ان المعلومات تغيرت
    void bookInfoChanged();
    //! اشارة الى عدم وجود صفحة تنقل سابقة
    void prevHistorieEnabled(bool);
    //! اشارة الى عدم وجود صفحة تنقل تالية
    void nextHistorieEnabled(bool);
    //! اشارة تغير الجزء والصفحة
    void pagePartChanged(const QString &);
    /**
     * @brief settingsChanged تغير الاعدادات
     *تتاح من النافذة الرئيسية
     */
    void settingsChanged();


public slots:
    /**
     * @brief openDataBase فتح الكتاب
     * @param name عنوان الكتاب الوحيد
     * @param title اسم الكتب
     * @param Autor اسم المؤلف
     * @param position الموضع الذي سيتم فتحه افتراضيا 0
     */
    bool  openDataBase(const QString &name,const QString &title,const QString &Autor,int position=0) ;        //فتح الكتاب
    void firstPage();       /*!< للذهاب الى الصفحة الاولى */
    void nextPage();        /*!< تلذهاب الى الصفحة التالية */
    void prevPage();        /*!< للذهاب الى الصفحة السابقة */
    void lastPage();        /*!< للذهاب الى الصفحة الاخيرة */
    void historiePrev();    /*!< للذهاب الى التنقل السابق  */
    void historieNext();   /*!< للذهاب الى التنقل التالي  */

    /**
     * @brief gotoPageDlg نافذة الذهاب الى صفحة معينة او موضع
     */
    void gotoPageDlg();
    /**
     * @brief gotoPageDlg  الذهاب الى صفحة معينة
     */
    void gotoPage(int pos);
    /**
     * @brief gotoSoraAya الذهاب الى سورة واية معينة
     * @param mysora رقم الصورة
     * @param myaya رقم الاية
     * @return موجب او سالب
     */
    bool gotoSoraAya(const QString &mysora,const QString &myaya);
    /**
    * @brief updatPage تغيير مص في الصفحة
    * @param oldText النص السابق
    * @param newText التص الحالي
    * @param fullText هل كل النص
    */
    void updatPage(const QString &oldText,const QString &newText,const bool fullText);
    /**
     * @brief organizFahras تجديث روابط الفهرية لتتناسب مع موضع الصفحة
     */
    void organizFahras();
    /**
     * @brief removeTechkil حذف التشكيل
     * @param arg نعم او لا
     */
    void removeTechkil(const bool arg);
    /**
     * @brief closeRequested معرفة اذا كان هناك تغير عند الخروج
     * @return
     */
    bool closeRequested();
    /**
    * @brief setEditBook اظهار ادوات التحرير
    * @param edit true or false
    */
    void setEditBook(const bool edit);
    void zoomOut();
    void zoomIn();
    void serchInPdf(const QString &txt); //! البحث في مل بي دي اف
    void updateSignals();//! تحديث الاشارات عند تغسر الالسن


private slots:
    void setPdfPage(int index);
    void loadSettings();                    /*!< تحميل اعدادات المستخدم */
    void setupTools();                      /*!< تكوين الادوات */
    void customContextMenu();           /*!< القائمة المنسدلة لمربع نص الصفحة */
    void textBookHighlighted(QUrl );
    void textBookAnchorClicked(QUrl );
    /**
     * @brief ceatCadre انشاء اطار النص المزخرف
     * @param cadreFolder مجلد الزخرفة
     * @param isCadre هل سيتم تفعيل الزخرفة
     */
    void ceatCadre(const QString &cadreFolder, bool isCadre=true);
    /**
     * @brief bookValueChanged تغير موضع الصفحة
     * @param pos الموضع
     */
    void bookValueChanged(int pos);


    void fahrasItemClicked(QTreeWidgetItem* item);
    /**
    * @brief convertTextToHtml تحويل النص حصب بيانات المستخدم من الالوان والخطوط
    * @param txt النص الذي سيتم تحويله
    */
    void convertTextToHtml(const QString &txt);
    /**
  * @brief closeMessages  سؤال هل يتم حفظ الكتاب والفهرسة ام لا
  * @return ايجابي ان تم الحفظ او العكس
  */
    bool closeMessages();

    // EDITE
    void itemFahrasAdd();                   /*!< اضافة عنصر جديد بعد المحدد الى الفهرسة */
    void itemFahrasAddChild();              /*!< اضافة عنصر ابن للعنصر المحدد الى الفهرسة */
    /**
     * @brief addItemInTreeFahras اضافة عنصر  لى الفهرسة
     * @param child اذا كان موجبا يتم استدعاء itemFahrasAddChild()
     * اما اذا كان سالبا ستم استدعاء itemFahrasAdd()
     */
    void addItemInTreeFahras(bool child);
    /**
     * @brief saveBookEndFahras  حفظ الفهرسة ونصوص الكتاب باستدعاء
     * saveFahrass() و saveCurrentBook()
     */
    void saveBookEndFahras();               /*!< حفظ الفهرسة */
    /**
     * @brief saveFahrass حفظ الفهرسىة بواسطة
     * Utils::fahrasSave(QTreeWidget *view, const QString &bkname, const QString &pathCostum)
     */
    void saveFahrass();
    void saveCurrentBook();                 /*!< حفظ نصوص الكتاب databook::saveBook(const QString &path) */
    void itemFahrasRename();                /*!< اعادة تسمية غنصر الفهرسة */
    void itemFahrasRemove();                /*!< حذف عنصر الفهرسة */
    void itemFahrasMoveDown();              /*!< نقل العنصر للاسفل */
    void itemFahrasMoveUp();                /*!< نقل العنصر للاسفل */
    /**
     * @brief goFindFah انشاء فهارس جديدة بواسطة البحث في النصوص عن معاملات
     * @param rx نص الميتوى الاول
     * @param rx2 نص المستةى الثاني
     * يتم استدعاء
     * Utils::findFahariss()
     */
    void goFindFah(QRegExp rx,QRegExp rx2);
    void newPageAfter();                /*!< صفحة جديدة بعد الحالية */
    void newPageBefort();               /*!< صفحة جديدة قبل الحالية */
    void removePage();                  /*!< حذف الصفحة */
    void fontCostum();                  /*!< ادراج خط مخصص */

    void fitInwidthPdf();//! تحجيم ملف بي دي اف الى عرص الصفحة
    void actionFitInwidth(bool checked); //! fitInwidthPdf()
    void scrollByMouse(int x,int y);

private:
  // متغيرات ذات قيمة مبدئية =0
    ToolEdite *toolEdite;  //! ادوات التحرير
    bool m_isTefsir;                /*!< هل كتاب التفسير يحتوي على فهرسة الايات والسور */
    bool m_FahrasIsModified;        /*!< هل تم تغيير الفهرسة */
    bool m_PageIsModified;          /*!< هل تم تغيير الصفحات */
    bool m_isEditable;              /*!<  هل في حالة التحرير */
    bool m_isPdf;                 /*!< هل الكتاب من نوع بيدياف */
    bool m_isBook;
    bool m_isRemoveTechkile;
    bool m_fitInwidth;

   //اوامر
    QAction *mActionFitInwidth;    //! fitInwidthPdf()
    QAction *mActionNext;          //! nextPage()
    QAction *mActionPrev;          //! prevPage()
    QAction *mActionFirst;         //! firstPage()
    QAction *mActionLast;          //!  lastPage()
    //
    PdfWidget   *pdfWidget ;        //! اداة كتاب من نوع بي دي اف
    databook  *DataBook;            //! قاعدو بيانات الكتاب
    //متغيرات يتحكم فيها المستخدم
    QString m_WebColorBack;          /*!< لون خلفية النص */
    QString m_WebFont;              /*!< نوع النص */
    QString m_WebFontTitle;         /*!< خط العناوين */
    QString m_WebFontTitleSize;     /*!< حجم خط العناوين */
    QString m_WebFontHachia;        /*!< خط الحاشية */
    QString m_WebFontKhase;         /*!<  حط مخصص */
    QString m_WebFontColor;         /*!< لون النص العام */
    QString m_WebFontTitleColor;    /*!< لون خط العناوين */
    QString m_WebFontFindColor;     /*!<  لون خط البحث */
    QString m_WebFontPrefertColor;  /*!< لون الخط الخاص */
    QString m_pathCostm;            /*!< مسار المخصص للكتب */

    int m_arabicNmber;              /*!<  الرقام عربية=1 */


    //متغيرات الكتاب
    QString m_pathApp;              /*!< مسار التطبيق */
    QString m_bookPath;             /*!<  مسار الكتاب الحالي */
    QString m_pathUser;             /*!< مسار المستخدم */
    QString m_bookName;             /*!< اسم مجلد الكتاب */
    QString m_title;                /*!<  عنوان الكتاب */
    QString m_author;               /*!< المؤلف */
    QString m_betaka;               /*!< بطاقة الكتاب */

    QList<int> m_listHistorieNext;  /*!< قائمة التنقلات التالية */
    QList<int> m_listHistoriePrev;  /*!< قائمة التنقلات السابقة */

    //
    QList<QWidget *>m_listFrames;   /*!< قائمة بودجات الصور غلى الاطار */
    //ادوات البرنامج امتعددة
    QGridLayout *gridLayout;
    QVBoxLayout *vLayoutFahras ;
    QVBoxLayout *hLayoutPage;

    QStackedWidget *stackedWidget ;


    QWidget * pageBook;//! اداة حمل لكتاب
    QWidget *pagePdf ;//! اداة حمل بيدي اف
 QScrollArea   *scrollArea;
    qreal scalPdf;
    qreal pdfWidth;
};

#endif // TABBOOK_H
