QT -= gui

TARGET = gmssluse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../gmsslwrap/

SOURCES = gmssluse.cpp

LIBS += -L../bin -lgmsslwrap

unix {
QMAKE_LFLAGS += '-Wl,-rpath-link=../bin/'
}
