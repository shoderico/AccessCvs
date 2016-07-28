#-------------------------------------------------
#
# Project created by QtCreator 2015-08-26T07:49:59
#
#-------------------------------------------------

QT -= gui

TARGET = resource
TEMPLATE = lib

CONFIG += dll

DEFINES += RESOURCE_LIBRARY

SOURCES += \
    resource/resource.cpp

HEADERS += \
    resource/resource_global.h \
    resource/resource.h

RESOURCES += \
    resource/resource.qrc

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self
