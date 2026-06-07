QT -= gui
#QT += concurrent
QT += axcontainer

CONFIG += dll

TARGET   = accesscvsmodel
TEMPLATE = lib

DEFINES += ACCESSCVSMODEL_LIBRARY

SOURCES += \
    accesscvsmodel/accessobjectmodel.cpp \
    accesscvsmodel/accessobjectproxymodel.cpp \
    accesscvsmodel/accessprojectcontainer.cpp \
    accesscvsmodel/processor/accessdesignobjectprocessor.cpp \
    accesscvsmodel/processor/accessobjectprocessor.cpp \
    accesscvsmodel/processor/formprocessor.cpp \
    accesscvsmodel/processor/macroprocessor.cpp \
    accesscvsmodel/processor/moduleprocessor.cpp \
    accesscvsmodel/processor/odbctableprocessor.cpp \
    accesscvsmodel/processor/accessprojectfileprocessor.cpp \
    accesscvsmodel/processor/queryasobjectprocessor.cpp \
    accesscvsmodel/processor/queryassqlprocessor.cpp \
    accesscvsmodel/processor/relationprocessor.cpp \
    accesscvsmodel/processor/reportprocessor.cpp \
    accesscvsmodel/processor/tabledefprocessor.cpp \
    accesscvsmodel/processor/tableobjectprocessor.cpp

HEADERS += \
    accesscvsmodel/accesscvsmodel_global.h \
    accesscvsmodel/accessobjectmodel.h \
    accesscvsmodel/accessobjectproxymodel.h \
    accesscvsmodel/accessprojectcontainer.h \
    accesscvsmodel/processor/accessdesignobjectprocessor.h \
    accesscvsmodel/processor/accessobjectprocessor.h \
    accesscvsmodel/processor/formprocessor.h \
    accesscvsmodel/processor/macroprocessor.h \
    accesscvsmodel/processor/moduleprocessor.h \
    accesscvsmodel/processor/odbctableprocessor.h \
    accesscvsmodel/processor/accessprojectfileprocessor.h \
    accesscvsmodel/processor/queryasobjectprocessor.h \
    accesscvsmodel/processor/queryassqlprocessor.h \
    accesscvsmodel/processor/relationprocessor.h \
    accesscvsmodel/processor/reportprocessor.h \
    accesscvsmodel/processor/tabledefprocessor.h \
    accesscvsmodel/processor/tableobjectprocessor.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
