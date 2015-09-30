# directly dependent libraries for this module
includeDepModule( officelib )
includeDepModule( comutil )
includeDepModule( addinutil )
include($${PROJECT_ROOT}/external/qtsolutions_inc.pri)


# directly dependent qt dlls for this module
#DEP_DLLS_QT.files +=
#DEP_DLLS_QT_PLATFORMS +=

# disable "warning: ignoring #pragma warning"
QMAKE_CXXFLAGS += -Wno-unknown-pragmas
QMAKE_CFLAGS += -Wno-unknown-pragmas
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas

export(QMAKE_CXXFLAGS)
export(QMAKE_CFLAGS)
