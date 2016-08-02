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
    cvscontroller \
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
cvscontroller.depends        += view addinutil cvsmodel
accesscontroller.depends     += view addinutil accessmodel
helpcontroller.depends       +=      addinutil comutil

# addin layer
addin.depends     += comutil addinutil
accesscvs.depends += resource addin \
                     cvscontroller accesscontroller helpcontroller
accesscvs.depends += resource addin \
                     helpcontroller
excelcvs.depends += resource addin \
                    excellib \
                    util \
                    helpcontroller \
                    cvscontroller

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
