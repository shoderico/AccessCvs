# directly dependent libraries for this module
include($${PROJECT_ROOT}/officelib/officelib_inc.pri)
include($${PROJECT_ROOT}/util/util_inc.pri)
include($${PROJECT_ROOT}/accessutil/accessutil_inc.pri)
include($${PROJECT_ROOT}/external/libqgit2_inc.pri)
include($${PROJECT_ROOT}/external/qtsolutions_inc.pri)

# directly dependent qt dlls for for this module
DEP_DLLS_QT.files += \
    $$[QT_INSTALL_BINS]/Qt5Gui.dll \
    $$[QT_INSTALL_BINS]/Qt5Widgets.dll \
    $$[QT_INSTALL_BINS]/Qt5Concurrent.dll \
    $$[QT_INSTALL_BINS]/Qt5Svg.dll \
    $$[QT_INSTALL_BINS]/Qt5WinExtras.dll

DEP_DLLS_QT_PLATFORMS.files += \
    $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll

