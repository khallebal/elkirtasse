TEMPLATE = lib
CONFIG += plugin release

INCLUDEPATH += ../qbooks
TARGET = $$qtLibraryTarget(kirtassecdrom)
DESTDIR = ../usr/share/elkirtasse/plugins
HEADERS += \ 
    cdromshamila.h \
    dialogcdrom.h
     QT += xml sql
SOURCES += \ 
    cdromshamila.cpp \
    dialogcdrom.cpp
FORMS += \
    Dialogcdrom.ui
    
# install
MKDIR = mkdir -p /usr/share/elkirtasse/plugins
target.path = /usr/share/elkirtasse/plugins

INSTALLS += target 
   














