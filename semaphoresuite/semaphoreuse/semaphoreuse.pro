QT -= gui

TARGET = semaphoreuse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../semaphorewrap/

SOURCES = semaphoreuse.cpp

LIBS += -L../bin -lsemaphorewrap
