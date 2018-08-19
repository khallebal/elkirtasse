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
#ifndef BOOKSINTERFACE_H
#define BOOKSINTERFACE_H
#include <QObject>
#include <QtGui>


/**
 * @brief The NetInterface class فئة خاصة بالتعامل مع التحميل
 */
class NetInterface
{
public:

    virtual ~NetInterface() {}

virtual    QString execPlugin() = 0;
virtual    QString loadFile(const QString &urlfile)=0;

};
/**
 * @brief The RowatInterface class فئة خاصة بالتعامل مع الرواة
 */
class RowatInterface
{
public:

   virtual ~RowatInterface() {}
   virtual  void treeChargeRowtName(QTreeWidget *view) = 0;
   virtual  QString readxml(int pos) = 0;
  virtual  void  rawiFind(QString rawi,QTreeWidget *viewS,QTreeWidget *viewD) = 0;

};

/**
 * @brief The ShamilaInterface class فئة خاصة بالتعامل مع قرص الشاملة
 */
class ShamilaInterface
{
public:

   virtual ~ShamilaInterface() {}
virtual    QString execPlugin() = 0;

};
/**
 * @brief The PdfInterface class فئة خاصة بالتعامل مع ملفات بي دي اف
 */
class PdfInterface
{
public:

    virtual ~PdfInterface() {}

  virtual QWidget *PdfWidgetScroll()=0;

    virtual  int pagesCount()=0;

    virtual  int pageCurent()=0;
    virtual  void setListActions(QList<QAction *> list)=0;
 //   virtual  void setEnsureVisible(QRectF rectF)=0;
    virtual   QRectF searchForwards(const QString &text)=0;
    virtual   bool setDocument(const QString &filePath)=0;
    virtual   void closeDoc()=0;
    virtual  void setCostmColor(bool costum,const QString &color)=0;
    virtual  void setPage(int page = -1, QRectF rectF=QRectF(0,0,0,0))=0;
    virtual  void loadToc(QTreeWidget *view)=0;
    virtual void zoomOut()=0;
    virtual void zoomIn()=0;
virtual  QImage pixmap()=0;
};

//! [2]

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(NetInterface,
                    "com.elirtasse.Plugin.NetInterface/1.0");
Q_DECLARE_INTERFACE(RowatInterface,
                    "com.trolltech.Plugin.RowatInterface/1.0");

Q_DECLARE_INTERFACE(ShamilaInterface,
                    "com.trolltech.Plugin.ShamilaInterface/1.0");
Q_DECLARE_INTERFACE(PdfInterface,
                    "com.trolltech.Plugin.PdfInterface/1.0");
QT_END_NAMESPACE
#endif // BOOKSINTERFACE_H
