QT += gui
QT += widgets

CONFIG += dll

TARGET   = helpcontroller
TEMPLATE = lib

DEFINES += HELPCONTROLLER_LIBRARY

SOURCES += \
    helpcontroller/helpcontroller.cpp

HEADERS += \
    helpcontroller/helpcontroller_global.h \
    helpcontroller/helpcontroller.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
