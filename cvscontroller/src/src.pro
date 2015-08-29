QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = cvscontroller
TEMPLATE = lib

DEFINES += CVSCONTROLLER_LIBRARY

SOURCES += \
    cvscontroller/cvscontroller.cpp \
    cvscontroller/view/maindialog.cpp

HEADERS += \
    cvscontroller/cvscontroller_global.h \
    cvscontroller/cvscontroller.h \
    cvscontroller/view/maindialog.h

FORMS += \
    cvscontroller/view/maindialog.ui

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

INCLUDEPATH += $${PROJECT_ROOT}/addin/src

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
