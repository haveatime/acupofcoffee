QT += network

TARGET        = samplegeneralplugin
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../generalplugininterface

DEFINES += SAMPLE_GENERAL_PLUGIN_EXPORT

HEADERS       = samplegeneralplugin.h

SOURCES       = samplegeneralplugin.cpp

LIBS += -L../bin -lgeneralplugininterface

