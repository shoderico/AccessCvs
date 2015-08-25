#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T16:35:35
#
#-------------------------------------------------

QT -= gui
QT += axcontainer

TARGET = accessutil
TEMPLATE = lib

DEFINES += ACCESSUTIL_LIBRARY

SOURCES += \
    accessutil.cpp

HEADERS += \
    accessutil_global.h \
    accessutil.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
