# accesscvs
unset(MY_TARGET)
unset(MY_FILES)
MY_TARGET = accesscvs
MY_FILES  = $${PROJECT_BINARY_DIR}/$${MY_TARGET}.dll
ACCESSCVS_FILES = $${MY_FILES}


#LIBS          += -L$${PROJECT_LIBRARY_DIR}/ -l$${MY_TARGET}
INCLUDEPATH    += $${PROJECT_ROOT}/$${MY_TARGET}/src
PRE_TARGETDEPS += $${PROJECT_BINARY_DIR}/$${MY_TARGET}.dll # spacial case

# register dlls to module/external
DEP_DLLS_MODULE.files += $${MY_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# copy this module's dll to bin directory again.
# workaround for :
#    qaxserver's post_link runs AFTER other QMAKE_POST_LINKs in pro.
#    In other words, even if we set QMAKE_POST_LINK to copy dll in .pro, correct dll will not copied.
#    so we will copy correct dll by includer of this module.
QMAKE_PRE_LINK += $(COPY) \"$$system_path($${OUT_PWD}/../../$${MY_TARGET}/src/$${BUILD_TYPE}/$${MY_TARGET}.dll)\" \"$$system_path($${PROJECT_BINARY_DIR})\"$$escape_expand(\n\t)

# load self dependencies
include($${MY_TARGET}_dep.pri)
