TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    accesslib \
    excellib \
    util \
    accesscvs \
    accesscmd \
    accessmodel \
    external \
    resource \
    cvsmodel \
    accesscvsmodel \
    excelcvsmodel \
    addin \
    accesscontroller \
    cvsaddincontroller \
    accesscvsaddincontroller \
    excelcvsaddincontroller \
    view \
    addinutil \
    comutil \
    installer \
    helpcontroller \
    excelcvs

# office lib layer
accesslib.depends += officelib
excellib.depends += officelib

# model layer
cvsmodel.depends += util officelib
accesscvsmodel.depends += cvsmodel accesslib
excelcvsmodel.depends += cvsmodel excellib
accessmodel.depends += util accesslib

# controller layer
cvsaddincontroller.depends   += view addinutil accesscvsmodel
accesscontroller.depends     += view addinutil accessmodel
helpcontroller.depends       +=      addinutil comutil

accesscvsaddincontroller.depends += accesscvsmodel cvsaddincontroller
excelcvsaddincontroller.depends += excelcvsmodel cvsaddincontroller

# addin layer
addin.depends     += comutil addinutil
accesscvs.depends += resource addin \
                     accesscontroller accesscvsaddincontroller helpcontroller
excelcvs.depends += resource addin \
                    excellib \
                    util \
                    helpcontroller \
                    excelcvsaddincontroller

# cli layer
accesscmd.depends += accessmodel

# installer layer
installer.depends += accesscmd accesscvs

OTHER_FILES += \
    qmake/common.pri \
    qmake/submodule.pri \
    qmake/subexe.pri \
    qmake/mod_src.pro.tmpl \
    qmake/mod_dep.pri.tmpl \
    qmake/mod_inc.pri.tmpl \
    qmake/mod.pro.tmpl
