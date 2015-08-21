QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = officelib
TEMPLATE = lib

DEFINES += OFFICELIB_LIBRARY

SOURCES += \
    access.cpp \
    adodb.cpp \
    dao.cpp \
    office.cpp \
    vbide.cpp

HEADERS +=\
    access.h \
    adodb.h \
    dao.h \
    office.h \
    vbide.h \
    officelib.h \
    officelib_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
