#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T20:50:02
#
#-------------------------------------------------

QT       += core gui
QT		 += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    serverstreamer.cpp

HEADERS  += main_window.h \
    serverstreamer.h

FORMS    += main_window.ui
