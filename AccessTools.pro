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
    acccontroller \
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
acccontroller.depends        += view addinutil accessmodel
helpcontroller.depends       +=      addinutil comutil

# addin layer
addin.depends     += comutil addinutil
accesscvs.depends += resource addin \
                     cvscontroller acccontroller helpcontroller
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
    common.pri \
    submodule.pri \
    subexe.pri \
    mod_src.pro.tmpl \
    mod_dep.pri.tmpl \
    mod_inc.pri.tmpl
