# inc.pri : external module
# QtSolutions
QTSOLUTIONS_DIR     = $${LIBRARY_ROOT}/qt-solutions/qtwinmigrate

# Reference build artifacts directly (no pre-placed lib_msvc_* folders)
QTSOLUTIONS_LIB_DIR = $${QTSOLUTIONS_DIR}/lib
QTSOLUTIONS_BIN_DIR = $${QTSOLUTIONS_DIR}/lib

QTSOLUTIONS_INC_DIR = $${QTSOLUTIONS_DIR}/src
QTSOLUTIONS_LIBNAME = QtSolutions_MFCMigrationFramework-head$${DEP_DLLS_POSTFIX}
QTSOLUTIONS_FILES   = $${QTSOLUTIONS_BIN_DIR}/$${QTSOLUTIONS_LIBNAME}.dll

#include($${QTSOLUTIONS_DIR}/src/qtwinmigrate.pri)
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
