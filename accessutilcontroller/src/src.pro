QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = accessutilcontroller
TEMPLATE = lib

DEFINES += ACCESSUTILCONTROLLER_LIBRARY

SOURCES += \
    accessutilcontroller/accessutilcontroller.cpp

HEADERS += \
    accessutilcontroller/accessutilcontroller.h \
    accessutilcontroller/accessutilcontroller_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

INCLUDEPATH += $${PROJECT_ROOT}/addin/src

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
