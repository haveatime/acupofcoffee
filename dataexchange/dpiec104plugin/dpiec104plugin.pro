QT -= gui

TARGET        = dpiec104plugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../dataprotocolinterface

DEFINES += DP_IEC104_PLUGIN_EXPORT

HEADERS       = dpiec104plugin.h

SOURCES       = dpiec104plugin.cpp

LIBS += -L../bin -ldataprotocolinterface

