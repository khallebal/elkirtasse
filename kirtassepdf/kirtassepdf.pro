TEMPLATE = lib
CONFIG += plugin release

INCLUDEPATH += ../qbooks
INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -L/usr/lib -lpoppler-qt4

TARGET = $$qtLibraryTarget(kirtassepdf)
DESTDIR = ../usr/share/elkirtasse/plugins
HEADERS += pdfwidget.h

QT += widgets
SOURCES += pdfwidget.cpp \

# install
MKDIR = mkdir -p /usr/share/elkirtasse/plugins


target.path = /usr/share/elkirtasse/plugins

INSTALLS += target
   

