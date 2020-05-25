QT -= gui

TARGET        = memorywrap
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

DEFINES += MEMORY_WRAP_EXPORT

INCLUDEPATH += ./

SOURCES       = memorywrap.cpp

