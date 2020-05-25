QT -= gui

TARGET = generalpluginuse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../generalplugininterface/

SOURCES = generalpluginuse.cpp

LIBS += -L../bin -lgeneralplugininterface
