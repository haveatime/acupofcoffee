QT -= gui

TARGET = dtmonitor
DESTDIR = ../../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../../datatransportwraper/

HEADERS       = dtpeekeye.h

SOURCES += dtmonitor.cpp dtpeekeye.cpp

unix {
QMAKE_LFLAGS += '-Wl,-rpath-link=../../bin'
}

LIBS += -L../../bin -ldatatransportwraper
