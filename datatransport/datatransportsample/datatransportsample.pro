QT -= gui

TARGET = datatransportsample
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../datatransportinterface/ ../datatransportwraper/

HEADERS += transportpluginload.h

SOURCES = datatransportsample.cpp transportpluginload.cpp

unix {
QMAKE_LFLAGS += '-Wl,-rpath-link=../bin'
}

LIBS += -L../bin -ldatatransportwraper
