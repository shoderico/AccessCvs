TEMPLATE = subdirs

SUBDIRS += \
    util \
    accesscvs \
    accutil \
    accessutil \
    external

accesscvs.depends += util
accutil.depends += accessutil

OTHER_FILES += \
    common.pri \
    submodule.pri \
    subexe.pri
