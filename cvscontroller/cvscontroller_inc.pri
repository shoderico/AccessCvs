# cvscontroller
CVSCONTROLLER_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
CVSCONTROLLER_BIN_DIR   = $${PROJECT_BINARY_DIR}
CVSCONTROLLER_FILES = $${CVSCONTROLLER_BIN_DIR}/cvscontroller.dll
LIBS +=          -L$${CVSCONTROLLER_LIB_DIR}/ -lcvscontroller

INCLUDEPATH += $${PROJECT_ROOT}/cvscontroller/src
PRE_TARGETDEPS += $${CVSCONTROLLER_LIB_DIR}/libcvscontroller.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${CVSCONTROLLER_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(cvscontroller_dep.pri)
