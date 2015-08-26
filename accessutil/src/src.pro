#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T16:35:35
#
#-------------------------------------------------

QT -= gui
QT += axcontainer

TARGET = accessutil
TEMPLATE = lib

CONFIG += dll

LIBS += -loleacc

DEFINES += ACCESSUTIL_LIBRARY

SOURCES += \
    accessutil/accessutil.cpp

HEADERS += \
    accessutil/accessutil_global.h \
    accessutil/accessutil.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
