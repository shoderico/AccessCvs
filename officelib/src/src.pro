QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = officelib
TEMPLATE = lib

DEFINES += OFFICELIB_LIBRARY

SOURCES += \
    officelib/office.cpp \
    officelib/vbide.cpp

HEADERS +=\
    officelib/office.h \
    officelib/vbide.h \
    officelib/officelib.h \
    officelib/officelib_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
