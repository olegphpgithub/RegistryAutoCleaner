#-------------------------------------------------
#
# Project created by QtCreator 2018-11-19T15:21:59
#
#-------------------------------------------------

QT       += core gui

CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RegistryAutoCleaner
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SimpleTimer.cpp

HEADERS  += MainWindow.h \
    SimpleTimer.h

FORMS    += MainWindow.ui
