# QtSolutions
QTSOLUTIONS_DIR     = $${LIBRARY_ROOT}/QtSolutions/qtwinmigrate
QTSOLUTIONS_BIN_DIR = $${QTSOLUTIONS_DIR}/lib
QTSOLUTIONS_FILES   = $${QTSOLUTIONS_BIN_DIR}/QtSolutions_MFCMigrationFramework-head.dll
include($${QTSOLUTIONS_DIR}/src/qtwinmigrate.pri)

# register this module dll files to dependent external files
DEP_DLLS_EXTERNAL.files += $${QTSOLUTIONS_FILES}

# load self dependencies
include(qtsolutions_dep.pri)
