# inc.pri : external module
# QtSolutions
QTSOLUTIONS_DIR     = $${LIBRARY_ROOT}/qt-solutions/qtwinmigrate
win32-msvc* {

contains(QT_ARCH, x86_64) {
    QTSOLUTIONS_BIN_DIR = $${QTSOLUTIONS_DIR}/lib_msvc_x64
    QTSOLUTIONS_LIB_DIR = $${QTSOLUTIONS_DIR}/lib_msvc_x64
}
else:contains(QT_ARCH, i386) {
    QTSOLUTIONS_BIN_DIR = $${QTSOLUTIONS_DIR}/lib_msvc_x86
    QTSOLUTIONS_LIB_DIR = $${QTSOLUTIONS_DIR}/lib_msvc_x86
}


}
win32-g++ {
    QTSOLUTIONS_BIN_DIR = $${QTSOLUTIONS_DIR}/lib_mingw_x86
    QTSOLUTIONS_LIB_DIR = $${QTSOLUTIONS_DIR}/lib_mingw_x86
}
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
