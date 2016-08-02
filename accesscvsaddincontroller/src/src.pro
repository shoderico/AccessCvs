QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = accesscvsaddincontroller
TEMPLATE = lib

DEFINES += ACCESSCVSADDINCONTROLLER_LIBRARY

SOURCES += \
    accesscvsaddincontroller/accesscvsaddincontroller.cpp

HEADERS += \
    accesscvsaddincontroller/accesscvsaddincontroller.h \
    accesscvsaddincontroller/accesscvsaddincontroller_global.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
