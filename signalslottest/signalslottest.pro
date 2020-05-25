QT += network

TARGET = signalslottest
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

HEADERS += usertestclass.h

SOURCES += signalslottest.cxx usertestclass.cxx
