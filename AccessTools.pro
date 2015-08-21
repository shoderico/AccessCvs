TEMPLATE = subdirs

SUBDIRS += \
    util \
    accesscvs \
    accutil \
    accessutil \
    external

accesscvs.depends += util
accutil.depends += accessutil
