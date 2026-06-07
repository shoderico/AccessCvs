QT += gui

CONFIG += dll

TARGET   = addinutil
TEMPLATE = lib

DEFINES += ADDINUTIL_LIBRARY

SOURCES += \
    addinutil/addinutil.cpp

HEADERS += \
    addinutil/addinutil.h \
    addinutil/addincontroller.h \
    addinutil/addinutil_global.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
