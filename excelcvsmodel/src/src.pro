QT -= gui
#QT += concurrent
QT += axcontainer

CONFIG += dll

TARGET   = excelcvsmodel
TEMPLATE = lib

DEFINES += EXCELCVSMODEL_LIBRARY

SOURCES += \
    excelcvsmodel/excelobjectmodel.cpp \
    excelcvsmodel/excelobjectproxymodel.cpp \
    excelcvsmodel/excelprojectcontainer.cpp

HEADERS += \
    excelcvsmodel/excelcvsmodel_global.h \
    excelcvsmodel/excelobjectmodel.h \
    excelcvsmodel/excelobjectproxymodel.h \
    excelcvsmodel/excelprojectcontainer.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
