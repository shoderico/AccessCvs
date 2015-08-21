# QtSolutions
QTSOLUTIONS_DIR   = $${LIBRARY_ROOT}/QtSolutions/qtwinmigrate
QTSOLUTIONS_FILES = $${QTSOLUTIONS_DIR}/lib/QtSolutions_MFCMigrationFramework-head.dll
include($${QTSOLUTIONS_DIR}/src/qtwinmigrate.pri)

# register dlls to module/external
DEP_DLLS_EXTERNAL.files += $${QTSOLUTIONS_FILES}

# load self dependencies
include(qtsolutions_dep.pri)
