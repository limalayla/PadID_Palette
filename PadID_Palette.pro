#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T13:42:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PadID_Palette
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    couleur.cpp \
    colorwindowadd.cpp

HEADERS  += mainwindow.h \
    client.h \
    couleur.h \
    colorwindowadd.h

FORMS    += mainwindow.ui \
    colorwindowadd.ui
