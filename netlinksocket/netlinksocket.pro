QT =

TARGET = netlinksocket
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

win32:{
SOURCES += winnetlinksocket.cpp
} else {
SOURCES += unixnetlinksocket.cpp
}
