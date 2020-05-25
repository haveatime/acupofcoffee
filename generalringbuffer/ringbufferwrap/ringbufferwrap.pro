QT -= gui

TARGET = ringbufferwrap
DESTDIR       = ../bin
TEMPLATE = lib
CONFIG       += plugin debug

DEFINES += RINGBUFFER_WRAP_EXPORT

SOURCES += ringbufferwrap.cxx
