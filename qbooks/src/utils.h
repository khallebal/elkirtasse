#ifndef UTILS_H
#define UTILS_H
#include <QTreeWidget>
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QComboBox>


/**
 * @brief The Utils class بعض العمليات التي تحتاجها الكثير من الفئات
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class Utils
{
public:
    Utils();
    static QDir getPluginDir();
static QString getBookPath(const QString & name,const QString &pathCostm);
    /**
     * @brief favoriteCharge تحميل شجرة المفضلة
     * @param view شجرة المفضلة
     * @param icong ايقونة المجموعة
     * @param iconf ايقوتة الكتاب
     */
    static  void favoriteCharge(QTreeWidget *view,const QIcon &icong,const QIcon &iconf);
    /**
     * @brief treeChargeJozaa تحميل الاجزاء
     * @param view شجرة الاجزاء
     */
    static  void treeChargeJozaa(QTreeWidget *view);
    /**
     * @brief treeChargeSoura تحميل السور
     * @param view شجرة السور
     */
    static  void treeChargeSoura(QTreeWidget *view);
    /**
     * @brief treeChargeGroupe تحكيل شجرة الكنب
     * @param view شجرة الكتب
     * @param checked هل سينك تفعيل التحديد
     * @param asCombobox هل ستم تحميل القائمة المنسدلة
     * @param comboGroup القائمة المنسدلة
     */
    static  void treeChargeGroupe(QTreeWidget *view,int checked,bool asCombobox,QComboBox *comboGroup=0);
    /**
     * @brief treeChargeFahrass تحميل فهرسة الكتاب
     * @param view شجرة الفهرسة التي ستم تحميلها
     * @param Bname اسم الكتاب
     */
    static  void treeChargeFahrass(QTreeWidget *view,const QString &Bname);
    /**
     * @brief comboCharge تحميل قائمة منسدلة
     * @param comboالقائمة المنسدلة
     */
    static  void comboCharge(QComboBox *combo);
    /**
     * @brief favoriteSave حفظ المفضلة
     * @param view شجرة المفضلة
     */
    static void favoriteSave(QTreeWidget *view);
    /**
     * @brief treeSaveGroupe
     * @param view
     * @return
     */
    static bool treeSaveGroupe(QTreeWidget *view);
    /**
     * @brief fahrasSave
     * @param view
     * @param bkname
     * @param pathCostum
     * @return
     */
    static bool fahrasSave(QTreeWidget *view, const QString &bkname, const QString &pathCostum);
    /**
     * @brief treeMenuRemoveBook حذف كتاب
     * @param BKname معرف الكتاب
     * @param removall حذفه من الشجرة والكتاب
     * @param pathCostum المسار المخصص
     * @return
     */
    static bool treeMenuRemoveBook(const QString &BKname,bool removall,const QString &pathCostum);
    /**
     * @brief checkBookExist
     * @param BKname
     * @return
     */
    static bool checkBookExist(QString BKname);
    /**
     * @brief getItemByBook
     * @param view
     * @param dirName
     * @return
     */
    static QTreeWidgetItem *getItemByBook(QTreeWidget *view,const QString &dirName);
    /**
     * @brief getUrlInBookList جلب عنوان انترنت لكتاب محدد من قائمة العناوين
     * @param dirName معرف الكتاب
     * @return العنوان
     */
    static QString getUrlInBookList(const QString &dirName);
    /**
     * @brief treeUpdateGroupe تحديث شجرة الكتب
     * @param view الشجرة المستهدفة
     * @param remove هل سيحذف ام يضيف
     * @param pathCostum المسار المخصص
     */
    static void treeUpdateGroupe(QTreeWidget *view, bool remove, const QString &pathCostum);
    /**
     * @brief addNewBook اضافة كتاب جديد
     * @param bkpath مسار الكتاب
     * @param bktitle عنوان الكتاب
     * @param bkauth المؤلف
     * @param bkbetaka بطاقة الكتاب
     * @param groupid معرف الكتاب
     * @param cheked هل تفسير
     * @return موجب ان تم الاضافة
     */
    static bool addNewBook(const QString &bkpath,const QString &bktitle,
                           const QString &bkauth,const QString &bkbetaka,
                           const QString &groupid ,bool cheked);
    /**
     * @brief treeviewItemRemove حذف عنصر من الشجرة
     * @param view الشجرة المستهدفة
     * @return  موجب ان تم الحذف
     */
    static bool treeviewItemRemove(QTreeWidget *view);
    //! نقل عنصر للاعلى في الشجرة
    static bool treeviewItemUp(QTreeWidget *view);
    //! نقل عنصر للاسفل في الشجرة
    static bool treeviewItemDown(QTreeWidget *view);
    /**
     * @brief genirateId انشاء معرف وحيد للكتاب
     * @param view الشجرة المستهدفى
     * @return المعرف
     */
    static int genirateId(QTreeWidget *view);
    /**
     * @brief saveBookInfo حفظ معلومات الكتاب
     * @param bookname معرف الكتاب
     * @param title العنوان
     * @param author المؤلف
     * @param betaka البطاقة
     * @param pathCostum المسار المخصص
     * @return  موجب ان تم الحفظ
     */
    static bool saveBookInfo(const QString &bookname,const QString &title,
                             const QString &author,const QString &betaka,
                             const QString &pathCostum);
    /**
     * @brief chekBookInfo الاستعلام عن معلومات الكتاب ان كانت متاحة
     * @param bookname اسم الكتاب
     * @return موجب ان تم
     */
    static bool  chekBookInfo(QString bookname);
    /**
     * @brief loadBookInfo جلب معلومات الكتاب من ملف المعلومات
     * @param bookname معرف الكتاب
     *QHash["title"] = "عنوان الكتاب";
     *QHash["author"] = "المؤلف";
     *QHash["betaka"] = "البطاقة";
     * @return QHash<نص, نص>
     */
    static  QHash<QString, QString> loadBookInfo(QString bookname);
    /**
     * @brief geniratNewBookName انشاء معرف للكتاب وحيد
     * @param groupParent المجموعة كي تضاف لام الكتاب
     * @return الاسم
     */
    static QString geniratNewBookName(QString groupParent);
    /**
     * @brief loadTarGz استخراج ملفات مضغوطى
     * @param path المسار
     * @return موجب ان تم الاستخراج
     */
    static bool loadTarGz(QString path);
    /**
     * @brief removeTempDirs حذف مجلد مؤقت
     * @param dirName اسم المجلد
     */
    static  void removeTempDirs(QString dirName);
    /**
     * @brief findFahariss عملية انشاء فهارس في تحرير الفهرسة
     * @param view الشجرة المستهدفة
     * @param bookpath مسار الكتاب
     * @param rx النص الاول
     * @param rx2 النص الثاني
     * @param rowcount عدد السجلات في الكتاب
     */
    static void findFahariss(QTreeWidget *view,QString bookpath,QRegExp rx,QRegExp rx2,int rowcount);
private:
    static  QTreeWidgetItem* getItem(QTreeWidgetItem *item);
    static  void writeInDoc(QString tit,QString data,QString lvl,QDomDocument doc);
    static bool removeTempFiles(QString tempDir);

};

#endif // UTILS_H
