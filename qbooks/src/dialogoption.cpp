#include "dialogoption.h"
#include "ui_dialogoption.h"
#include<QtGui>

//#define BTN_STYLE_BEGEN "QPushButton {background-color:"
//#define BTN_STYLE_END " ;\n border: 2px solid rgb(0, 170, 127);\n border-radius: 4px;\nmin-width: 64px;;}\n"
//#define BTN_STYLE_HOVER "QPushButton:hover {\n border: 3px solid rgba(85, 170, 255, 200);\n border-radius: 4px;\n }\n"
//#define BTN_STYLE_PRESED " QPushButton:pressed {\n border: 3px solid rgba(147, 147, 147, 247)\n border-radius: 4px;\n }"
//(BTN_STYLE_BEGEN+colorBack+BTN_STYLE_END+BTN_STYLE_HOVER+BTN_STYLE_PRESED)
Dialogoption::Dialogoption(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialogoption)
{
    ui->setupUi(this);
    m_btnStyle= "QPushButton {background-color:\%1 ;\n"
            "border: 2px solid rgb(0, 170, 127);\n"
            "     border-radius: 4px;\n"
            "min-width: 64px;"
            " ;}\n"
            "QPushButton:hover {\n"
            "      border: 3px solid rgba(85, 170, 255, 200);\n"
            "     border-radius: 4px;\n"
            " }\n"
            " QPushButton:pressed {\n"
            "    border: 3px solid rgba(147, 147, 147, 247)\n"
            "     border-radius: 4px;\n"
            " }";
    QLocale Arabic(QLocale::Arabic);
    Arabic.setNumberOptions(QLocale::OmitGroupSeparator);
    QString txt="1234567890";
    QString s2 = Arabic.toString(txt.toInt());
    ui->comboBoxNumber->addItem(trUtf8("system"));
    ui->comboBoxNumber->addItem(trUtf8("Arabic")+" ("+txt+")");
    ui->comboBoxNumber->addItem(trUtf8("Arabic India")+" ("+s2+")");
    //------------
    ui->comboBoxLng->addItem(trUtf8("النظام"));
    ui->comboBoxLng->addItem(QString::fromUtf8("العربية"));
    ui->comboBoxLng->addItem(QString::fromUtf8("اوردو  باكستان"));
    ui->comboBoxLng->addItem(QString::fromUtf8("english"));
    ui->comboBoxLng->addItem(QString::fromUtf8("francais"));
    //------------
    ui->comboBoxCadre->addItem(QIcon(":/images/image/top.png"),trUtf8("الافتراضي"));
    QDir appDir(qApp->applicationDirPath());
#if defined(Q_OS_HAIKU)
		appDir.cd(".");
		QString pathApp=  appDir.absolutePath()+"/";
#else
    appDir.cdUp();
    QString pathApp=  appDir.absolutePath()+"/share/elkirtasse";
#endif
    QDir dirImage(pathApp+"/data/images");
    QString subdir;
    foreach ( subdir, dirImage.entryList(QDir::AllDirs | QDir::NoDotAndDotDot |
                                         QDir::Hidden )){
        ui->comboBoxCadre->addItem(QIcon(pathApp+"/data/images/"+subdir+"/top.png"),subdir);
    }
    loadSettings();
}
Dialogoption::~Dialogoption()
{

}
void Dialogoption::loadSettings()//load layou
{

    //themesStyle ;fontName
    QStringList styles;
    styles << trUtf8("النظام") <<QStyleFactory::keys() ;
#ifdef Q_OS_HAIKU
	QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
	QSettings settings(m_pathUser+"/setting.ini",QSettings::IniFormat);
#else
	QString m_pathUser=QDir::homePath()+"/.kirtasse";
	QSettings settings(m_pathUser+"/data/setting.ini",QSettings::IniFormat);
#endif
    //****************************

    settings.beginGroup("MainWindow");
    //--------------------------------------------
    QString myStyleName=settings.value("style","").toString();
    int df=   styles.indexOf(myStyleName);
    ui->comboBox->addItems(styles);
    ui->comboBox->setCurrentIndex(df);
    //-------------------------------------------
    //m_WebFont=settings.value("font",trUtf8("Traditional Arabic,20,-1,5,50,0,0,0,0,0")).toString();

    m_fontName.fromString(settings.value("font",trUtf8("Traditional Arabic,20,-1,5,50,0,0,0,0,0")).toString());
    QVariant fs=m_fontName.pointSize();
    ui->label_font->setText(  m_fontName.family()+" "+ fs.toString());
    //---------------------------------------
#ifdef  Q_OS_X11

    ui->label_fontTitle->setText(settings.value("fontTitle","KacstTitle").toString());
    ui->label_fontHachia->setText(settings.value("fontHachia","KacstBook").toString());
#else

    ui->label_fontTitle->setText(settings.value("fontTitle","Andalus").toString());
    ui->label_fontHachia->setText(settings.value("fontHachia","Traditional Arabic").toString());
#endif
    ui->label_fontKhasse->setText(settings.value("FontKhase","Jameel Noori Nastaleeq").toString());

    ui->label_fontTitleSize->setText(settings.value("fontTitleSize","20").toString());



    //---------------------------------------
    colorBack=settings.value("bcolor","#F9F9B9").toString();
    ui->pushButton_backcolor->setStyleSheet(QString(m_btnStyle).arg(colorBack));
   //-------------------------------------------------------------------

    //---------------------------------------
    colorFont=  settings.value("WebFontColor","#000000").toString();
    ui->pushButton_fontcolor->setStyleSheet(QString(m_btnStyle).arg(colorFont));
    //-------------------------------------
    colorFind=settings.value("WebFontFindColor","#aa5500").toString();
    ui->pushButton_findcolor->setStyleSheet(QString(m_btnStyle).arg(colorFind));
    //------------------------------------------------
    colorPrefer=settings.value("WebFontPrefertColor","#264c00").toString();
    ui->pushButton_prefercolor->setStyleSheet(QString(m_btnStyle).arg(colorPrefer));
    //--------------------------------------------------------------
    colorTitle=settings.value("WebFontTitleColor","#ff0000").toString();
    ui->pushButton_titlecolor->setStyleSheet(QString(m_btnStyle).arg(colorTitle));
    //---------------------------------------------------------------------
    ui->spinBox_3->setValue(settings.value("recentNbr",7).toInt());
    //--------------------------------------------------------------
    ui->lineEdit_costmPath->setText(settings.value("pathCostm",m_pathUser+"/books").toString());
    //--------------------------------------------------------------------
    // ui->actionTreeD->setChecked(settings.value("isThreeD",false).toBool())   ;
    ui->comboBoxLng->setCurrentIndex(settings.value("lng",0).toInt());
    //------------------------------------------------------------------------
    ui->checkBoxCadre->setChecked(settings.value("WebCadre",true).toBool());
    //-------------------------------------------------------------
    QString  m_CadrFolder=settings.value("CadrFolder","default").toString();
    int fg=  ui->comboBoxCadre->findText(m_CadrFolder,Qt::MatchExactly);
    ui->comboBoxCadre->setCurrentIndex(fg);
    //----------------------------------------------------------------
  //  ui->checkBoxThemes->setChecked(settings.value("iconsThemes",false).toBool());
    //---------------------------------------------------------------
    ui->checkBoxLngLayou->setChecked(settings.value("layouDir",false).toBool());
    //------------------------------------------------------------------
    ui->comboBoxNumber->setCurrentIndex(settings.value("arabicNmber",0).toInt());
    //-------------------------------------------------------------------
    ui->lineEditStyleCostum->setText(settings.value("styleCostum","").toString());
    //--------------------------------------------------------------------------------
    ui->checkBoxStyleCostum->setChecked(settings.value("isStyleCostum",false).toBool());
//----------------------------------------------------------------------------
    ui->checkBoxPdfColor->setChecked(settings.value("isPdfColor",false).toBool());
    //    m_listActToAdd=settings.value("listActToAdd",m_listActToAdd<<
    //     29<<28<<12<<1000<<15<<16<<17<<18<<1000<<20<<21<<22<<23<<24<<1000<<26<<48<<49<<2000).toList();



    settings.endGroup();
    settings.beginGroup("ToolBar");
    //--------------------------------------------------------------
    ui->radioButtonStandardTool->setChecked(settings.value("isStandardToolBar",true).toBool());
    ui->radioButtonRubon->setChecked(!settings.value("isStandardToolBar",true).toBool());
    //--------------------------------------------------------------
    ui->spinBoxIconSize->setValue(settings.value("toolIconSize",22).toInt());
    //--------------------------------------------------------------
    ui->checkBoxTxtUnderIcon->setChecked(settings.value("textUnderIcon",false).toBool());

    settings.endGroup();


}


