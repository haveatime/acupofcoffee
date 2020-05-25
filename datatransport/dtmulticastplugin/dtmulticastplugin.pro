QT -= gui
QT += network

TARGET        = dtmulticastplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_MULTICAST_PLUGIN_EXPORT

HEADERS       = dtmulticastplugin.h

SOURCES       = dtmulticastplugin.cpp

LIBS += -L../bin -ldatatransportinterface

