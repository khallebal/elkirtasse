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
#include "recentbooks.h"

#include <QDir>
#include <QAction>
#include <QTextStream>
RecentBooks::RecentBooks()
{

    recentLoad();
}
RecentBooks::~RecentBooks()
{


}
//****************************bigen recent****************************
void RecentBooks::recentLoad()
{
    QString myxmlgroup="<?xml version='1.0' encoding='UTF-8'?>"
                       "<setting>"
                       "</setting>";
#ifdef Q_OS_HAIKU
  QString  m_pathUser=QDir::homePath()+"/config/setings/elkirtasse";
#else
	QString  m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/recent.xml");
    file.open(QIODevice::ReadOnly);
    if (!m_docR.setContent(&file))
    {
        m_docR.setContent(myxmlgroup);
    }
    file.close();
}
void RecentBooks::recentCharge()
{


    QDomElement racine = m_docR.documentElement(); //renvoie la balise racine
    QDomNode noeud = racine.lastChild();
    QDomElement Root;
    int i=0;
    while(!noeud.isNull())
    {
        i++;
        Root=noeud.toElement();
        QString texttitle=Root.attribute("title");
        QString textid=Root.attribute("id");
        QString textname=Root.attribute("name");
        QString textaut=Root.attribute("aut");
        QString isTefsir=Root.attribute("isTefsir");

        recentFileActs[i]->setText(texttitle);
        recentFileActs[i]->setData(textid + "|" + textname + "|" + textaut+"|" + isTefsir);
        recentFileActs[i]->setVisible(true);

        if(textname.endsWith("pdf"))
            recentFileActs[i]->setIcon(QIcon::fromTheme("application-pdf",QIcon(":/images/image/application-pdf")));
        else
            recentFileActs[i]->setIcon( QIcon(":/images/image/booki.png"));

        noeud = noeud.previousSibling();

      }
}

void RecentBooks::recentChange(QString Bname,QString Btitle,QString Baut,QString Bid,int nbr,bool isTefsir)
{
    QDomElement racine = m_docR.documentElement(); //renvoie la balise racine
    QDomNodeList tab;
    QDomElement Root;
    QDomNode noeud = racine.firstChild();
    while(!noeud.isNull())
    {
        Root=noeud.toElement();
        QString textname=Root.attribute("name");
        if (textname==Bname){
            racine.removeChild(Root);
        }
        noeud = noeud.nextSibling();
    }
    tab=racine.childNodes();
    while ((int)tab.length()>=nbr)
    {
        QDomNode noeud_f = racine.firstChild();
        QDomElement Root_f;
        Root_f=noeud_f.toElement();
        racine.removeChild(Root_f);
    }
    QDomElement myel=m_docR.createElement("book");     //انشاء عنصر جديد
    racine.appendChild(myel);                    //اظافته في اخر القائمة
    myel.setAttribute("id",Bid);        //اسم مجلد الكتاب
    myel.setAttribute("name",Bname);      //اسم الكتاب
    myel.setAttribute("title",Btitle);      //اسم المؤلف
    myel.setAttribute("aut",Baut);

    myel.setAttribute("isTefsir",isTefsir);
}
void RecentBooks::recentSave()
{
#ifdef Q_OS_HAIKU
  QString  m_pathUser=QDir::homePath()+"/config/setings/elkirtasse";
#else
    QString  m_pathUser=QDir::homePath()+"/.kirtasse";
#endif
    QFile file(m_pathUser +"/data/recent.xml");
    file.open(QIODevice::WriteOnly);              //فتح الملف للكتابة عليها
    QTextStream out(&file);                       //الكتابة
    const int IndentSize = 1;
    m_docR.save(out,IndentSize);                      //حفظ الملف
    file.close();
    m_docR.clear();
}
