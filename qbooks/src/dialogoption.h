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
#ifndef DIALOGOPTION_H
#define DIALOGOPTION_H
#include <QDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QStyleFactory>

//#include <QAbstractButton>
namespace Ui
{
    class Dialogoption;
}

/**
 * @brief The Dialogoption class خيارات البرنامج
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
*/
class Dialogoption : public QDialog
{
    Q_OBJECT

public:
    Dialogoption(QWidget *parent = 0);
    ~Dialogoption();


private:
    Ui::Dialogoption *ui;


    QString m_btnStyle;     /*!< سمة زر الالوان */

    QString colorBack;      /*!< لون الخلفية */
    QString colorFont;      /*!< لون الخط */

    QString colorPrefer;    /*!< اللون الخاص */
    QString colorTitle;     /*!< لون العناوين */
    QString colorFind;      /*!< لون البحث */
    QFont m_fontName;       /*!< خط الصفحة */
    QString pathCostm;      /*!< مسار الكتب المخصص */


private slots:
    void loadSettings();                /*!< نحكيل الاعدادات */
    void saveSettings();                /*!< حفظ الاعدادات */
    void on_toolButtonkhass_clicked();  /*!< اختيار خط خاص */

    void on_toolButtonHachia_clicked();  /*!< اختيار خط الحاشية */
    void on_toolButtonTitle_clicked();      /*!< اختيار خط العناوين */
    void on_toolButton_costmPath_clicked(); /*!< اختيار خط مخصص */
    void on_toolButton_clicked();          /*!< اختيار خط الصفحة */

    void on_buttonBox_accepted();


    void on_pushButton_backcolor_clicked(); /*!< اختيار لون الخلفية */
    void on_pushButton_findcolor_clicked();     /*!< اختيار لون البحث */
    void on_pushButton_prefercolor_clicked();   /*!< اختيار لون خاص */
    void on_pushButton_titlecolor_clicked();    /*!< اختيار لون العناوين */
    void on_pushButton_fontcolor_clicked();     /*!< اختيار لون الخط العام */


 //   void on_checkBoxStandardTool_clicked();

    void on_toolButtonStyleCostum_clicked();



public slots:

};
#endif // DIALOGOPTION_H
