QT -= gui
QT += concurrent
QT += axcontainer

CONFIG += dll

TARGET   = cvsmodel
TEMPLATE = lib

DEFINES += CVSMODEL_LIBRARY

SOURCES += \
    cvsmodel/objectitem.cpp \
    cvsmodel/objectmodel.cpp \
    cvsmodel/objectproxymodel.cpp \
    cvsmodel/sanitizer/accessdesignobjectsanitizer.cpp \
    cvsmodel/sanitizer/tabledatasanitizer.cpp \
    cvsmodel/sanitizer/tabledefsanitizer.cpp \
    cvsmodel/processor/objectprocessor.cpp \
    cvsmodel/processor/vbprojectprocessor.cpp \
    cvsmodel/processor/referenceprocessor.cpp \
    cvsmodel/processor/projectlevelobjectprocessor.cpp \
    cvsmodel/processor/vbeprocessor.cpp \
    cvsmodel/command/sanitizetempdircommand.cpp \
    cvsmodel/command/commandbase.cpp \
    cvsmodel/command/desanitizetempdircommand.cpp \
    cvsmodel/command/deletefromsourcedircommand.cpp \
    cvsmodel/command/deletefromtempdircommand.cpp \
    cvsmodel/command/deletefromprojectcommand.cpp \
    cvsmodel/command/copyfromtempdirtosourcedircommand.cpp \
    cvsmodel/command/copyfromsourcedirtotempdircommand.cpp \
    cvsmodel/command/comparetempdircommand.cpp \
    cvsmodel/command/importfromtempdirtoprojectcommand.cpp \
    cvsmodel/command/exportfromprojecttotempdircommand.cpp \
    cvsmodel/command/updateitemsdifferenceasiscommand.cpp \
    cvsmodel/command/updatefiletimeintempdirbyexportdatecommand.cpp \
    cvsmodel/command/updateitemsexportdatecommand.cpp \
    cvsmodel/command/updateitemsdifferencecommand.cpp \
    cvsmodel/command/updateitemsinsourcedircommand.cpp \
    cvsmodel/command/updatefiletimeintempdircommand.cpp \
    cvsmodel/command/updateitemsinprojectcommand.cpp \
    cvsmodel/command/updateitemscreateupdatedatefromprojectcommand.cpp \
    cvsmodel/command/updateitemsdifferencebyfiletimecommand.cpp \
    cvsmodel/command/loaditemsfromprojectcommand.cpp \
    cvsmodel/command/loaditemsfromsourcedircommand.cpp \
    cvsmodel/objectitemmap.cpp \
    cvsmodel/projectcontainer.cpp

HEADERS += \
    cvsmodel/cvsmodel_global.h \
    cvsmodel/objectitem.h \
    cvsmodel/objectmodel.h \
    cvsmodel/objectproxymodel.h \
    cvsmodel/sanitizer/accessdesignobjectsanitizer.h \
    cvsmodel/sanitizer/tabledatasanitizer.h \
    cvsmodel/sanitizer/tabledefsanitizer.h \
    cvsmodel/processor/objectprocessor.h \
    cvsmodel/processor/vbprojectprocessor.h \
    cvsmodel/processor/referenceprocessor.h \
    cvsmodel/processor/projectlevelobjectprocessor.h \
    cvsmodel/processor/vbeprocessor.h \
    cvsmodel/cvsmodel_const.h \
    cvsmodel/command/sanitizetempdircommand.h \
    cvsmodel/command/commandbase.h \
    cvsmodel/command/desanitizetempdircommand.h \
    cvsmodel/command/deletefromsourcedircommand.h \
    cvsmodel/command/deletefromtempdircommand.h \
    cvsmodel/command/deletefromprojectcommand.h \
    cvsmodel/command/copyfromtempdirtosourcedircommand.h \
    cvsmodel/command/copyfromsourcedirtotempdircommand.h \
    cvsmodel/command/comparetempdircommand.h \
    cvsmodel/command/importfromtempdirtoprojectcommand.h \
    cvsmodel/command/exportfromprojecttotempdircommand.h \
    cvsmodel/command/updateitemsdifferenceasiscommand.h \
    cvsmodel/command/updatefiletimeintempdirbyexportdatecommand.h \
    cvsmodel/command/updateitemsexportdatecommand.h \
    cvsmodel/command/updateitemsdifferencecommand.h \
    cvsmodel/command/updateitemsinsourcedircommand.h \
    cvsmodel/command/updatefiletimeintempdircommand.h \
    cvsmodel/command/updateitemsinprojectcommand.h \
    cvsmodel/command/updateitemscreateupdatedatefromprojectcommand.h \
    cvsmodel/command/updateitemsdifferencebyfiletimecommand.h \
    cvsmodel/command/loaditemsfromprojectcommand.h \
    cvsmodel/command/loaditemsfromsourcedircommand.h \
    cvsmodel/objectitemmap.h \
    cvsmodel/projectcontainer.h

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
