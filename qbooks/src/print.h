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
#ifndef PRINT_H
#define PRINT_H
#include <QDialog>
#include "databook.h"
#include <QTreeWidget>
#include <QDomDocument>

namespace Ui
{
    class Print;
}
/**
 * @brief The Print class فئة خاصة لتصدير الكتب بصيغ متعددة
 * html , pdf , epub , qml ,odt
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class Print : public QDialog
{
    Q_OBJECT

public:
    Print(QWidget *parent = 0);
    ~Print();
    QTreeWidget *treeviw;
    // QString m_bookName;


    void setBookTitle(QString name){m_bookTitle=name;}
    void setBookAuthor(QString name){m_bookAuthor=name;}
    void setBookBetaka(QString name){m_bookBetaka=name;}
    void setBookCurentPath(QString name){m_curentPath=name;}
    void setpathApp(QString path){m_pathApp=path;}

private:

    Ui::Print *ui;
    void  loadSettings();
    QDomDocument m_docBooK;
    int rowsCount; /*!< عدد السجلات */
    QString m_fileName;
    QTextEdit* m_textEdit;
    int mCurPart; /*!< الجزء الحالي */
    int m_n;
    QString m_bookTitle;            /*!< عنوان الكتاب*/
    QString m_bookAuthor;            /*!< مؤلف الكتاب */
    QString    m_bookBetaka;         /*!< بطاقة الكتاب */
    QString m_WebColorBack;                          /*!< لون خلفية النص*/
    QString m_WebFont;                            /*!< نوع النص*/
    QString m_WebFontColor;         /*!< لون الخط العام*/
    QString m_WebFontTitleColor;    /*!< لون خط العنوان*/
    QString m_WebFontPrefertColor;  /*!< لون الخط الخاص*/
    QString m_curentPath;           /*!< مسار الكتاب*/
    QString m_WebFontTitle;         /*!< خط العناوين*/
    QString m_pathApp;              /*!< مسار البرنامج*/
private slots:
   void on_comboBox_currentIndexChanged(int index);
    void on_toolButtonGetDir_clicked();
    void on_buttonBox_clicked(QAbstractButton* button);

    bool openBook();
    /**
     * @brief fileSave حفظ الملف بالصيغ المختارة في القائمة المنسدلة
     * @return
     */
    bool fileSave();
    /**
     * @brief creatDocument انشاء ملف من نوع دوكمنت في حالة اختيار
     * pdf or odt
     * ثم استدعاء
     * fileSave
     */
    void creatDocument();


    QString convertTextBetaka(QString txt);
    QString convertText(QString txt,QString part,QString page,int id);
    QString convertEpubStyle(QString txt,QString part,QString page, int id);
    /**
     * @brief creatHtmlDocument انشاء ملفات اش تي مل او ملفات كمل
     * @param isQml اذا كان موجب سينشأ ملف كمل او سالب  سينشأملف اش تي مل
     */
    void    creatHtmlDocument(bool isQml);
    void creatHtmlFahrassa(bool isQml );
    QString getChildHtml(QTreeWidgetItem *item);
    bool creatHtmlPages(bool isQml);
    /**
     * @brief creatEpubDocument انشاء كتاب من نوع epub
     */
    void creatEpubDocument();
    bool   creatEpubDIrectory();
    bool creatEpubContent();
    bool  creatEpubToc();
    QString getChildNavpoint(QTreeWidgetItem *item);
    bool creatEpubInfo();
    bool creatEpubPages();
    bool   zipUpubDocument();


    //! حذف الملفات المؤقتة
    void removeTempDirs(QString dirName);
    bool removeTempFiles(QString tempDir);

};

#endif // PRINT_H