void Dialogoption::saveSettings()//حفظ البيانات الى ملف
{
#ifdef Q_OS_HAIKU
	QString m_pathUser=QDir::homePath()+"/config/settings/elkirtasse";
	QSettings settings(m_pathUser+"/setting.ini",QSettings::IniFormat);
#else
    QString m_pathUser=QDir::homePath()+"/.kirtasse/data";
    QSettings settings(m_pathUser+"/setting.ini", QSettings::IniFormat);
#endif
    // QSettings settings("Kirtasse", "setting");
    settings.beginGroup("MainWindow");
    settings.setValue("style", ui->comboBox->currentText());
    settings.setValue("font", m_fontName.toString());
    settings.setValue("fontTitle", ui->label_fontTitle->text());
    settings.setValue("fontTitleSize", ui->label_fontTitleSize->text());
    settings.setValue("FontKhase", ui->label_fontKhasse->text());
    settings.setValue("fontHachia", ui->label_fontHachia->text());
    settings.setValue("bcolor",colorBack);
    settings.setValue("WebFontColor",colorFont);
    settings.setValue("WebFontFindColor",colorFind);
    settings.setValue("WebFontPrefertColor",colorPrefer);
    settings.setValue("WebFontTitleColor",colorTitle);
    settings.setValue("recentNbr",ui->spinBox_3->value());
    settings.setValue("pathCostm",ui->lineEdit_costmPath->text());
    // settings.setValue("isThreeD",ui->actionTreeD->isChecked());
    settings.setValue("WebCadre", ui->checkBoxCadre->isChecked());
  //  settings.setValue("iconsThemes",  ui->checkBoxThemes->isChecked());

    settings.setValue("CadrFolder", ui->comboBoxCadre->currentText());
    settings.setValue("lng",ui->comboBoxLng->currentIndex());
    settings.setValue("layouDir",ui->checkBoxLngLayou->isChecked());
    settings.setValue("arabicNmber",ui->comboBoxNumber->currentIndex());


       // settings.setValue("listActToAdd", m_listActToAdd);
    settings.setValue("styleCostum",ui->checkBoxStyleCostum->isChecked());
    settings.setValue("isStyleCostum", ui->checkBoxStyleCostum->isChecked());
   settings.setValue("isPdfColor", ui->checkBoxPdfColor->isChecked());
    settings.endGroup();

    settings.beginGroup("ToolBar");

    settings.setValue("isStandardToolBar", ui->radioButtonStandardTool->isChecked());
    settings.setValue("toolIconSize",ui->spinBoxIconSize->value());
    settings.setValue("textUnderIcon",ui->checkBoxTxtUnderIcon->isChecked());

    settings.endGroup();
}



