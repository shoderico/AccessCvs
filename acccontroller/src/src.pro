QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = acccontroller
TEMPLATE = lib

DEFINES += ACCCONTROLLER_LIBRARY

SOURCES += \
    acccontroller/acccontroller.cpp

HEADERS += \
    acccontroller/acccontroller.h \
    acccontroller/acccontroller_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
