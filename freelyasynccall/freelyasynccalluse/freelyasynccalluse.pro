QT -= gui

TARGET = freelyasynccalluse
DESTDIR = ../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += ../freelyasynccallwrap/

HEADERS       = freelycallclass.h

SOURCES = freelycallclass.cpp freelyasynccalluse.cpp

LIBS += -L../bin -lfreelyasynccallwrap
