QT += gui
QT += widgets

CONFIG += dll

TARGET   = gitcontroller
TEMPLATE = lib

DEFINES += GITCONTROLLER_LIBRARY

SOURCES += \
    gitcontroller/gitcontroller.cpp

HEADERS += \
    gitcontroller/gitcontroller.h \
    gitcontroller/gitcontroller_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

INCLUDEPATH += $${PROJECT_ROOT}/addin/src

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
