TEMPLATE = subdirs

SUBDIRS += \
    util \
    accesscvs

accesscvs.depends += util
