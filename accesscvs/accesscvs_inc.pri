# accesscvs
ACCESSCVS_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
ACCESSCVS_BIN_DIR   = $${PROJECT_BINARY_DIR}
ACCESSCVS_FILES = $${ACCESSCVS_BIN_DIR}/accesscvs.dll
#LIBS +=          -L$${ACCESSCVS_LIB_DIR}/ -laccesscvs

INCLUDEPATH += $${PROJECT_ROOT}/accesscvs/src
PRE_TARGETDEPS += $${ACCESSCVS_BIN_DIR}/accesscvs.dll # spacial case

# register dlls to module/external
DEP_DLLS_MODULE.files += $${ACCESSCVS_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(accesscvs_dep.pri)

# copy this module's dll to bin directory again.
# workaround for :
#    qaxserver's post_link runs AFTER other QMAKE_POST_LINKs in pro.
#    In other words, even if we set QMAKE_POST_LINK to copy dll in .pro, correct dll will not copied.
#    so we will copy correct dll by includer of this module.
QMAKE_PRE_LINK += $(COPY) \"$$system_path($${OUT_PWD}/../../accesscvs/src/$${BUILD_TYPE}/accesscvs.dll)\" \"$$system_path($${PROJECT_BINARY_DIR})\"$$escape_expand(\n\t)
