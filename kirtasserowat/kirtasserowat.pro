TEMPLATE = lib
CONFIG += plugin release

INCLUDEPATH += ../qbooks
TARGET = $$qtLibraryTarget(kirtasserowat)
DESTDIR = ../usr/share/elkirtasse/plugins
HEADERS += rowat.h
   QT += xml widgets
SOURCES += rowat.cpp \
    
# install
MKDIR = mkdir -p /usr/share/elkirtasse/plugins


target.path = /usr/share/elkirtasse/plugins

INSTALLS += target
   

