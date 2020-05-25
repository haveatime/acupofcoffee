QT -= gui
QT += network

TARGET        = dttcpclientplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_TCPCLIENT_PLUGIN_EXPORT

HEADERS       = dttcpclientplugin.h

SOURCES       = dttcpclientplugin.cpp

LIBS += -L../bin -ldatatransportinterface

