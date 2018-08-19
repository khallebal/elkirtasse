TEMPLATE = lib
CONFIG += plugin release

INCLUDEPATH += ../qbooks
TARGET = $$qtLibraryTarget(kirtassenet)
DESTDIR = ../usr/share/elkirtasse/plugins
HEADERS += networkplugin.h \
    dialognet.h
SOURCES += networkplugin.cpp \
    dialognet.cpp
QT += xml
FORMS += dialognet.ui
# install
KDIR = mkdir -p /usr/share/elkirtasse/plugins
target.path = /usr/share/elkirtasse/plugins
INSTALLS += target
