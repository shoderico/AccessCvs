# util
UTIL_LIB_DIR = $${PROJECT_LIBRARY_DIR}
UTIL_BIN_DIR = $${PROJECT_BINARY_DIR}
UTIL_FILES   = $${UTIL_BIN_DIR}/util.dll
LIBS +=      -L$${UTIL_LIB_DIR}/ -lutil

INCLUDEPATH += $${PROJECT_ROOT}/util/src

# register this module dll files to dependent module files
DEP_DLLS_MODULE.files += $${UTIL_FILES}

# load self dependencies
include(util_dep.pri)
