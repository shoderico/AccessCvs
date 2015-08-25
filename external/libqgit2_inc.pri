# LibQGit2
LIBQGIT_DIR     = $${LIBRARY_ROOT}/LibQGit2/install
LIBQGIT_LIB_DIR = $${LIBQGIT_DIR}/lib
LIBQGIT_BIN_DIR = $${LIBQGIT_DIR}/bin
LIBQGIT_INC_DIR = $${LIBQGIT_DIR}/include

LIBQGIT_FILES = $${LIBQGIT_BIN_DIR}/libgit2.dll \
                $${LIBQGIT_BIN_DIR}/libqgit2.dll
LIBS += -L$${LIBQGIT_LIB_DIR}/ -lqgit2.dll
LIBS += -L$${LIBQGIT_LIB_DIR}/ -lgit2.dll
INCLUDEPATH += $${LIBQGIT_INC_DIR}
DEPENDPATH  += $${LIBQGIT_BIN_DIR}

# register this module dll files to dependent external files
DEP_DLLS_EXTERNAL.files += $${LIBQGIT_FILES}

# load self dependecies
include(libqgit2_dep.pri)
