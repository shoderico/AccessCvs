QT += gui
QT += widgets

CONFIG += dll

TARGET   = helpaddincontroller
TEMPLATE = lib

DEFINES += HELPADDINCONTROLLER_LIBRARY

SOURCES += \
    helpaddincontroller/helpaddincontroller.cpp

HEADERS += \
    helpaddincontroller/helpaddincontroller_global.h \
    helpaddincontroller/helpaddincontroller.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
