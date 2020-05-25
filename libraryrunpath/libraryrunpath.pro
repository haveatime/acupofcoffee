QT -= gui

TARGET = libraryrunpath
DESTDIR = ./
TEMPLATE = app
CONFIG += debug console

unix {
QMAKE_LFLAGS += '-Wl,-rpath=\'\$$ORIGIN\''
QMAKE_LFLAGS += '-Wl,-rpath=\'\$$ORIGIN\',-enable-new-dtags'
}

SOURCES = libraryrunpath.cpp
