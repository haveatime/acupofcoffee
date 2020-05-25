QT -= gui

TARGET = datatransportinterface
DESTDIR = ../bin
TEMPLATE = lib
CONFIG += debug console plugin

DEFINES += DATA_TRANSPORT_PLUGIN_INTERFACE_EXPORT

HEADERS = datatransportinterface.h

SOURCES += datatransportinterface.cpp
