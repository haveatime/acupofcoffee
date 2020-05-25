QT -= gui
QT += network

TARGET = multicastuse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../multicastwrap/

HEADERS += multicasttest.h

SOURCES = multicasttest.cpp multicastuse.cpp

LIBS += -L../bin -lmulticastwrap
