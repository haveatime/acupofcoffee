QT -= gui

TARGET = datalinkframeinterface
DESTDIR = ../bin
TEMPLATE = lib
CONFIG += debug console plugin

DEFINES += DATA_LINKFRAME_INTERFACE_EXPORT

HEADERS = datalinkframeinterface.h

SOURCES += datalinkframeinterface.cpp
