QT -= gui
QT += network

TARGET        = dttcpserverplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_TCPSERVER_PLUGIN_EXPORT

HEADERS       = dttcpserverplugin.h

SOURCES       = dttcpserverplugin.cpp

LIBS += -L../bin -ldatatransportinterface

