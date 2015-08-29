# cvsmodel
CVSMODEL_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
CVSMODEL_BIN_DIR   = $${PROJECT_BINARY_DIR}
CVSMODEL_FILES = $${CVSMODEL_BIN_DIR}/cvsmodel.dll
LIBS +=          -L$${CVSMODEL_LIB_DIR}/ -lcvsmodel

INCLUDEPATH += $${PROJECT_ROOT}/cvsmodel/src
PRE_TARGETDEPS += $${CVSMODEL_LIB_DIR}/libcvsmodel.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${CVSMODEL_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(cvsmodel_dep.pri)
