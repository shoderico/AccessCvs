# qtsolutions_paths.pri
# Definition-only file for QtSolutions / qtwinmigrate.
# This file contains only path and filename definitions.
# No side effects (LIBS, DEFINES, export, include, etc.) are introduced here.
# It can be safely included from the top-level AccessCvs.pro.

# --- Directory definitions ---
isEmpty(LIBRARY_ROOT) {
    # Fallback when included directly from top-level AccessCvs.pro etc.
    # (for cases where common.pri has not been loaded yet)
    QTSOLUTIONS_DIR = $$PWD/../../qt-solutions/qtwinmigrate
} else {
    QTSOLUTIONS_DIR = $${LIBRARY_ROOT}/qt-solutions/qtwinmigrate
}

# Reference build artifacts directly (no pre-placed lib_msvc_* folders)
QTSOLUTIONS_LIB_DIR = $${QTSOLUTIONS_DIR}/lib
QTSOLUTIONS_BIN_DIR = $${QTSOLUTIONS_DIR}/lib
QTSOLUTIONS_INC_DIR = $${QTSOLUTIONS_DIR}/src

# --- Config-specific postfix (embed the logic from common.pri to make this file self-contained) ---
Debug {
    DEP_DLLS_POSTFIX = d
}
Release {
    DEP_DLLS_POSTFIX =
}

# --- Filename definitions (derived from QTSOLUTIONS_LIBNAME_BASE) ---
QTSOLUTIONS_LIBNAME_BASE = QtSolutions_MFCMigrationFramework-head
QTSOLUTIONS_LIBNAME      = $${QTSOLUTIONS_LIBNAME_BASE}$${DEP_DLLS_POSTFIX}
QTSOLUTIONS_FILES        = $${QTSOLUTIONS_BIN_DIR}/$${QTSOLUTIONS_LIBNAME}.dll

# --- Files to clean (covers both release/debug + .lib/.exp using wildcards) ---
QTSOLUTIONS_CLEAN_FILES = \
    $${QTSOLUTIONS_LIB_DIR}/$${QTSOLUTIONS_LIBNAME_BASE}*.dll \
    $${QTSOLUTIONS_LIB_DIR}/$${QTSOLUTIONS_LIBNAME_BASE}*.lib \
    $${QTSOLUTIONS_LIB_DIR}/$${QTSOLUTIONS_LIBNAME_BASE}*.exp
