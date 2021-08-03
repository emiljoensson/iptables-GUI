#-------------------------------------------------
#
# Project created by QtCreator 2013-05-06T17:45:41
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sources
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

FORMS += \
    mainwindow.ui
