#ifndef DIALOGACTIONS_H
#define DIALOGACTIONS_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
    class DialogActions;
}

/**
 * @brief The DialogActions class فئة خاصة بتخصيص ازرار الاوامر على شريط الادواة
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class DialogActions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogActions(QWidget *parent = 0);
    ~DialogActions();


private:
    Ui::DialogActions *ui;
 QList<QVariant >listActToAdd;/*!< قائمة الاوامر التي ستضاف للوحة*/

public slots:
    /**
      * @brief setListActions  قوائم الاوامر
      * @param listActions الاوامر المتاحة في البرنامج
      * @param listToAdd القوائم المستخدمة في اللوحة حاليا
      */
    void setListActions(   QList<QAction *>listActions,QList<QVariant >listToAdd);
    /**
     * @brief getListActToAdd جلب قائمة الاوامر
     * @return قائمة بالاوامر المخصصة
     */
    QList<QVariant >getListActToAdd(){return listActToAdd;}

private slots:

    void on_buttonBox_accepted();                   /*!< النقر على زر موافق */
    void on_treeWidgetDest_itemSelectionChanged();  /*!< عند تغير التحديد في شجرة الاوامر التي ستضاف للوحة */
    void on_treeWidgetOrig_itemSelectionChanged();  /*!< عند تغير التحديد في شجرة الاوامرالمتاحة */
    void on_toolButtonUp_clicked();                 /*!< النقر على زر نقل للاعلى */
    void on_toolButtonDown_clicked();                /*!< النقر على زر نقل للاعلى */
    void on_toolButtonAdd_clicked();                /*!< النقر على زر اضافى امر */
    void on_toolButtonRemove_clicked();             /*!< النقر على زر حذف الامر */

};

#endif // DIALOGACTIONS_H
