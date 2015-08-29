# accessutil
ACCESSUTILCONTROLLER_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
ACCESSUTILCONTROLLER_BIN_DIR   = $${PROJECT_BINARY_DIR}
ACCESSUTILCONTROLLER_FILES = $${ACCESSUTILCONTROLLER_BIN_DIR}/accessutilcontroller.dll
LIBS +=          -L$${ACCESSUTILCONTROLLER_LIB_DIR}/ -laccessutilcontroller

INCLUDEPATH += $${PROJECT_ROOT}/accessutilcontroller/src
PRE_TARGETDEPS += $${ACCESSUTILCONTROLLER_LIB_DIR}/libaccessutilcontroller.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${ACCESSUTILCONTROLLER_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(accessutilcontroller_dep.pri)
