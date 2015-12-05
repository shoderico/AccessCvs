TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    accesslib \
    excellib \
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
    view \
    addinutil \
    comutil \
    installer \
    helpcontroller

# office lib layer
accesslib.depends += officelib
excellib.depends += officelib

# model layer
cvsmodel.depends   += util accesslib
accmodel.depends += util accesslib

# controller layer
cvscontroller.depends        += view addinutil cvsmodel
acccontroller.depends        += view addinutil accmodel
helpcontroller.depends       +=      addinutil comutil

# addin layer
addin.depends     += comutil addinutil
accesscvs.depends += resource addin \
                     cvscontroller acccontroller helpcontroller


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
