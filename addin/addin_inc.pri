# addin
ADDIN_LIB_DIR = $${PROJECT_LIBRARY_DIR}
ADDIN_BIN_DIR = $${PROJECT_BINARY_DIR}
#ADDIN_FILES   = $${ADDIN_BIN_DIR}/addin.dll
LIBS +=       -L$${ADDIN_LIB_DIR}/ -laddin

INCLUDEPATH += $${PROJECT_ROOT}/addin/src
PRE_TARGETDEPS += $${ADDIN_LIB_DIR}/libaddin.a

DEF_FILE = $${PROJECT_ROOT}/addin/src/addin/distfile/addin.def
RC_FILE  = $${PROJECT_ROOT}/addin/src/addin/distfile/addin.rc
#DISTFILES += \
#    $${PROJECT_ROOT}/addin/src/addin/addin.def \
#    $${PROJECT_ROOT}/addin/src/addin/addin.rc \
#    $${PROJECT_ROOT}/addin/src/addin/ribboncallback.idl

# register dlls to module/external
#DEP_DLLS_MODULE.files += $${ADDIN_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(addin_dep.pri)
