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
    cvsmodel/projectsetting.cpp \
    cvsmodel/setting.cpp \
    cvsmodel/sanitizer/accessdesignobjectsanitizer.cpp \
    cvsmodel/sanitizer/tabledatasanitizer.cpp \
    cvsmodel/sanitizer/tabledefsanitizer.cpp \
    cvsmodel/processor/objectprocessor.cpp \
    cvsmodel/processor/vbprojectprocessor.cpp \
    cvsmodel/processor/projectfileprocessor.cpp \
    cvsmodel/processor/referenceprocessor.cpp \
    cvsmodel/processor/projectlevelobjectprocessor.cpp \
    cvsmodel/processor/moduleprocessor.cpp \
    cvsmodel/processor/macroprocessor.cpp \
    cvsmodel/processor/reportprocessor.cpp \
    cvsmodel/processor/formprocessor.cpp \
    cvsmodel/processor/accessdesignobjectprocessor.cpp \
    cvsmodel/processor/accessobjectprocessor.cpp \
    cvsmodel/processor/queryasobjectprocessor.cpp \
    cvsmodel/processor/queryassqlprocessor.cpp \
    cvsmodel/processor/relationprocessor.cpp \
    cvsmodel/processor/tabledefprocessor.cpp

HEADERS += \
    cvsmodel/cvsmodel_global.h \
    cvsmodel/objectitem.h \
    cvsmodel/objectitems.h \
    cvsmodel/objectmodel.h \
    cvsmodel/objectproxymodel.h \
    cvsmodel/projectsetting.h \
    cvsmodel/setting.h \
    cvsmodel/sanitizer/accessdesignobjectsanitizer.h \
    cvsmodel/sanitizer/tabledatasanitizer.h \
    cvsmodel/sanitizer/tabledefsanitizer.h \
    cvsmodel/processor/objectprocessor.h \
    cvsmodel/processor/vbprojectprocessor.h \
    cvsmodel/processor/projectfileprocessor.h \
    cvsmodel/processor/referenceprocessor.h \
    cvsmodel/processor/projectlevelobjectprocessor.h \
    cvsmodel/processor/moduleprocessor.h \
    cvsmodel/processor/macroprocessor.h \
    cvsmodel/processor/reportprocessor.h \
    cvsmodel/processor/formprocessor.h \
    cvsmodel/processor/accessdesignobjectprocessor.h \
    cvsmodel/processor/accessobjectprocessor.h \
    cvsmodel/processor/queryasobjectprocessor.h \
    cvsmodel/processor/queryassqlprocessor.h \
    cvsmodel/processor/relationprocessor.h \
    cvsmodel/processor/tabledefprocessor.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
