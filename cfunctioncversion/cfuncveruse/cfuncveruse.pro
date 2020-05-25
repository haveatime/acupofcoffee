QT -= gui

TARGET = cfuncveruse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += .

SOURCES = cfuncveruse.cpp

LIBS += -L../bin -lcfuncverlib
