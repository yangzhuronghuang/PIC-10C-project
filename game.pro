#-------------------------------------------------
#
# Project created by QtCreator 2016-01-24T23:57:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app


SOURCES += main.cpp\
        gameui.cpp \
    gameboard.cpp \
    features.cpp

HEADERS  += gameui.h \
    gameboard.h \
    features.h

FORMS    += gameui.ui \
    gameboard.ui

RESOURCES += \
    images.qrc

QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc++

CONFIG += c++11
