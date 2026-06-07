QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = excelcvsaddincontroller
TEMPLATE = lib

DEFINES += EXCELCVSADDINCONTROLLER_LIBRARY

SOURCES += \
    excelcvsaddincontroller/excelcvsaddincontroller.cpp

HEADERS += \
    excelcvsaddincontroller/excelcvsaddincontroller.h \
    excelcvsaddincontroller/excelcvsaddincontroller_global.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
