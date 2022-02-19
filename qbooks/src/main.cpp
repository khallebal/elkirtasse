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
#include <QApplication>
#include "mainwindow.h"
#include "booksInterface.h"
#include <QSplashScreen>
#include <QTranslator>
#include <QLocale>
#include <QFontDatabase>
#include <QWidgetAction>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #ifdef Q_OS_HAIKU
    a.setApplicationName("Elkirtasse");
    #else
    a.setApplicationName("elkirtasse");
    #endif
    a.setApplicationVersion("3.7.0");
    a.setOrganizationName("Abouzakaria");
    a.setWindowIcon(QIcon::fromTheme("elkirtasse",QIcon(":/images/image/groopbook")));
    qDebug() << "test debug";
    //  QIcon icon;
    QPixmap pixmap(":/images/image/titlekirtasse.png");
    QSplashScreen splash(pixmap);
    splash.setMask(QBitmap(pixmap.mask()));
    splash.show();
    a.processEvents();
    QDir dir;
    QString h=dir.homePath();
    //Create Data Path
    auto data_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << "QStandardPaths::ApplicatonsLocation =" +QStandardPaths::StandardLocation(QStandardPaths::ApplicationsLocation);
    qDebug() << "data-path"+data_path;
    if (!QDir(data_path).exists()) {
    	QDir().mkpath(data_path);
    	}
    //data path
    if (!QDir(data_path).exists(data_path+"/data")) {
    	QDir().mkpath(data_path+"/data");
    	}
    
    //books path
    if (!QDir(data_path).exists(data_path+"/books")) {
    	QDir().mkpath(data_path+"/books");
    	}
    //download path	
    if (!QDir(data_path).exists(data_path+"/download")) {
    	QDir().mkpath(data_path+"/download");
    	}

    QFile file;
    QDir appDir(QCoreApplication::applicationDirPath() );

    appDir.cdUp();
    QString pathApp=  appDir.absolutePath()+"/share/elkirtasse";
    if (!file.exists(data_path+"/data/group.xml")){
        file.copy(pathApp+"/data/group.xml",data_path+"/data/group.xml");
    }
    if (!file.exists(data_path+"/data/bookslist.xml")){
        file.copy(pathApp+"/data/bookslist.xml",data_path+"data/bookslist.xml");
    }

//    if (!file.exists(h+"/.fonts/trado.ttf")){
//        file.copy(pathApp+"/data/font/trado.ttf",h +"/.fonts/trado.ttf");
//    }


    //    QFontDatabase::addApplicationFont(pathApp+"/data/font/Jameel-Noori.ttf");
    QFontDatabase::addApplicationFont(pathApp+"/data/font/trado.ttf");
    QFontDatabase::addApplicationFont(pathApp+"/data/font/ARABSQ.ttf");
    //QLocale::setDefault(QLocale(QLocale::Arabic, QLocale::Egypt));
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translatorsys = new QTranslator(&a);
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translatorsys);
    QSettings settings(data_path+"/data/setting.ini",QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    int lng=settings.value("lng",0).toInt();

    settings.endGroup();
    QString locale = QLocale::system().name();
    // QMessageBox::information(0,"",locale.section("_",0,0));
    QTranslator translator;
    if(lng==0){
        translator.load(QString(pathApp+"/translat/kirtasse_"+locale.section("_",0,0)) );
    }else  if(lng==2){
        translator.load(QString(pathApp+"/translat/kirtasse_pk") );
    }else if(lng==3){
        translator.load(QString(pathApp+"/translat/kirtasse_en") );
    }else if(lng==4){
        translator.load(QString(pathApp+"/translat/kirtasse_fr") );
	}
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
