#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T16:35:35
#
#-------------------------------------------------

QT -= gui
QT += axcontainer

TARGET = accmodel
TEMPLATE = lib

CONFIG += dll

LIBS += -loleacc

DEFINES += ACCMODEL_LIBRARY

SOURCES += \
    accmodel/accmodel.cpp

HEADERS += \
    accmodel/accmodel_global.h \
    accmodel/accmodel.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
