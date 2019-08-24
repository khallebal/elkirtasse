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
#ifndef FINDBOOK_H
#define FINDBOOK_H
#include <QObject>
#include <QLabel>
#include <QAction>
#include <QApplication>
#include <QProgressBar>
#include <QProgressDialog>
#include <QTreeWidget>
#include <QDomDocument>
#include <QXmlStreamReader>

class QProgressBar;
class QAction;
class QLabel;
class QTextDocument;
class QPushButton;

/**
 * @brief The findbook class عمليات البحث في الكتب و شجرات الكتب وغيرها
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
*/
class findbook : public QObject
{
    Q_OBJECT

public:


    findbook();
    ~findbook();


    QTreeWidget *treeView;
    QLabel *labelProgress;
    QLabel *label_progressImg;
    QProgressBar *progressBar1;
    QAction *buttonStop;
    QString  findText;
    QString  findTitle;
    QString findAuthor;
    QString findName;
    bool isTefsir;                  /*!< هل الكتاب تفسير للقرءان الكريم */
bool isNassToFind;             /*!< هل سيتم البحث في النصوص او الفهاري */
     QString pathCostum;             /*!< ميار الكتب عند المستخدم */
    int resultCount;                /*!< عدد النتائج */
   // int getFind;
    bool isFindRuned;
    bool findToList;                /*!< هل يتم البحث حاليا */
    bool findFirst;                 /*!< البحث عن اول النص في الصفحة */
    bool stopFind;                  /*!< توقيف عملية البحث */
    bool findMultiFind;             /*!< بحث متعدد */
    bool findAnd;                   /*!< بحث عن نص و نص او نص او نص */
    bool findSawabik;               /*!< البحث بالسوابق */
    bool noHamza;                   /*!< تجاهل الهمزة */
private:
    int m_listbkfind;
    QList<QString> m_listStrToFind;

    QString m_pathUser;

    int m_progresseValue;

    QXmlStreamReader xml;

    QDomDocument m_docBKfindlist;                   //وثيقة قائمة الكتب للبحث

    QString str;
    QString id;
    QString page;
    QString part;
public slots:
    /**
     * @brief findOneBook البحث في كتاب واحد
     * @param bookpath مسار الكتاب
     */
    void findOneBook(QString bookpath);
    /**
     * @brief findInAllBook البحث في كل الكتب
     * @param inPages في الصفحات ام الفهارس
     */
    void findInAllBook(bool inPages);
    /**
 * @brief findInAllFahariss البحث في الفهارس
 * @param bookpath مسار الكتاب
 */
void findInAllFahariss(QString bookpath);
/**
 * @brief findFahariss البحث عن فهارس في الكتاب هذا الامر متاح عند تحرير كتاب لانشاء فهاري
 * @param view الشجرة المستهدفة بالنتائج
 * @param bookpath مسار الكتاب
 * @param str1 النص الاول
 * @param str2 النص الثاني
 * @param rowcount عدد السجلات لاستخدامها في مؤشر التقدم
 */
void findFahariss(QTreeWidget *view,QString bookpath,QRegExp str1,QRegExp str2,int rowcount);
//*****************************
/**
 * @brief searchInDoc البحث في الصفحة الحالية
 * @param document الوثيقى المستهدفة
 * @param searchString نص البحث
 * @param color لون البحث
 * @return ايجابي ان تم وجود النص
 */
bool searchInDoc(QTextDocument *document,QString searchString,QColor color);
/**
 * @brief searchInTreeview البحث في اي شجرة عن نص
 * @param view الشجرة المستهدفة
 * @param searchString النص
 * @param colum العمود الذي ستم البحث فيه
 */
void searchInTreeview(QTreeWidget *view,QString searchString,int colum);

/**
     * @brief loadResultFind تحكيل نتائج بحث سابقة
     * @param view الشجرة المستهدفة
     * @param fn مسار الملف
     */
    void loadResultFind(QTreeWidget *view,QString fn);
    /**
     * @brief saveResultFind حفظ نتائج البحث
     * @param view الشجرة المستهدفة
     * @param fn مسار الحفظ
     * @return موجب ان تم الحفظ
     */
    bool saveResultFind(QTreeWidget *view,QString fn);

private slots:
void readStrFirst();
    void readXml();
    void readStrMultiFind();
    void readStr();
    bool chargelistStrToFind(QString search);
    bool chargelistStrMultiFind(QString search);
    bool showAllItems( QTreeWidgetItem* parent ,QTreeWidget *view) ;
    bool searchTreeForString( const QString &searchString, QTreeWidgetItem* parent ,bool  itemtop,int  topindex,QTreeWidget *view,int colum) ;


};
#endif // FINDBOOK_H
