include(prifunctionuse.pri)

QT =

TARGET = prifunctionuse
TEMPLATE = app
DESTDIR = ./
CONFIG += debug console

SOURCES += prifunctionuse.cpp

#
# TEST isEnvExist FUNCTION
#
isEnvExist(QTDIR) {
   QTDIR_LIB_PATH = $$(QTDIR)/lib
} else {
   QTDIR_LIB_PATH = lib
}

message(QTDIR_LIB_PATH=$${QTDIR_LIB_PATH})


#
# TEST addPrefix FUNCTION
#

unix {
USER_ADD_PREFIX = $$addPrefix(USER,head)
} else {
USER_ADD_PREFIX = $$addPrefix(USERNAME,head)
}
message(USER_ADD_PREFIX=$${USER_ADD_PREFIX})
