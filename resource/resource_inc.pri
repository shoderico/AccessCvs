# resource
RESOURCE_LIB_DIR   = $${PROJECT_LIBRARY_DIR}
RESOURCE_BIN_DIR   = $${PROJECT_BINARY_DIR}
RESOURCE_FILES = $${RESOURCE_BIN_DIR}/resource.dll
LIBS +=          -L$${RESOURCE_LIB_DIR}/ -lresource

INCLUDEPATH += $${PROJECT_ROOT}/resource/src

# register dlls to module/external
DEP_DLLS_MODULE.files += $${RESOURCE_FILES}
#DEP_DLLS_EXTERNAL.files += $${}

# load self dependencies
include(resource_dep.pri)
