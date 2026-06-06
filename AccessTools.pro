TEMPLATE = subdirs

SUBDIRS += \
    officelib \
    accesslib \
    util \
    accesscvs \
    accesscmd \
    accessmodel \
    external \
    resource \
    cvsmodel \
    accesscvsmodel \
    addin \
    accessaddincontroller \
    cvsaddincontroller \
    accesscvsaddincontroller \
    view \
    addinutil \
    comutil \
    installer \
    helpaddincontroller
#    excellib \
#    excelcvsmodel \
#    excelcvsaddincontroller \
#    excelcvs

# office lib layer
accesslib.depends += officelib
#excellib.depends += officelib

# model layer
cvsmodel.depends += util officelib
accesscvsmodel.depends += cvsmodel accesslib
#excelcvsmodel.depends += cvsmodel excellib
accessmodel.depends += util accesslib

# controller layer
cvsaddincontroller.depends += view addinutil cvsmodel
accessaddincontroller.depends += view addinutil accessmodel
helpaddincontroller.depends += addinutil comutil

accesscvsaddincontroller.depends += accesscvsmodel cvsaddincontroller
#excelcvsaddincontroller.depends += excelcvsmodel cvsaddincontroller

# addin layer
addin.depends += comutil addinutil external
accesscvs.depends += resource addin \
                     helpaddincontroller \
                     accesscvsaddincontroller \
                     accessaddincontroller

#excelcvs.depends += resource addin \
#                    helpaddincontroller \
#                    excelcvsaddincontroller

# cli layer
accesscmd.depends += accessmodel

# installer layer
#installer.depends += accesscmd accesscvs excelcvs
installer.depends += accesscmd accesscvs

OTHER_FILES += \
    qmake/common.pri \
    qmake/submodule.pri \
    qmake/subexe.pri \
    qmake/mod_src.pro.tmpl \
    qmake/mod_dep.pri.tmpl \
    qmake/mod_inc.pri.tmpl \
    qmake/mod.pro.tmpl
