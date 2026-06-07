#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T16:35:35
#
#-------------------------------------------------

QT -= gui
QT += axcontainer

TARGET = accessmodel
TEMPLATE = lib

CONFIG += dll

LIBS += -loleacc

DEFINES += ACCESSMODEL_LIBRARY

SOURCES += \
    accessmodel/accessmodel.cpp

HEADERS += \
    accessmodel/accessmodel_global.h \
    accessmodel/accessmodel.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri)
