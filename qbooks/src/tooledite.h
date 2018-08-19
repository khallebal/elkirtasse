#ifndef TOOLEDITE_H
#define TOOLEDITE_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QAction>

#include <QToolBar>
class QFrame;
/**
 * @brief The ToolEdite class ادوات تحرير الكتاب
 *  \author    أبو زكريا
 *  \version   3.6.8
 *  \date      2010-2014
 *  \copyright GNU Public License.
 */
class ToolEdite : public QWidget
{
    Q_OBJECT
public:
    explicit ToolEdite(QWidget *parent = 0);
    ~ToolEdite();
     QFrame *frameEdit  ;
          QToolBar *toolBarEditBookPage ;
          QAction *actionFahAddItem;
          QAction *actionFahAddChild;
          QAction *actionNewPageAfter;
          QAction *actionNewPageBefor;
           QAction *actionFontCostum;
           QAction *actionRemovePage;

signals:

    void itemFahrasAdd();
    void itemFahrasAddChild();
    void saveBookEndFahras();
    void itemFahrasRename();
    void itemFahrasRemove();
    void itemFahrasMoveDown();
    void itemFahrasMoveUp();
    void goFindFah(QRegExp rx,QRegExp rx2);
    void newPageAfter();
    void newPageBefort();
    void removePage();
    void fontCostum();
public slots:
    void setCopyAvailable(bool);
private:
    //! tools edit fahras

    //!
    QAction *actionItemFahUp;
    QAction *actionItemDown;
    QAction *actionItemFahDelet;
    QAction *actionItemFahRen;

    QAction *actionSaveEdit;





    //! tool edit

  QPushButton *pushButtonEditClose ;
 QCheckBox *checkBoxNumber ;
  QCheckBox *checkBoxNumber2 ;
   QLineEdit *lineEditNiveau1;
    QLineEdit *lineEditNiveau2;
      QPushButton *pushButtonGoFindFah ;

private slots:
     void btnGoFindFah();
};

#endif // TOOLEDITE_H
