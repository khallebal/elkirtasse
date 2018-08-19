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
#ifndef DIALOGNET_H
#define DIALOGNET_H

#include <QDialog>
#include <QTreeWidget>
#include <QProgressDialog>
//#include <QHttp>
#include <QFile>
// #include <QtNetwork>
 #include <QAbstractButton>

 #include <QProcess>

namespace Ui {
    class Dialognet;

}
/**
 * @brief The Dialognet class نافذة لجلب كتب من الانترنت
 */
class Dialognet : public QDialog {
    Q_OBJECT
    // Q_INTERFACES(BooksInterface)


public:
    /**
      * @brief Dialognet النافذة الرئسية لاختيار كتاب
      * @param parent
      */
    Dialognet(QWidget *parent = 0);
    /**
      * @brief Dialognet نافذة التحميل يتم استدعاؤها مياشرة باعطائخا اسم الكتاب
      * @param urlfile اسم الكتاب
      * @param parent
      */
    Dialognet(const QString &urlfile, QWidget *parent = 0);
    ~Dialognet();
    QString tagzName;
protected:
    void changeEvent(QEvent *e);
public slots:

private:

    Ui::Dialognet *ui;

    QString m_urlFile;
    //   QString m_urlPath;
    //  bool isloaded;
    QProcess *process;
    bool m_error;
    bool isDownlist;//تنزيل قائمة الكتب نعم او لا
private slots:
    /**
     * @brief treeChargeGroupe تجميل شجرة الكتب من booklist
     * @param view الشجرة المستهدفة
     * @param checked هل يتم غرض اداة التحديد
     */
    void treeChargeGroupe(QTreeWidget *view,int checked);

    /**
     * @brief downloadFile غملية التحميل
     * @param urlfile عنوان الكتاب على الانترنت
     * @param distPath مسار التحميل على الجهاز
     * @return ايجابي اذا تم التحميل
     */
    bool downloadFile(const QString &urlfile,const QString &distPath);

    void init();
    //! Brief  قراءة المحرجات
    void readstdout();
    //! Brief  قراءة المحﻻجات التي تعلم عن الخطأ
    void readsteror();
    //! Brief  الاعلام بنخاية التحميل
    void processHasFinished(int);

    void on_lineEdit_2_textChanged(QString );
    void searchInTreeview(QTreeWidget *view,QString searchString,int colum);
    bool searchTreeForString( const QString &searchString, QTreeWidgetItem* parent,bool itemtop,int  topindex,QTreeWidget *view,int colum);
    bool showAllItems( QTreeWidgetItem* parent,QTreeWidget *view );
    void on_treeWidget_itemSelectionChanged();
    //! Brief زر جلب قائمة الكتب المختارة من القائمة المتسدلة
    void on_toolButtonGetList_clicked();
    //! Brief  انهاء العملية
    void cancelDownload();
    //! Brief جلب اعدادات المستخدم لقائمة الكتب
    void loadSettings();
    //! Brief  حفظ الاعدادات في حالة التغيير
    void saveSettings();
    //! Brief جلب قائمة القوائم من الادات في حالة التغيير ثم الحفظ
    QStringList getListUrls();
    //! Brief  النقر على زر موافق او الغاء
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // DIALOGNET_H
