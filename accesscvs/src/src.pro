QT -= gui
QT += axserver
QT += axcontainer

CONFIG += dll
#CONFIG += qaxserver_no_postlink
CONFIG += c++11

LIBS += -loleacc

TARGET   = AccessCvs
TEMPLATE = lib
#VERSION  = 1.0

DEFINES += ACCESSCVS_LIBRARY
DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += \
    cvsaddin/cvsaddinfactory.cpp \
    cvsaddin/cvsaddinimpl.cpp \
    managers/windowwidgetmanager.cpp

HEADERS +=\
    cvsaddin/cvsaddinfactory.h \
    cvsaddin/cvsaddinimpl.h \
    managers/windowwidgetmanager.h

OTHER_FILES += \
    cvsaddin/ribbon.xml

RESOURCES += \
    cvsaddin/cvsaddin.qrc

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
