# accessutil
ACCESSUTIL_DIR   = $${PROJECT_LIBRARY_DIR}
ACCESSUTIL_FILES = $${ACCESSUTIL_DIR}/accessutil.dll
LIBS +=          -L$${ACCESSUTIL_DIR}/ -laccessutil

# register dlls to module/external
DEP_DLLS_MODULE.files += $${ACCESSUTIL_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(accessutil_dep.pri)
