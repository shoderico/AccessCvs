QT += gui
QT += widgets

CONFIG += dll

TARGET   = view
TEMPLATE = lib

DEFINES += VIEW_LIBRARY

SOURCES += \
    view/boolcheckboxitemdelegate.cpp \
    view/checkboxitemdelegate.cpp \
    view/uiblocker.cpp

HEADERS += \
    view/view_global.h \
    view/boolcheckboxitemdelegate.h \
    view/checkboxitemdelegate.h \
    view/uiblocker.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
