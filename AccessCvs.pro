TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    accesslib \
    util \
    accesscvs \
    accesscmd \
    accessmodel \
    external \
    qtwinmigrate \
    resource \
    cvsmodel \
    accesscvsmodel \
    addin \
    accessaddincontroller \
    cvsaddincontroller \
    accesscvsaddincontroller \
    view \
    addinutil \
    comutil \
    installer \
    helpaddincontroller
#    excellib \
#    excelcvsmodel \
#    excelcvsaddincontroller \
#    excelcvs

# qt-solutions/qtwinmigrate (external project added to SUBDIRS)
qtwinmigrate.file   = ../qt-solutions/qtwinmigrate/buildlib/buildlib.pro
qtwinmigrate.builddir = $$shadowed($$PWD)/qtwinmigrate

# Safely include only definitions (no side effects: LIBS/DEFINES/export etc. are not present)
include(external/qtsolutions_paths.pri)

win32 {
    QMAKE_CLEAN     += $$QTSOLUTIONS_CLEAN_FILES
    QMAKE_DISTCLEAN += $$QTSOLUTIONS_CLEAN_FILES

    message(qmake_clean setted using QTSOLUTIONS_CLEAN_FILES from qtsolutions_paths.pri);
}


# Enforce that config.pri exists (checked at qmake time)
!exists(../qt-solutions/qtwinmigrate/config.pri) {
    error("ERROR: ../qt-solutions/qtwinmigrate/config.pri does not exist. " \
          "Please run `configure.bat -library` in the qtwinmigrate directory first.")
}

# office lib layer
accesslib.depends += officelib
#excellib.depends += officelib

# model layer
cvsmodel.depends += util officelib
accesscvsmodel.depends += cvsmodel accesslib
#excelcvsmodel.depends += cvsmodel excellib
accessmodel.depends += util accesslib

# controller layer
cvsaddincontroller.depends += view addinutil cvsmodel
accessaddincontroller.depends += view addinutil accessmodel
helpaddincontroller.depends += addinutil comutil

accesscvsaddincontroller.depends += accesscvsmodel cvsaddincontroller
#excelcvsaddincontroller.depends += excelcvsmodel cvsaddincontroller

# external layer
external.depends += qtwinmigrate

# addin layer
addin.depends += comutil addinutil external qtwinmigrate
accesscvs.depends += resource addin \
                     helpaddincontroller \
                     accesscvsaddincontroller \
                     accessaddincontroller

#excelcvs.depends += resource addin \
#                    helpaddincontroller \
#                    excelcvsaddincontroller

# cli layer
accesscmd.depends += accessmodel

# installer layer
#installer.depends += accesscmd accesscvs excelcvs
installer.depends += accesscmd accesscvs

OTHER_FILES += \
    qmake/common.pri \
    qmake/submodule.pri \
    qmake/subexe.pri \
    qmake/mod_src.pro.tmpl \
    qmake/mod_dep.pri.tmpl \
    qmake/mod_inc.pri.tmpl \
    qmake/mod.pro.tmpl
