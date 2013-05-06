#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T19:18:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = firewall
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    system.cpp \
    service.cpp \
    rule.cpp \
    profile.cpp

HEADERS  += mainwindow.h \
    system.h \
    service.h \
    rule.h \
    profile.h

FORMS    += mainwindow.ui
