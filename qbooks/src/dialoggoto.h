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
#ifndef DIALOGGOTO_H
#define DIALOGGOTO_H
#include <QDialog>
#include <QXmlStreamReader>
#include <QAbstractButton>
namespace Ui
{
    class dialoggoto;
}

/**
 * @brief The dialoggoto class نافذة للذهاب الى موضع محدد في الكتاب
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class dialoggoto : public QDialog
{
    Q_OBJECT

public:
    dialoggoto(QWidget *parent = 0);
    ~dialoggoto();
    QString pathBook;       /*!< مسار الكتاب المخصص */
    int positon;            /*!< الموضع المحدد */
    int rowCount;           /*!< عدد سجلات الكتاب */

private:
    Ui::dialoggoto *ui;

QXmlStreamReader xml;
private slots:
/**
 * @brief readXmlBook قراءة ملف اكس مل للبحث عن جزء او صفحة
 * @param spinPart الجزء محدد في اداة الجزء
 * @param spinPage الصفحة محددة في اداة الصفحة
 * @return موجب ان وجد الجزء والصفحة
 */
bool readXmlBook(QString spinPart,QString spinPage);

/**
     * @brief on_buttonBox_clicked النقر على ازرار النافذة
     * @param button نوع الزر موافق ام الغاء
     */
    void on_buttonBox_clicked(QAbstractButton* button);
public slots:
    /**
     * @brief updateValue تحديث الادوات
     */
    void updateValue();
};
#endif // DIALOGGOTO_H
