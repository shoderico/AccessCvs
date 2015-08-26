TEMPLATE = subdirs

SUBDIRS += \
    util \
    accesscvs \
    accutil \
    accessutil \
    external \
    resource \
    cvsmodel

accesscvs.depends += util accessutil cvsmodel resource
accutil.depends += accessutil
accessutil.depdens += util
cvsmodel.depends += util

OTHER_FILES += \
    common.pri \
    submodule.pri \
    subexe.pri \
    mod_src.pro.tmpl \
    mod_dep.pri.tmpl \
    mod_inc.pri.tmpl
