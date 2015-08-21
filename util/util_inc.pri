# util
UTIL_DIR   = $${PROJECT_LIBRARY_DIR}
UTIL_FILES = $${UTIL_DIR}/util.dll
LIBS +=    -L$${UTIL_DIR}/ -lutil

# register dlls to module/external
DEP_DLLS_MODULE.files += $${UTIL_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(util_dep.pri)
