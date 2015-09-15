TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    util \
    accesscvs \
    acccmd \
    accmodel \
    external \
    resource \
    cvsmodel \
    addin \
    acccontroller \
    cvscontroller \
    gitcontroller \
    view \
    addinutil \
    comutil \
    installer \
    helpcontroller

# model layer
cvsmodel.depends   += util officelib
accmodel.depends += util officelib

# controller layer
cvscontroller.depends        += view addinutil cvsmodel
gitcontroller.depends        += view addinutil cvsmodel
acccontroller.depends        += view addinutil accmodel
helpcontroller.depends       +=      addinutil comutil

# addin layer
addin.depends     += officelib comutil
accesscvs.depends += resource addin \
                     cvscontroller gitcontroller acccontroller helpcontroller


# cli layer
acccmd.depends += accmodel

# installer layer
installer.depends += acccmd accesscvs

OTHER_FILES += \
    common.pri \
    submodule.pri \
    subexe.pri \
    mod_src.pro.tmpl \
    mod_dep.pri.tmpl \
    mod_inc.pri.tmpl
