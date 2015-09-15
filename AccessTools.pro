TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    util \
    accesscvs \
    accutil \
    accmodel \
    external \
    resource \
    cvsmodel \
    addin \
    accessutilcontroller \
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
accessutilcontroller.depends += view addinutil accmodel
helpcontroller.depends       +=      addinutil comutil

# addin layer
addin.depends     += officelib comutil
accesscvs.depends += resource addin \
                     cvscontroller gitcontroller accessutilcontroller helpcontroller


# cli layer
accutil.depends += accmodel

# installer layer
installer.depends += accutil accesscvs

OTHER_FILES += \
    common.pri \
    submodule.pri \
    subexe.pri \
    mod_src.pro.tmpl \
    mod_dep.pri.tmpl \
    mod_inc.pri.tmpl
