# accutil
ACCUTIL_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
ACCUTIL_BIN_DIR   = $${PROJECT_BINARY_DIR}
ACCUTIL_FILES = $${ACCUTIL_BIN_DIR}/accutil.exe
#LIBS +=          -L$${ACCUTIL_LIB_DIR}/ -laccutil

INCLUDEPATH += $${PROJECT_ROOT}/accutil/src
PRE_TARGETDEPS += $${ACCUTIL_BIN_DIR}/accutil.exe # spacial case

# register dlls to module/external
DEP_DLLS_MODULE.files += $${ACCUTIL_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(accutil_dep.pri)
