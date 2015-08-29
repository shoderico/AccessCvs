# directly dependent libraries for this module
include($${PROJECT_ROOT}/officelib/officelib_inc.pri)

# directly dependent qt dlls for this module
#DEP_DLLS_QT.files +=
#DEP_DLLS_QT_PLATFORMS +=

# disable "warning: ignoring #pragma warning"
QMAKE_CXXFLAGS += -Wno-unknown-pragmas
QMAKE_CFLAGS += -Wno-unknown-pragmas
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas
