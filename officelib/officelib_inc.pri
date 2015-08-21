# officelib
OFFICELIB_DIR   = $${PROJECT_LIBRARY_DIR}
OFFICELIB_FILES = $${OFFICELIB_DIR}/officelib.dll
LIBS +=         -L$${OFFICELIB_DIR}/ -lofficelib

# register dlls to module/external
DEP_DLLS_MODULE.files += $${OFFICELIB_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(officelib_dep.pri)
