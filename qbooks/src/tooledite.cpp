#include "tooledite.h"
#include <QLayout>
#include <QIcon>
#include <QLabel>
#include <QStyle>
ToolEdite::ToolEdite(QWidget *parent) :
    QWidget(parent)
{
    //! EDITE FAHRAS********************************************************************

    QIcon icon29;
    icon29.addFile(QString::fromUtf8(":/images/image/edit-delete.png"), QSize(), QIcon::Normal, QIcon::Off);


    frameEdit = new QFrame(this);
    frameEdit->setFrameShape(QFrame::Panel);
    frameEdit->setFrameShadow(QFrame::Raised);
    QVBoxLayout *verticalLayout_3 = new QVBoxLayout(frameEdit);
    verticalLayout_3->setSpacing(3);
    verticalLayout_3->setContentsMargins(3, 3, 3, 3);
    verticalLayout_3->setContentsMargins(3, 3, 3, 6);
    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(3);
    horizontalLayout_2->setContentsMargins(5, 2, -1, 2);
    QLabel *label_4 = new QLabel(trUtf8("تحرير الكتاب"),frameEdit);
    label_4->setLayoutDirection(Qt::LeftToRight);
    label_4->setAlignment(Qt::AlignRight|Qt::AlignHCenter|Qt::AlignVCenter);

    horizontalLayout_2->addWidget(label_4);

//    pushButtonEditClose = new QPushButton(icon29,"",frameEdit);
//    pushButtonEditClose->setMaximumSize(QSize(18, 18));
//    pushButtonEditClose->setDefault(false);
//    pushButtonEditClose->setFlat(true);

//    horizontalLayout_2->addWidget(pushButtonEditClose);


    verticalLayout_3->addLayout(horizontalLayout_2);

    QGridLayout * gridLayout_3 = new QGridLayout();
    gridLayout_3->setSpacing(3);

    checkBoxNumber = new QCheckBox(trUtf8("يبدأ برقم"),frameEdit);
    gridLayout_3->addWidget(checkBoxNumber, 0, 0, 1, 1);

    checkBoxNumber2 = new QCheckBox(trUtf8("يبدأ برقم"),frameEdit);
    gridLayout_3->addWidget(checkBoxNumber2, 0, 1, 1, 1);

    lineEditNiveau1 = new QLineEdit(frameEdit);

    gridLayout_3->addWidget(lineEditNiveau1, 1, 0, 1, 1);

    lineEditNiveau2 = new QLineEdit(frameEdit);

    gridLayout_3->addWidget(lineEditNiveau2, 1, 1, 1, 1);

    pushButtonGoFindFah = new QPushButton(frameEdit);
    pushButtonGoFindFah->setToolTip(trUtf8("تنفيذ البحث عن فهارس للكتاب"));
    pushButtonGoFindFah->setMinimumSize(QSize(25, 25));
    pushButtonGoFindFah->setMaximumSize(QSize(25, 25));
    pushButtonGoFindFah->setSizeIncrement(QSize(25, 25));
    pushButtonGoFindFah->setBaseSize(QSize(25, 25));
    pushButtonGoFindFah->setStyleSheet(QString::fromUtf8(" QPushButton:hover  {\n"
                                                  "     border: 1px solid #8f8f91;\n"
                                                  "     border-radius: 5px;\n"
                                                  " }\n"
                                                  ""));
     pushButtonGoFindFah->setIconSize(QSize(22, 22));
    pushButtonGoFindFah->setDefault(false);
    pushButtonGoFindFah->setFlat(true);
pushButtonGoFindFah->setIcon(QIcon::fromTheme("dialog-ok", QIcon(":/images/image/arrow-left.png")));
    gridLayout_3->addWidget(pushButtonGoFindFah, 1, 2, 1, 1);

    verticalLayout_3->addLayout(gridLayout_3);

    QHBoxLayout *horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setSpacing(3);
    horizontalLayout_5->setContentsMargins(3, 3, 3, 3);

    actionItemFahUp = new QAction(trUtf8("نقل العنصر للاعلى"),this);
       QIcon icon28;
    icon28.addFile(QString::fromUtf8(":/images/image/btnfolder.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionItemFahUp->setIcon(icon28);
    actionItemDown = new QAction(trUtf8("نقل العنصر للاسفل"),this);
     actionItemDown->setIcon(icon28);
    actionItemFahDelet = new QAction(trUtf8("حذف العنصر المحدد"),this);
    actionItemFahDelet->setIcon(icon29);
    actionItemFahRen = new QAction(trUtf8("اعادة تسمية العنصر"),this);
    QIcon icon30;
    icon30.addFile(QString::fromUtf8(":/images/image/edit-rename.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionItemFahRen->setIcon(icon30);
    actionFahAddItem = new QAction(trUtf8("اضافة عنصر جديد"),this);
    actionFahAddItem->setEnabled(false);
    QIcon icon31;
    icon31.addFile(QString::fromUtf8(":/images/image/edit_fahrass.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionFahAddItem->setIcon(icon31);
    actionFahAddChild = new QAction(trUtf8("اضافة عنصر ابن للعنصر المحدد"),this);
    actionFahAddChild->setEnabled(false);
    QIcon icon32;
    icon32.addFile(QString::fromUtf8(":/images/image/edit_fahrass_child.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionFahAddChild->setIcon(icon32);
    actionSaveEdit = new QAction(trUtf8("حفظ التعديلات على الكتاب والفهرسة"),this);
     QIcon icon33;
    icon33.addFile(QString::fromUtf8(":/images/image/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSaveEdit->setIcon(icon33);

    QToolBar *toolBarEditBookFahras=new QToolBar;
    toolBarEditBookFahras->addAction(actionItemFahUp);
    toolBarEditBookFahras->addAction(actionItemDown);
    toolBarEditBookFahras->addAction(actionItemFahRen);
    toolBarEditBookFahras->addSeparator();
    toolBarEditBookFahras->addAction(actionItemFahDelet);
    toolBarEditBookFahras->addSeparator();
    toolBarEditBookFahras->addAction(actionSaveEdit);

  actionItemFahUp->setIcon(style()->standardPixmap(QStyle::SP_ArrowUp));
   actionItemDown->setIcon(style()->standardPixmap(QStyle::SP_ArrowDown));

    horizontalLayout_5->addWidget(toolBarEditBookFahras);

verticalLayout_3->addLayout(horizontalLayout_5);

 //! EDITE PAGES********************************************************************
QIcon icon6;
icon6.addFile(QString::fromUtf8(":/images/image/address-book-new.png"), QSize(), QIcon::Normal, QIcon::Off);

actionNewPageAfter = new QAction(trUtf8("اضافة صفحة بعد الحالية"),this);
actionNewPageAfter->setObjectName(QString::fromUtf8("actionNewPageAfter"));

actionNewPageBefor = new QAction(trUtf8("اضافة صفحة قبل الحالية"),this);
actionNewPageBefor->setObjectName(QString::fromUtf8("actionNewPageBefor"));

actionRemovePage = new QAction(trUtf8("حذف الصفحة الحالية"),this);
actionRemovePage->setObjectName(QString::fromUtf8("actionRemovePage"));
actionRemovePage->setIcon(icon29);
actionFontCostum = new QAction(trUtf8("طبق خط مخصص للتحديد"),this);
actionFontCostum->setObjectName(QString::fromUtf8("actionFontCostum"));
actionFontCostum->setEnabled(false);

toolBarEditBookPage = new QToolBar(this);
     toolBarEditBookPage->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
toolBarEditBookPage->addAction(actionFahAddChild);
toolBarEditBookPage->addAction(actionFahAddItem);
toolBarEditBookPage->addSeparator();
toolBarEditBookPage->addAction(actionNewPageAfter);
toolBarEditBookPage->addAction(actionNewPageBefor);
toolBarEditBookPage->addAction(actionRemovePage);
toolBarEditBookPage->addSeparator();
toolBarEditBookPage->addAction(actionFontCostum);

actionNewPageAfter->setIcon(QIcon::fromTheme("document-new",QIcon(style()->standardPixmap(QStyle::SP_FileIcon))));
actionNewPageBefor->setIcon(QIcon::fromTheme("document-new",QIcon(style()->standardPixmap(QStyle::SP_FileIcon))));
actionFontCostum->setIcon(QIcon::fromTheme("format-text-underline",QIcon(":/images/image/draw-text.png")));

//! CONNECTIONS------------------------------------------------------------------------

//connect(pushButtonEditClose,SIGNAL(clicked()),this,SLOT(close()));

connect(actionItemDown, SIGNAL(triggered()), this, SIGNAL(itemFahrasMoveDown()));
connect(actionItemFahUp, SIGNAL(triggered()), this, SIGNAL(itemFahrasMoveUp()));
connect(actionItemFahRen, SIGNAL(triggered()), this, SIGNAL(itemFahrasRename()));
connect(actionItemFahDelet, SIGNAL(triggered()), this, SIGNAL(itemFahrasRemove()));
connect(actionFahAddItem, SIGNAL(triggered()), this, SIGNAL( itemFahrasAdd()));
connect(actionFahAddChild, SIGNAL(triggered()), this, SIGNAL( itemFahrasAddChild()));

connect(actionNewPageAfter, SIGNAL(triggered()), this, SIGNAL( newPageAfter()));
connect(actionNewPageBefor, SIGNAL(triggered()), this, SIGNAL( newPageBefort()));

connect(actionRemovePage, SIGNAL(triggered()), this, SIGNAL( removePage()));

connect(pushButtonGoFindFah, SIGNAL(clicked()), this, SLOT( btnGoFindFah()));
connect(actionFontCostum, SIGNAL(triggered()), this, SIGNAL( fontCostum()));
connect(actionSaveEdit, SIGNAL(triggered()), this, SIGNAL( saveBookEndFahras()));


}

ToolEdite::~ToolEdite()
{
    delete frameEdit;
    delete toolBarEditBookPage;
//m_anim->stop();
}

void ToolEdite::btnGoFindFah()//انشاء فهارس
{
    QRegExp rx;
    QRegExp rx2;
    if (checkBoxNumber->isChecked()){
        rx.setPattern("(\\d+)("+lineEditNiveau1->text()+")");
    }else{
        if (lineEditNiveau1->text().isEmpty())
            return;
        rx.setPattern(lineEditNiveau1->text());
    }
    if (checkBoxNumber2->isChecked()){
        rx2.setPattern("(\\d+)("+lineEditNiveau2->text()+")");
    }else{

        rx2.setPattern(lineEditNiveau2->text());
    }
emit goFindFah(rx,rx2);


}
void  ToolEdite::setCopyAvailable(bool arg)
{
    actionFahAddItem->setEnabled(arg);
    actionFahAddChild->setEnabled(arg);
//    QAction *actionNewPageAfter;
//    QAction *actionNewPageBefor;
     actionFontCostum->setEnabled(arg);
 //    QAction *actionRemovePage;

}
