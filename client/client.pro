#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T20:50:51
#
#-------------------------------------------------

QT += core gui
QT += network
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app # a rabs to?

SOURCES +=  main.cpp \
            main_window.cpp \
            streamreceiver.cpp \
            ../common/common.cpp

HEADERS += main_window.h \
           streamreceiver.h \
           ../common/common.h

FORMS += main_window.ui

INCLUDEPATH += ../common

CONFIG += c++11 testlib

#LIBS += -L../common -lcommon

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
#else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

#INCLUDEPATH += $$PWD/../common
#DEPENDPATH += $$PWD/../common
