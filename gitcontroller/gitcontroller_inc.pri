# gitcontroller
GITCONTROLLER_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
GITCONTROLLER_BIN_DIR   = $${PROJECT_BINARY_DIR}
GITCONTROLLER_FILES = $${GITCONTROLLER_BIN_DIR}/gitcontroller.dll
LIBS +=          -L$${GITCONTROLLER_LIB_DIR}/ -lgitcontroller

INCLUDEPATH += $${PROJECT_ROOT}/gitcontroller/src
PRE_TARGETDEPS += $${GITCONTROLLER_LIB_DIR}/libgitcontroller.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${GITCONTROLLER_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(gitcontroller_dep.pri)