void Dialogoption::on_pushButton_fontcolor_clicked()
{
    QColor couleur = QColorDialog::getColor(colorFont, this,trUtf8("اختيار لون خط النص"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){
        colorFont= couleur.name();
        ui->pushButton_fontcolor->setStyleSheet(QString(m_btnStyle).arg(couleur.name()) );
    }
}

void Dialogoption::on_pushButton_titlecolor_clicked()
{
    QColor couleur = QColorDialog::getColor(colorTitle, this,trUtf8("اختيار لون العناوين"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){
        colorTitle= couleur.name();
        ui->pushButton_titlecolor->setStyleSheet(QString(m_btnStyle).arg(couleur.name()) );
    }
}

void Dialogoption::on_pushButton_prefercolor_clicked()
{
    QColor couleur = QColorDialog::getColor(colorPrefer, this,trUtf8("اختيار لون الترميز"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){
        colorPrefer= couleur.name();
        ui->pushButton_prefercolor->setStyleSheet(QString(m_btnStyle).arg(couleur.name()) );
    }
}

void Dialogoption::on_pushButton_findcolor_clicked()
{
    QColor couleur = QColorDialog::getColor(colorFind, this,trUtf8("اختيار لون ترميز البحث"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){
        colorFind= couleur.name();
        ui->pushButton_findcolor->setStyleSheet(QString(m_btnStyle).arg(couleur.name()) );
    }
}

void Dialogoption::on_pushButton_backcolor_clicked()
{
    QColor couleur = QColorDialog::getColor(colorBack, this,trUtf8("اختيار لون خلفية النص"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){
        colorBack= couleur.name();
        ui->pushButton_backcolor->setStyleSheet(QString(m_btnStyle).arg(couleur.name()) );
    }
}

void Dialogoption::on_toolButton_clicked()
{
    bool ok = false;
    QFont police =QFontDialog::getFont(&ok, m_fontName, this,trUtf8("اختيار خط الصفحة"));
    if (ok)
    {
       m_fontName=(police);
        QVariant fs=police.pointSize();
      //  ui->label_fontSize->setText(fs.toString());
        ui->label_font->setText(police.family()+" "+fs.toString());
    }
}

void Dialogoption::on_buttonBox_accepted()
{

    saveSettings();
   this->accept();
}

void Dialogoption::on_toolButton_costmPath_clicked()
{
    QFileDialog dlg;
    QString homeDir=QDir::homePath () ;
    QString dir = dlg.getExistingDirectory(this, tr("Open Directory"),
                                           homeDir,
                                           QFileDialog::ShowDirsOnly
                                           |QFileDialog::DontResolveSymlinks);
   if(!dir.isEmpty())
        ui->lineEdit_costmPath->setText(dir);

}

void Dialogoption::on_toolButtonTitle_clicked()
{
    bool ok = false;
    QVariant fs=ui->label_fontTitleSize->text().trimmed();
    QFont police =QFontDialog::getFont(&ok, QFont(ui->label_fontTitle->text(), fs.toInt()), this,trUtf8("اختيار خط عنوان الصفحة"));
    if (ok)
    {

       fs=police.pointSize();
  QString   fontTitleSize=fs.toString();
      ui->label_fontTitleSize->setText(fontTitleSize);
        ui->label_fontTitle->setText(police.family());
    }
}

void Dialogoption::on_toolButtonHachia_clicked()
{
    bool ok = false;
    QFont police =QFontDialog::getFont(&ok, ui->label_fontHachia->text(), this,trUtf8("اختيار خط حاشية الصفحة"));
    if (ok)
    {
      //  fontHachiaName=(police.family());
        ui->label_fontHachia->setText(police.family());
    }
}


void Dialogoption::on_toolButtonkhass_clicked()
{
    bool ok = false;
    QFont police =QFontDialog::getFont(&ok, ui->label_fontKhasse->text(), this,trUtf8("اختيار خط خاص الصفحة"));
    if (ok)
    {
      //  fontKhasse=(police.family());
        ui->label_fontKhasse->setText(police.family());
    }
}



//void Dialogoption::on_checkBoxStandardTool_clicked()
//{
//       QMessageBox::information(0,"",trUtf8("سيتم تطبيق التغيير بعد اعادة تشغيل التطبيق"));
//}

void Dialogoption::on_toolButtonStyleCostum_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    ui->lineEditStyleCostum->text(),
                                                    tr("Style (*.qss  );;All files (*.*)"));
    if(!fileName.isEmpty())
        ui->lineEditStyleCostum->setText(fileName);
}




