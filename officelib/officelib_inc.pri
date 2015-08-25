# officelib
OFFICELIB_LIB_DIR = $${PROJECT_LIBRARY_DIR}
OFFICELIB_BIN_DIR = $${PROJECT_BINARY_DIR}
OFFICELIB_FILES   = $${OFFICELIB_BIN_DIR}/officelib.dll
LIBS           += -L$${OFFICELIB_LIB_DIR}/ -lofficelib

INCLUDEPATH += $${PROJECT_ROOT}/officelib/src

# register this module dll files to depenent module files
DEP_DLLS_MODULE.files += $${OFFICELIB_FILES}

# load self dependencies
include(officelib_dep.pri)
