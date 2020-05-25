QT -= gui

TARGET        = freelyasynccallwrap
DESTDIR       = ../bin/
TEMPLATE      = lib
CONFIG       += debug plugin

DEFINES += FREELY_ASYNC_CALL_WRAP_EXPORT

HEADERS       = freelyasyncemitter.h

SOURCES       = freelyasyncemitter.cpp freelyasynccallwrap.cpp
