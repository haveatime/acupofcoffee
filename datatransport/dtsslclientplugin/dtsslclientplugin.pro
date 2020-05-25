QT -= gui
QT += network

TARGET        = dtsslclientplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_SSLCLIENT_PLUGIN_EXPORT

HEADERS       = dtsslclientplugin.h

SOURCES       = dtsslclientplugin.cpp

LIBS += -L../bin -ldatatransportinterface

