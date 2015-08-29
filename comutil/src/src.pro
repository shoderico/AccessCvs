QT -= gui
QT += winextras
QT += svg

CONFIG += dll

TARGET   = comutil
TEMPLATE = lib

DEFINES += COMUTIL_LIBRARY

LIBS += -loleaut32 -luuid

SOURCES += \
    comutil/comutil.cpp

HEADERS += \
    comutil/comutil.h \
    comutil/comutil_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
