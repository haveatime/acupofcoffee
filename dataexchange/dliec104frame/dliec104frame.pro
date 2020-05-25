QT -= gui

TARGET        = dliec104frame
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datalinkframeinterface

DEFINES += DL_IEC104_FRAME_EXPORT

HEADERS       = dliec104frame.h

SOURCES       = dliec104frame.cpp

LIBS += -L../bin -ldatalinkframeinterface

