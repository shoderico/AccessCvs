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
cvsmodel.depends   += util accesslib excellib
accessmodel.depends += util accesslib

# controller layer
cvsaddincontroller.depends   += view addinutil cvsmodel
accesscontroller.depends     += view addinutil accessmodel
helpcontroller.depends       +=      addinutil comutil

accesscvsaddincontroller.depends += cvsmodel cvsaddincontroller
excelcvsaddincontroller.depends += cvsmodel cvsaddincontroller

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
