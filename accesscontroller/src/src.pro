QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = accesscontroller
TEMPLATE = lib

DEFINES += ACCESSCONTROLLER_LIBRARY

SOURCES += \
    accesscontroller/accesscontroller.cpp

HEADERS += \
    accesscontroller/accesscontroller.h \
    accesscontroller/accesscontroller_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
