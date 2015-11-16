#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T21:27:50
#
#-------------------------------------------------

QT       -= gui

TARGET = common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

SOURCES += common.cpp

HEADERS += common.h\
        common_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
