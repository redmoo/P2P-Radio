#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T20:50:02
#
#-------------------------------------------------

QT  += core gui
QT  += network
QT  += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app

SOURCES +=  main.cpp\
            main_window.cpp \
            serverstreamer.cpp \
            player.cpp \
            audiosource.cpp

HEADERS +=  main_window.h \
            serverstreamer.h \
            player.h \
            audiosource.h

FORMS += main_window.ui

RESOURCES +=

INCLUDEPATH += ../common

CONFIG += c++11 qtestlib

#LIBS += -L../common -lcommon

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
#else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

#INCLUDEPATH += $$PWD/../common
#DEPENDPATH += $$PWD/../common

OTHER_FILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common
DEPENDPATH += $$PWD/../common
