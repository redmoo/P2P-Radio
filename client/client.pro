#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T20:50:51
#
#-------------------------------------------------

QT       += core gui
QT		 += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    streamreceiver.cpp

HEADERS  += main_window.h \
    streamreceiver.h

FORMS    += main_window.ui
