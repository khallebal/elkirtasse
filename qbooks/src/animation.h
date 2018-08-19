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
#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtGui>
#include <QObject>

/**
 * @brief The Pixmap class فئة خاصة بالايقونات التي تعرض في المشهد المتحرك
 */
class Pixmap : public QGraphicsWidget
{
    Q_OBJECT

public:
    /**
     * @brief Pixmap
     * @param pix
     * @param parent
     */
    Pixmap(const QPixmap &pix, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent), orig(pix), p(pix)
    {

        setCacheMode(DeviceCoordinateCache);
        setAcceptHoverEvents(true);
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
    {
        painter->drawPixmap(QPointF(), p);
    }

    virtual void setGeometry(const QRectF &rect)
    {
        QGraphicsWidget::setGeometry(rect);

        if (rect.size().width() > orig.size().width())
            p = orig.scaled(rect.size().toSize());
        else
            p = orig;
    }
signals:
    void pressed();
    void release();
    void enter();
    void leave();


protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *)
    {
        emit pressed();
        update();
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *)
    {
        emit release();
        update();
    }
    void hoverEnterEvent(  QGraphicsSceneHoverEvent *)
    {

        emit enter();
        update();

    }
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent  *)
    {
        emit leave();
        update();
    }
private:
    QPixmap orig;
    QPixmap p;

};
/*!
 *  \brief     فئة خاصة بمشهد الايقونات المتحركة
 *  \details   The Pixmap class فئة خاصة بالايقونات التي تعرض في المشهد المتحرك
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class View : public QGraphicsView
{
    Q_OBJECT
public:
    View( QWidget *parent=0) ;
    ~View();
    /**
     * @brief setTreeViw جلب الشجرة التي يتم تحميل المعلومات منها
     * @param view الشجرة
     */
    void setTreeViw(QTreeWidget *view){treeView =view;}

    void init();        /*!< تكوين المشهد */
    bool threeD;        /*!< ايجابي ثلاثي الابعاد سابي التتابخ */
    int recentNbr;     /*!< عدد الكتب السابقة المسموح بها */
    enum { MaxRecentFiles = 15 };
    QAction *recentFileActs[MaxRecentFiles];/*!< اوامر الكتب السابقة */
signals:
    //! مؤشر لامر فتح كتاب
    void openBook(const QString &,const QString &,const QString &,int =0);
    //! مؤشر لعرض المستوى الحالي او قائمة المجموعات
    void labelInfoChanged(const QString &);

protected:
    //! حدث تغير حجم التافذة
    void resizeEvent(QResizeEvent *event)
    {

        QGraphicsView::resizeEvent(event);
        setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    //! حدث النقر
    void keyPressEvent(QKeyEvent *event);
    //! حدث رفع زر المؤشر
    void keyReleaseEvent(QKeyEvent *event);
private:
    QTreeWidget *treeView;      /*!< الشجرة المستهدفة */
    QGraphicsScene *mscene;     /*!< المشهد */
#if QT_VERSION >= 0x040600
    QEasingCurve::Type curveType;       /*!< نوع الحركة */
#endif
    int itemCurant;         /*!< عدد الايقونات */
    QList<Pixmap *> items;  /*!< قائمة بالايقونات المستخدمة */
    QTimer  *m_timer ;      /*!< الساعة */
    bool m_right;           /*!< هل التحرك الى اليمين في وضع الثلاثي */
    bool isBook;             /*!< هل الايقونة المستهدفة هي ايقونة كتاب */
    bool isThreeD;          /*!< هل المشهد ثلاثي الابعاد */
public slots:
    void chargeLevelOne();  /*!< تحميل المستوى الاول اي قائمة المجموعات */
    void chargeRecent();    /*!< تحميل الكتب السابقة */
    void chargeEnrto();     /*!< تحميل شاشة البداية */
private slots:
    /**
     * @brief animChargePixmape تحميل اليقونات مشهد التتالي
     * @param level المستوى اي الاول او الثاني
     * @param index معرف لعنصر الشجرة
     * @param indexParent معرف للاب في الشجرة
     */
    void animChargePixmape(int level,int index,int indexParent);
    /**
    * @brief animChargePixmape تحميل اليقونات مشهد ثلاثي الابعاد
    * @param level المستوى اي الاول او الثاني
    * @param index معرف لعنصر الشجرة
    * @param indexParent معرف للاب في الشجرة
    */
    void ThreeDChargePixmape(int level,int index,int indexParent);
    void animTeled();               /*!< تحريك متتالي */
    void animChargeLevelTow();      /*!<  تحميل المستوى الثاني اي الكتب عند مشهد التتالي */
    void animItemOpenBook();        /*!< فتح الكتاب عند مشهد التتالي */
    void moveThreeDright();         /*!< تحريك مشهد الثلاثي الى اليمين */
    void moveThreeDleft();          /*!< تحريك مشهد الثلاثي الى اليسار */
    /**
     * @brief moveThreeD تحريك الثلاثي الايعتد
     * @param right موجب يمين
     */
    void moveThreeD(bool right);
    void animEnterItem();           /*!< عند ادخال الفأرة في مشهد التتالي */
    void animLeaveItem();           /*!< عند اخراج الفأرة في مشهد التتالي */
    void ThreeDItemOpenBook();      /*!< فتح الكتاب عند مشهد ثلاثي الابعاد */
    void threedChargeLevelTow();    /*!<  تحميل المستوى الثاني اي الكتب عند مشهد الثلاثي */
    void time_run();                /*!< تشغيل الساعة */
    void stop_time();               /*!< توقيف الساعة */
    void chargeNavigator();
    void chargeMaskItem();
    void comboItemOpenBook();       /*!< فتح الكتب السابقة في مشهد التتالي */
    void ThreeDcomboItemOpenBook(); /*!< فتح الكتب السابقة في مشهد الثلاثي */
    /**
     * @brief animEtoile تحريك النجوم في مشهد البداية
     * @param rect حجم مربع النجمة
     */
    void animEtoile(QRect rect);
    void animEtoileTitle();
    void animEtoileBook();
};

#endif // ANIMATION_H
