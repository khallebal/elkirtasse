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
#ifndef NETWORKPLUGIN_H
#define NETWORKPLUGIN_H
#include <QObject>
#include "src/booksInterface.h"
#include "dialognet.h"


/**
 * @brief The networkplugin class فئة خاصة بتحميل الكتب
 */
class networkplugin : public QObject, NetInterface
{
    Q_OBJECT
    Q_INTERFACES(NetInterface)
public:

private:

public slots:
    //! تنفيذ نافذة التحميل للاهتيار
    QString execPlugin();
    /**
   * @brief loadFile تنفيذ نافذة التحميل مع التحميل مباشرة
   * @param urlfile عنوان الكتاب
   * @return ميار الكتاب على الجهاز
   */
    QString loadFile(const QString &urlfile);
    QString pluginTitle();


private slots:


};
#endif // NETWORKPLUGIN_H
