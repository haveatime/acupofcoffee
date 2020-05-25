QT -= gui
QT += serialport

TARGET        = dtserialportplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DT_SERIALPORT_PLUGIN_EXPORT

HEADERS       = dtserialportplugin.h

SOURCES       = dtserialportplugin.cpp

LIBS += -L../bin -ldatatransportinterface

