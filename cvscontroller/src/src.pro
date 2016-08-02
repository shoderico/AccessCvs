QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = cvscontroller
TEMPLATE = lib

DEFINES += CVSCONTROLLER_LIBRARY

SOURCES += \
    cvscontroller/cvscontroller.cpp \
    cvscontroller/view/maindialog.cpp \
    cvscontroller/view/hasdatacolumnitemdelegate.cpp \
    cvscontroller/view/cvsprogressdialog.cpp \
    cvscontroller/view/processtypenames.cpp \
    cvscontroller/view/progresshelper.cpp \
    cvscontroller/view/appsignalhandler.cpp \
    cvscontroller/accesscvscontroller.cpp \
    cvscontroller/excelcvscontroller.cpp \
    cvscontroller/view/settingdialog.cpp

HEADERS += \
    cvscontroller/cvscontroller_global.h \
    cvscontroller/cvscontroller.h \
    cvscontroller/view/maindialog.h \
    cvscontroller/view/hasdatacolumnitemdelegate.h \
    cvscontroller/view/cvsprogressdialog.h \
    cvscontroller/view/processtypenames.h \
    cvscontroller/view/progresshelper.h \
    cvscontroller/view/appsignalhandler.h \
    cvscontroller/accesscvscontroller.h \
    cvscontroller/excelcvscontroller.h \
    cvscontroller/view/settingdialog.h

FORMS += \
    cvscontroller/view/maindialog.ui \
    cvscontroller/view/cvsprogressdialog.ui \
    cvscontroller/view/settingdialog.ui

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
