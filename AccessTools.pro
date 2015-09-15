TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    util \
    accesscvs \
    accutil \
    accessutil \
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
accessutil.depends += util officelib

# controller layer
cvscontroller.depends        += view addinutil cvsmodel
gitcontroller.depends        += view addinutil cvsmodel
accessutilcontroller.depends += view addinutil accessutil
helpcontroller.depends       +=      addinutil comutil

# addin layer
addin.depends     += officelib comutil
accesscvs.depends += resource addin \
                     cvscontroller gitcontroller accessutilcontroller helpcontroller


# cli layer
accutil.depends += accessutil

# installer layer
installer.depends += accutil accesscvs

OTHER_FILES += \
    common.pri \
    submodule.pri \
    subexe.pri \
    mod_src.pro.tmpl \
    mod_dep.pri.tmpl \
    mod_inc.pri.tmpl
