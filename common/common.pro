#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T21:27:50
#
#-------------------------------------------------

QT       -= gui
QT		 += multimedia

TARGET = common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

SOURCES += common.cpp

HEADERS += common.h\
        common_global.h \
    client_info.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
