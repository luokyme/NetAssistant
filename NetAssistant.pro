#-------------------------------------------------
#
# Project created by QtCreator 2014-04-10T19:21:07
#
#-------------------------------------------------

!equals(QT_MAJOR_VERSION, 5): error("NetAssistant requires Qt 5")
CONFIG += c++11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetAssistant
target.path=/usr/local/bin
INSTALLS=target

TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    TcpServer.cpp

HEADERS  += \
    define.h \
    mainwindow.h \
    TcpServer.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    qrc.qrc

QT  +=network

win32:RC_FILE += icon.rc
msvc:QMAKE_CXXFLAGS += /utf-8

DISTFILES += \
    android/AndroidManifest.xml

TRANSLATIONS += language/English.ts \
                language/Chinese.ts

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
