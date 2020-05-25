win32 : {
GMSSL_SETUP_ENV = $$(GMSSL_SETUP)
isEmpty(GMSSL_SETUP_ENV):error(GMSSL_SETUP not found!)
}

QT -= gui

TARGET        = gmsslwrap
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

DEFINES += GMSSL_WRAP_EXPORT

INCLUDEPATH += ./ ../opensslwrap/

win32 : {
INCLUDEPATH += $$(GMSSL_SETUP)/include
}

SOURCES       = gmsslwrap.cpp

LIBS += -L../bin -lopensslwrap

unix : {
LIBS += -L/usr/local/lib -lgmcrypto
} else {
LIBS += -L$$(GMSSL_SETUP)/lib -llibcrypto
}
