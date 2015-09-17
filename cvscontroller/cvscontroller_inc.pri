# cvscontroller
unset(MY_TARGET)
unset(MY_FILES)
MY_TARGET = cvscontroller
MY_FILES  = $${PROJECT_BINARY_DIR}/$${MY_TARGET}.dll
CVSCONTROLLER_FILES = $${MY_FILES}


LIBS           += -L$${PROJECT_LIBRARY_DIR}/ -l$${MY_TARGET}
INCLUDEPATH    += $${PROJECT_ROOT}/$${MY_TARGET}/src
PRE_TARGETDEPS += $${PROJECT_LIBRARY_DIR}/lib$${MY_TARGET}.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${MY_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include($${MY_TARGET}_dep.pri)
