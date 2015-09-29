# inc.pri : normal module
# accmodel
unset(MY_TARGET)
unset(MY_FILES)
MY_TARGET = accmodel
MY_FILES  = $${PROJECT_BINARY_DIR}/$${MY_TARGET}.dll
ACCMODEL_FILES = $${MY_FILES}


LIBS           += -L$${PROJECT_LIBRARY_DIR}/ -l$${MY_TARGET}
INCLUDEPATH    += $${PROJECT_ROOT}/$${MY_TARGET}/src
PRE_TARGETDEPS += $${PROJECT_LIBRARY_DIR}/lib$${MY_TARGET}.a

# register this module dll files to dependent module files
DEP_DLLS_MODULE.files += $${MY_FILES}

# load self dependencies
include($${MY_TARGET}_dep.pri)
