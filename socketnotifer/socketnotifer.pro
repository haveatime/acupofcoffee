QT += network

TARGET = socketnotifer
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

SOURCES += socketnotifer.cxx

windows {
	LIBS += -lws2_32
}
