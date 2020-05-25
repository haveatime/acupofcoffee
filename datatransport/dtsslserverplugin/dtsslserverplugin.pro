QT -= gui
QT += network

TARGET        = dtsslserverplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_SSLSERVER_PLUGIN_EXPORT

HEADERS       = dtsslserverplugin.h

SOURCES       = dtsslserverplugin.cpp

LIBS += -L../bin -ldatatransportinterface

