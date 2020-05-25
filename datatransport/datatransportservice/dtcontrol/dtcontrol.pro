QTSERVICE_SETUP_ENV = $$(QTSERVICE_SETUP)
isEmpty(QTSERVICE_SETUP_ENV):error(QTSERVICE_SETUP not found!)

QT -= gui

TARGET = dtcontrol
DESTDIR = ../../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += $$(QTSERVICE_SETUP)/src

SOURCES += dtcontrol.cpp

win32 : debug {
LIBS += -L$$(QTSERVICE_SETUP)/lib -lQtSolutions_Service-headd
} else {
LIBS += -L$$(QTSERVICE_SETUP)/lib -lQtSolutions_Service-head
}
