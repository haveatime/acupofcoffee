QT -= gui
QT += network

TARGET        = multicastwrap
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

DEFINES += MULTICAST_WRAP_EXPORT

INCLUDEPATH += ./

HEADERS += multicastwrap.h

SOURCES       = multicastwrap.cpp

