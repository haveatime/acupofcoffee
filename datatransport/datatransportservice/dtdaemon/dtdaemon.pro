QTSERVICE_SETUP_ENV = $$(QTSERVICE_SETUP)
isEmpty(QTSERVICE_SETUP_ENV):error(QTSERVICE_SETUP not found!)

QT -= gui

TARGET = dtdaemon
DESTDIR = ../../bin
TEMPLATE = app
CONFIG += debug console

INCLUDEPATH += $$(QTSERVICE_SETUP)/src ../../datatransportwraper/

HEADERS       = dtcoordinator.h

SOURCES += dtdaemon.cpp dtcoordinator.cpp

unix {
QMAKE_LFLAGS += '-Wl,-rpath-link=../../bin'
}

LIBS += -L../../bin -ldatatransportwraper

win32 : debug {
LIBS += -L$$(QTSERVICE_SETUP)/lib -lQtSolutions_Service-headd
} else {
LIBS += -L$$(QTSERVICE_SETUP)/lib -lQtSolutions_Service-head
}
