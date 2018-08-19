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
#ifndef DIALOGCONVERT_H
#define DIALOGCONVERT_H
#include <QDialog>

 #include <QTextEdit>
 #include <QDomDocument>
namespace Ui
{
    class dialogconvert;
}

/**
 * @brief The dialogconvert class تحويل ملفات نصية الى كتاب
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
*/
class dialogconvert : public QDialog
{
    Q_OBJECT

public:
    dialogconvert(QWidget *parent = 0);
    ~dialogconvert();
    QString m_pathCostum; /*!< مسار الكتب المخصص */
private:
    Ui::dialogconvert *ui;
    /**
     * @brief creat_xmlfile انشاء ملفات الكتاب
     * @param path المسار
     * @return موجب ان تم انشاء الملفات
     */
    bool creat_xmlfile(QString path);
    /**
     * @brief load فتح الملف
     * @param f مسار الملف
     * @return موجب ان تم فتح الملف
     */
    bool load(const QString &f);
    /**
     * @brief creat_dir انشاء مجلد الكتاب
     * @return  موجب ان تم انشاء المجلد
     */
    bool creat_dir();
    /**
     * @brief ecrirexml  كتابة الملفات الى وثيقة اكس مل ليتم حفضها
     * @param jozaa الجزء
     * @return موجب ان تم كتابتها
     */
    bool ecrirexml(int jozaa);

    QTextEdit m_textEdit;       /*!< ادات النص الذي ستم فتحه */
    QDomDocument m_docBook;     /*!< وثيقة اكس مل التي سيتم انشاؤها من النص */
    QString m_newPath;          /*!< مسار الكتاب الذي سيتم انشاؤه */
    QString m_newPathDir;       /*!< اسم مجلد الكتاب الجديد */

    QString m_addGroupeId;      /*!< معرف المجموعة التي سيضاف اليها */
    int creatId;                /*!< المعرف الوحيد الذي يتم انشاؤه */
public slots:


private slots:

    void on_toolButton_down_clicked();                      /*!< خفظ الملف للاسفل */
    void on_toolButton_up_clicked();                        /*!< رفع الملف للاعلى */
    void on_toolButton_delete_clicked();                    /*!< حذف الملف */
    void on_comboBox_group_currentIndexChanged(int index);  /*!< تغير المجموعة في القائمة المنسدلة */
    void on_toolButton_fileNam_clicked();                   /*!< فتح نافذة اضافة ملفات */
    void on_pushButtonOk_clicked();                         /*!< النقر على زر موافق */


};
#endif // DIALOGCONVERT_H
