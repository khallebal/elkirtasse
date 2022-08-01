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
#ifndef ROWAT_H
#define ROWAT_H
#include <QTreeWidget>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QProgressBar>
#include <QTreeWidget>
#include <QFile>
#include <QtPlugin>
#include "src/booksInterface.h"

/**
 * @brief The rowat class هذه الفئة خاصة برواة التهذيب
 */
class rowat : public QObject, RowatInterface
{
    Q_OBJECT
    Q_INTERFACES(RowatInterface)
public:


private:
//! ملف بكل الرواة
    QDomDocument docRowat;
    //! المتغيرات من نوع نص
    QString name,TABAQA,WHO,AQUAL,ROTBA,R_ZAHBI,sheok,telmez,birth,death;
public slots:

    /**
     * @brief treeChargeRowtName تحميل شجة بكل الرواة
     * @param view الشجرة المستهدفة
     */
    void treeChargeRowtName(QTreeWidget *view);
    /**
     * @brief chargeRowat تحميل الرواة على ملف xml
     * @return  يعود باليجاب اذا تم التحميل
     */
    bool chargeRowat();
    /**
     * @brief moveToPosition الانتقال الى موضع من الملف
     * @param position الموضع
     * @return يعيد ملف اش تي ام ال بكل المعلومات
     */
    QString moveToPosition(int position);
    QString readxml(int pos);
    /**
     * @brief rawiFind البحث عن راي
     * @param rawi الاسم
     * @param viewS الشجرة الاصل
     * @param viewD الشجرة المستهدفة بالكتابة
     */
    void  rawiFind(QString rawi,QTreeWidget *viewS,QTreeWidget *viewD);
    /**
     * @brief clearData مسح كل الملف
     */
    void clearData();
private slots:
    /**
     * @brief creatHtml انشاء ملف
     * @return ارجاع الملف بصيغة اش تي مل
     */
    QString creatHtml();
    /**
     * @brief fullRawi البحث عن اسماء رواة كاملة
     * @param rawi اسم الراوي مثلا البخاري
     * @return   الاسم الكامل مثلا محمد بن اسماعيل البخاري
     */
    QString fullRawi(QString rawi);
    /**
     * @brief rawiFindAbou البحث عن الراوي اذا كان في اسمه ابو او ابي
     * @param rawi ايم الراوي
     * @param viewS الشجرة الاصل
     * @param viewD الشجرة المستهدفة بالنتائج
     * @return النتيجة
     */
    bool  rawiFindAbou(QString rawi,QTreeWidget *viewS,QTreeWidget *viewD);
};
#endif // ROWAT_H
