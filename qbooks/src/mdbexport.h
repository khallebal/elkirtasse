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
#ifndef MDBEXPORT_H
#define MDBEXPORT_H
#include <QDialog>

#include <QAbstractButton>
#include <QXmlStreamWriter>

namespace Ui
{
    class mdbexport;
}
/**
 * @brief The mdbexport class لتحويل كتب الشاملة الى صيغة المكتبة خاصة  بلينوكس وانضمة اكس
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class mdbexport : public QDialog
{
    Q_OBJECT

public:
    mdbexport(QWidget *parent = 0);
    ~mdbexport();
    QString m_path;

private:
    Ui::mdbexport *ui;
    QList<QString> listId;
    QString tableBook;
    QString tableTitle;

    QString m_bookTitle;
    QString m_bookBitaka;
    QString m_bookAuthor;
 QString m_addGroupeName;
    QString m_tempDir;
    QString msgTitle;
private slots:

void on_toolButton_delet_clicked();
void on_toolButtonInfo_clicked();
void criatBooks();
bool readMain(QString mdbSource);
bool creatmdbTempFiles(QString mdbSource);

    bool creatXmlFile(QString f,QString table,QString csv);
    bool creatInfo(QString bookdir);
    bool creatBash(QString fn,QString asbok);
   QString creatBooKDir();

 void on_comboBox_currentIndexChanged(int index);
    void on_buttonBox_clicked(QAbstractButton* button);
    void on_toolButton_fileNam_clicked();
    //--------------------ebook------------
    void criateEbookContent();
    void criateEbookToc();
};


#endif // MDBEXPORT_H
