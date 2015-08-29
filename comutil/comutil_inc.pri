# comutil
COMUTIL_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
COMUTIL_BIN_DIR   = $${PROJECT_BINARY_DIR}
COMUTIL_FILES = $${COMUTIL_BIN_DIR}/comutil.dll
LIBS +=       -L$${COMUTIL_LIB_DIR}/ -lcomutil

INCLUDEPATH += $${PROJECT_ROOT}/comutil/src
PRE_TARGETDEPS += $${COMUTIL_LIB_DIR}/libcomutil.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${COMUTIL_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(comutil_dep.pri)
