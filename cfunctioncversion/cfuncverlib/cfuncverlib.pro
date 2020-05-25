QT -= gui

TARGET        = cfuncverlib
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += debug console plugin

SOURCES       = cfuncverlib.cpp

QMAKE_LFLAGS += '-Wl,-version-script=cfuncverlib.ver'

