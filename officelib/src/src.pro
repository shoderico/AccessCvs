QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = officelib
TEMPLATE = lib

DEFINES += OFFICELIB_LIBRARY

SOURCES += \
    officelib/office.cpp \
    officelib/vbide.cpp \
    officelib/office_impl.cpp \
    officelib/vbide_impl.cpp

HEADERS +=\
    officelib/office.h \
    officelib/vbide.h \
    officelib/officelib.h \
    officelib/officelib_global.h \
    officelib/inline_export_def.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri)
