QT -= gui
QT += concurrent
QT += axcontainer

CONFIG += dll

TARGET   = cvsmodel
TEMPLATE = lib

DEFINES += CVSMODEL_LIBRARY

SOURCES += \
    cvsmodel/objectitem.cpp \
    cvsmodel/objectitems.cpp \
    cvsmodel/objectmodel.cpp \
    cvsmodel/objectproxymodel.cpp \
    cvsmodel/objectsetting.cpp \
    cvsmodel/projectsetting.cpp \
    cvsmodel/sanitizesetting.cpp \
    cvsmodel/tabledatasanitizesetting.cpp \
    cvsmodel/tabledefsanitizesetting.cpp

HEADERS += \
    cvsmodel/cvsmodel_global.h \
    cvsmodel/objectitem.h \
    cvsmodel/objectitems.h \
    cvsmodel/objectmodel.h \
    cvsmodel/objectproxymodel.h \
    cvsmodel/objectsetting.h \
    cvsmodel/projectsetting.h \
    cvsmodel/sanitizesetting.h \
    cvsmodel/tabledatasanitizesetting.h \
    cvsmodel/tabledefsanitizesetting.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
