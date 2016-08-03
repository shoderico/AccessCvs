QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = accessaddincontroller
TEMPLATE = lib

DEFINES += ACCESSADDINCONTROLLER_LIBRARY

SOURCES += \
    accessaddincontroller/accessaddincontroller.cpp

HEADERS += \
    accessaddincontroller/accessaddincontroller.h \
    accessaddincontroller/accessaddincontroller_global.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
