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
#ifndef DIALOGADDBOOKS_H
#define DIALOGADDBOOKS_H
#include <QDialog>
#include <QFileDialog>
#include <QAbstractButton>

namespace Ui
{
    class DialogAddBooks;
}
/**
 * @brief The DialogAddBooks class فئة خاصة باضافة كتب الى المكتبة
 *  \author    أبو زكريا
 *  \version   3.7.0
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class DialogAddBooks : public QDialog
{
    Q_OBJECT

public:
    DialogAddBooks(QWidget *parent = 0);
    ~DialogAddBooks();
    QString m_pathCostm;

private:
    Ui::DialogAddBooks *ui;
    QString m_addGroupeName;    /*!< المجموعة التي سيوضع فيها الكتاب */
    QString m_addGroupeId;      /*!< معرف المجموعة */

    QString m_bookDir;          /*!< مجلد الكتاب */
    QString m_newPathDir;       /*!< المسار الجديد للكتاب */

    QString Add_Book_Title;      /*!< عنوان الكتاب */
    QString Add_Autor_Name;      /*!< مؤلف الكتاب */
    QString Add_Betaka;          /*!< بطاقة الكتاب */
    QString msgTitle;            /*!< الرسالة التي ستعرض في حال النهاية */

private slots:
    void on_toolButtonInfo_clicked();               /*!< زر عرض معلومة عن الكتاب */
    void on_toolButton_delete_clicked();            /*!< زر حذف المسار من القائمة */
    /**
     * @brief on_buttonBox_clicked النقر على ازرار النافذة
     * @param button نوع الرز هل هو نعم ان الغاء
     */
    void on_buttonBox_clicked(QAbstractButton* button);


    void on_toolButton_zipFileNam_clicked();    /*!< فتح نافذة اختيار ملف مضغوط */
    void on_toolButton_fileNam_clicked();       /*!< فتح نافذة اختيار مجلد */

    void on_comboBox_group_currentIndexChanged(int index);  /*!< غند تغير المجموعة في القائمة المنسدلة */
    bool  creat_dir();       /*!< انشاء مجلد الكتاب */
    /**
     * @brief copyDir نسخ المجلد الى وجهة مسار المستخدم
     * @param filname المسار
     * @return موجب ان تم الحفظ
     */
    bool copyDir(QString filname,int );
    /**
  * @brief loadBookInfo جلب معلومات الكتاب
  * @param path المسار
  */
    void loadBookInfo(QString path);
    bool copyPdfFile(const QString &filname);
    bool loadPdfPlugin(const QString &filname);
public slots:

};
#endif // DIALOGADDBOOKS_H
