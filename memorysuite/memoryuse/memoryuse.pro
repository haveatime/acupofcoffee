QT -= gui

TARGET = memoryuse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../memorywrap/

SOURCES = memoryuse.cpp

LIBS += -L../bin -lmemorywrap
