QT -= gui

TARGET = rawdataprocess
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

SOURCES += rawdataprocess.cxx

unix:{
QMAKE_CXXFLAGS += -std=gnu++0x
}

