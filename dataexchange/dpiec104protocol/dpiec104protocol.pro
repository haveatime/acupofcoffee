QT -= gui

TARGET        = dpiec104protocol
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../dataprotocolinterface ../datalinkframeinterface  ../dliec104frame

DEFINES += DP_IEC104_PROTOCOL_EXPORT

HEADERS       = dpiec104protocol.h

SOURCES       = dpiec104protocol.cpp

LIBS += -L../bin -ldataprotocolinterface -ldliec104frame

