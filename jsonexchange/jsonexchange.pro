QT -= gui

TARGET = jsonexchange
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

INCLUDEPATH += .

SOURCES += jsonexchange.cpp

lessThan(QT_MAJOR_VERSION,5):{
INCLUDEPATH += $$(QJSON4_INCLUDE_PATH)
LIBS += -L$$(QJSON4_LIB_PATH) -lqjson4
}
