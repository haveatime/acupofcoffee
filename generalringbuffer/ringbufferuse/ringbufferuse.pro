QT -= gui

TARGET = ringbufferuse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../ringbufferwrap/

SOURCES = ringbufferuse.cxx

LIBS += -L../bin -lringbufferwrap
