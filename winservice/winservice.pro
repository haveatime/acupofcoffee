QT -= gui

TARGET = winservice
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

win32:{
SOURCES += winservice.cpp
} else {
SOURCES += unixservice.cpp
}

