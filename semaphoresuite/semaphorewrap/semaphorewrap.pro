QT -= gui

TARGET        = semaphorewrap
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

DEFINES += SEMAPHORE_WRAP_EXPORT

INCLUDEPATH += ./

SOURCES       = semaphorewrap.cpp

