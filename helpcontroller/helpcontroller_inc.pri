# helpcontroller
HELPCONTROLLER_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
HELPCONTROLLER_BIN_DIR   = $${PROJECT_BINARY_DIR}
HELPCONTROLLER_FILES = $${HELPCONTROLLER_BIN_DIR}/helpcontroller.dll
LIBS +=          -L$${HELPCONTROLLER_LIB_DIR}/ -lhelpcontroller

INCLUDEPATH += $${PROJECT_ROOT}/helpcontroller/src
PRE_TARGETDEPS += $${HELPCONTROLLER_LIB_DIR}/libhelpcontroller.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${HELPCONTROLLER_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(helpcontroller_dep.pri)
