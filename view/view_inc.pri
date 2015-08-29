# view
VIEW_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
VIEW_BIN_DIR   = $${PROJECT_BINARY_DIR}
VIEW_FILES = $${VIEW_BIN_DIR}/view.dll
LIBS +=          -L$${VIEW_LIB_DIR}/ -lview

INCLUDEPATH += $${PROJECT_ROOT}/view/src
PRE_TARGETDEPS += $${VIEW_LIB_DIR}/libview.a

# register dlls to module/external
DEP_DLLS_MODULE.files += $${VIEW_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(view_dep.pri)
