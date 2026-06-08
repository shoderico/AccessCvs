# inc.pri : external module
# QtSolutions
include(qtsolutions_paths.pri)

# The following contains only side effects for consumers of this module
LIBS += -L$${QTSOLUTIONS_LIB_DIR}/ -l$${QTSOLUTIONS_LIBNAME}
INCLUDEPATH += $${QTSOLUTIONS_INC_DIR}
DEPENDPATH  += $${LIBQGIT_BIN_DIR}
DEFINES     += QT_QTWINMIGRATE_IMPORT
export(LIBS)
export(INCLUDEPATH)
export(DEPENDPATH)
export(DEFINES)

# register this module dll files to dependent external files
DEP_DLLS_EXTERNAL.files += $${QTSOLUTIONS_FILES}
export(DEP_DLLS_EXTERNAL.files)

# load self dependencies
include(qtsolutions_dep.pri)
