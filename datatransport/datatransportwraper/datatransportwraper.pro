QT -= gui
QT += network xml

TARGET        = datatransportwraper
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

INCLUDEPATH  += ../datatransportinterface

DEFINES += DATA_TRANSPORT_WRAPER_EXPORT

HEADERS       = datatransportwraper.h

SOURCES       = datatransportwraper.cpp datatransportconfigxml.cpp

LIBS += -L../bin -ldatatransportinterface

