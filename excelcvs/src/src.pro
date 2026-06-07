QT -= gui
QT += axserver
QT += axcontainer

CONFIG += dll
#CONFIG += qaxserver_no_postlink
CONFIG += c++11

LIBS += -loleacc

TARGET   = ExcelCvs
TEMPLATE = lib
#VERSION  = 1.0

DEFINES += ExcelCVS_LIBRARY
DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += \
    excelcvs/excelcvsaddinfactory.cpp

HEADERS += \
    excelcvs/excelcvsaddinfactory.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
