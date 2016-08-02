QT += gui
QT += widgets
QT += axcontainer

CONFIG += dll

TARGET   = cvsaddincontroller
TEMPLATE = lib

DEFINES += CVSADDINCONTROLLER_LIBRARY

SOURCES += \
    cvsaddincontroller/cvsaddincontroller.cpp \
    cvsaddincontroller/view/maindialog.cpp \
    cvsaddincontroller/view/hasdatacolumnitemdelegate.cpp \
    cvsaddincontroller/view/cvsprogressdialog.cpp \
    cvsaddincontroller/view/processtypenames.cpp \
    cvsaddincontroller/view/progresshelper.cpp \
    cvsaddincontroller/view/appsignalhandler.cpp \
    cvsaddincontroller/view/settingdialog.cpp

HEADERS += \
    cvsaddincontroller/cvsaddincontroller_global.h \
    cvsaddincontroller/cvsaddincontroller.h \
    cvsaddincontroller/view/maindialog.h \
    cvsaddincontroller/view/hasdatacolumnitemdelegate.h \
    cvsaddincontroller/view/cvsprogressdialog.h \
    cvsaddincontroller/view/processtypenames.h \
    cvsaddincontroller/view/progresshelper.h \
    cvsaddincontroller/view/appsignalhandler.h \
    cvsaddincontroller/view/settingdialog.h

FORMS += \
    cvsaddincontroller/view/maindialog.ui \
    cvsaddincontroller/view/cvsprogressdialog.ui \
    cvsaddincontroller/view/settingdialog.ui

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
