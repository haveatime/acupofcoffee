QT -= gui
QT += network

TARGET        = dtudpclientplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_UDPCLIENT_PLUGIN_EXPORT

HEADERS       = dtudpclientplugin.h

SOURCES       = dtudpclientplugin.cpp

LIBS += -L../bin -ldatatransportinterface

