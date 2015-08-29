# addinutil
ADDINUTIL_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
ADDINUTIL_BIN_DIR   = $${PROJECT_BINARY_DIR}
ADDINUTIL_FILES = $${ADDINUTIL_BIN_DIR}/addinutil.dll
LIBS +=         -L$${ADDINUTIL_LIB_DIR}/ -laddinutil

INCLUDEPATH += $${PROJECT_ROOT}/addinutil/src
PRE_TARGETDEPS += $${ADDINUTIL_LIB_DIR}/libaddinutil.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${ADDINUTIL_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(addinutil_dep.pri)
