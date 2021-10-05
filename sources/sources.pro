#-------------------------------------------------
#
# Project created by QtCreator 2013-05-06T17:45:41
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sources
TEMPLATE = app

#DEFINES += plugins


SOURCES += main.cpp\
        mainwindow.cpp \
    palert.cpp \
    plugin/qonsole.cpp \
    system.cpp \
    service.cpp \
    rule.cpp \
    profile.cpp

HEADERS  += mainwindow.h \
    palert.h \
    plugin/qonsole.h \
    system.h \
    service.h \
    rule.h \
    profile.h

FORMS += \
    mainwindow.ui \
    palert.ui \
    plugin/qonsole.ui
