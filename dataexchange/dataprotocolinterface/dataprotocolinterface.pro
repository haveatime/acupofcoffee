QT -= gui

TARGET = dataprotocolinterface
DESTDIR = ../bin
TEMPLATE = lib
CONFIG += debug console plugin

DEFINES += DATA_PROTOCOL_PLUGIN_INTERFACE_EXPORT

HEADERS = dataprotocolinterface.h

SOURCES += dataprotocolinterface.cpp
