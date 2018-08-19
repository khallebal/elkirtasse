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
#ifndef DIALOGMDB_H
#define DIALOGMDB_H
#include <QDialog>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QAbstractButton>

namespace Ui
{
    class Dialogmdb;
}

/**
 * @brief The Dialogmdb class لتحويل كتب الشاملة الى صيغة المكتبة خاصة بويندوز
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class Dialogmdb : public QDialog
{
    Q_OBJECT

public:
    Dialogmdb(QWidget *parent = 0);
    ~Dialogmdb();
 QString m_path;
private:
    Ui::Dialogmdb *ui;
    QString Add_Book_Name;
    QString Add_Autor_Name ;
    QString Add_Betaka;
    QString m_addGroupeName;
    QString m_addGroupeId;
    QString m_bookDir;
    QString m_newPath;
     QString msgTitle;
    QSqlDatabase m_db;
QList<QString> listId;
QString tableBook;
QString tableTitle;
private slots:

void on_toolButtonInfo_clicked();
void on_toolButtonDelete_clicked();
void creatBook(QString fn);
bool creatDir();
    void on_buttonBox_clicked(QAbstractButton* button);

    void on_comboBox_currentIndexChanged(int index);
    void on_toolButton_fileNam_clicked();
};

#endif // DIALOGMDB_H
