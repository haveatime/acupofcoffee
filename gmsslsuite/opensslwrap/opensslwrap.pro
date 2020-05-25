win32 : {
GMSSL_SETUP_ENV = $$(GMSSL_SETUP)
isEmpty(GMSSL_SETUP_ENV):error(GMSSL_SETUP not found!)
}

QT -= gui

TARGET        = opensslwrap
DESTDIR       = ../bin
TEMPLATE      = lib
CONFIG       += plugin debug

DEFINES += OPENSSL_WRAP_EXPORT

INCLUDEPATH += ./

win32 : {
INCLUDEPATH += $$(GMSSL_SETUP)/include
}

SOURCES       = opensslwrap.cpp

unix : {
LIBS += -L/usr/local/lib -lgmcrypto
} else {
LIBS += -L$$(GMSSL_SETUP)/lib -llibcrypto
}
