QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = officelib
TEMPLATE = lib

DEFINES += OFFICELIB_LIBRARY

SOURCES += \
    officelib/access.cpp \
    officelib/adodb.cpp \
    officelib/dao.cpp \
    officelib/office.cpp \
    officelib/vbide.cpp

HEADERS +=\
    officelib/access.h \
    officelib/adodb.h \
    officelib/dao.h \
    officelib/office.h \
    officelib/vbide.h \
    officelib/officelib.h \
    officelib/officelib_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
