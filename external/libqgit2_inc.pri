# LibQGit2
LIBQGIT_DIR   = $${LIBRARY_ROOT}/LibQGit2/install
LIBQGIT_FILES = $${LIBQGIT_DIR}/bin/libgit2.dll \
                $${LIBQGIT_DIR}/bin/libqgit2.dll
LIBS       += -L$${LIBQGIT_DIR}/lib/ -lqgit2.dll
LIBS       += -L$${LIBQGIT_DIR}/lib/ -lgit2.dll
INCLUDEPATH +=  $${LIBQGIT_DIR}/include
DEPENDPATH  +=  $${LIBQGIT_DIR}/include

# register dlls to module/external
DEP_DLLS_EXTERNAL.files += $${LIBQGIT_FILES}

# load self dependecies
include(libqgit2_dep.pri)
