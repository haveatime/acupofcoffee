QT -= gui
QT += network

TARGET = hostnameinfo
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

INCLUDEPATH += ./

HEADERS += hostnameresult.h

SOURCES += hostnameinfo.cpp hostnameresult.cpp

