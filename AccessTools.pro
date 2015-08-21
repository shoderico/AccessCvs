TEMPLATE = subdirs

SUBDIRS += \
    util \
    accesscvs \
    accutil

accesscvs.depends += util
