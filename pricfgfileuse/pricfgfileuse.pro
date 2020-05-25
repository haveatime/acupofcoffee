include(pricfgfileuse.pri)

QT =

TARGET = pricfgfileuse
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

equals(IS_SOME_THING_YES, "yes") {
DEFINES += IS_SOME_THING_YES=yes
}

SOURCES += pricfgfileuse.cpp
