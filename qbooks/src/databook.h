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
#ifndef DATABOOK_H
#define DATABOOK_H
#include <QObject>
#include <QDomDocument>
#include <QtGui>

/**
 * @brief The databook class قاعدة التعامل مع بيانات الكتاب
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class databook : public QObject
{
    Q_OBJECT

public:

    databook();
    ~databook();
    //! الصفحة الحالية
    QString currentPage()const {return m_bookPage;}
    //! الجزء الحالي
    QString currentPart()const {return m_bookPart;}
    //! النص الحالي
    QString currentNass()const {return m_bookNass;}
    //! الموضع الحالي للتسجيل
    int currentPosition()const{return m_curPos;}
    //! عدد التسجيلات في الكتاب
    int count()const {return m_rowsCount;}
    //! هل الكتاب تفسير للقرءان الكريم
    bool isTefsir()const {return m_isTefsir;}
    /**
     * @brief removeTechkile حذف التشكيل
     * @param remove نعم ام لا
     */
    void removeTechkile(const bool &remove){m_techkile=remove;}

public slots:
    /**
     * @brief openBook فتح الكتاب
     * @param bkpath مسار الكتاب
     * @param position الموضع
     * @return نعم اذا تم الفتح او لا
     */
    bool openBook(const QString &bkpath, int position=0);
    //! الذهاب الى موضع محدد
    void moveToPosition(int position);
    /**
     * @brief findAya البحث عن اية في القرءان الكريم
     * @param soura رقم السورة
     * @param aya رقم الاية
     * @return  هل تم البحث ايجابيا ام سالبيا
     */
    bool findAya(const QString &soura, const QString &aya);
    /**
     * @brief updatPage تحديث الصفحة في حال تغيرها من طرف المستخدم
     * @param oldText النص الذي سيحول
     * @param newText النص الجديد
     * @param fullText هل كل النص في الصفحة ام بعضه
     */
    void updatPage(const QString &oldText, const QString &newText, bool fullText);
    /**
     * @brief saveBook حفظ الكتاب
     * @param path مسار الحفظ
     * @return  ايجابي اذا تم الحفظ
     */
    bool saveBook(const QString &path);
    //! مسح البيانات في حال فتح كتاب اخر
    void claerBook();
    /**
     * @brief insertPage ادراج صفحة
     * @param after  هل بعد الحالية
     */
    void insertPage(bool after);
    //! حذف الصفحة
    void removeCurPage();
    /**
     * @brief treeOrganizFahrass ترتيب الفهرسة بحيث تناسب مواضع الصفحات المضمنة
     *وهذا في حالة كتب الشاملة او ان المواضع خاطئة
     * @param view الشجرة المستخدفة
     * @param Bname مسار الكتاب
     */
    void treeOrganizFahrass(QTreeWidget *view, const QString &Bpath);

private slots:
    /**
     * @brief getIfTefsir هل الكتاب تفسير للقرءان الكريم
     * @return نغم اذا كان تفسير
     */
    bool getIfTefsir();
    /**
     * @brief noTechkile حذف التشكيل من النص
     * @param text النص المشكل
     * @return نص غير مشكل
     */
    QString noTechkile(const QString &text) const;
    /**
     * @brief getItemLast جلب العنصرالاخير في الشجرة
     * @param item العنصر الاب ان كان ابا
     * @return العنصر الابن الاخير
     */
    QTreeWidgetItem* getItemLast(QTreeWidgetItem *item);
    bool chargeList();
    int getPositionID(const QString &id);
private:
    QString m_bookNass;          /*!< نص الصفحة */
    QString m_bookPart;          /*!< الجزء */
    QString m_bookPage;          /*!< الصفحة */
    QString m_bookCurentId;      /*!< معرف الكتاب */

    bool m_isTefsir;             /*!< هل الكتاب تفسير للقرءان الكريم */
    bool m_techkile;             /*!< خل يسمح بالتشكيل */
    int m_curPos;                /*!< الموضع الحالي في الكتاب */
    int m_rowsCount;             /*!< عدد السجلات في الكتاب */

    QDomDocument *m_docBooK;      /*!< وثيقة بيانات الكتاب */

    QList<QString> listId;

};
#endif // DATABOOK_H
