# accessutil
ACCESSUTIL_LIB_DIR = $${PROJECT_LIBRARY_DIR}
ACCESSUTIL_BIN_DIR = $${PROJECT_BINARY_DIR}
ACCESSUTIL_FILES   = $${ACCESSUTIL_BIN_DIR}/accessutil.dll
LIBS            += -L$${ACCESSUTIL_LIB_DIR}/ -laccessutil

INCLUDEPATH += $${PROJECT_ROOT}/accessutil/src

# register this module dll files to dependent module files
DEP_DLLS_MODULE.files += $${ACCESSUTIL_FILES}

# load self dependencies
include(accessutil_dep.pri)
