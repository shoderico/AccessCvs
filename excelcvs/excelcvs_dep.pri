# directly dependent libraries for this module
includeDepModule( excellib )
includeDepModule( resource )
includeDepModule( addin )
includeDepModule( util )
# controllers
#includeDepModule( acccontroller )
#includeDepModule( cvscontroller )
includeDepModule( helpcontroller )


# directly dependent qt dlls for for this module
DEP_DLLS_QT.files += \
    $$[QT_INSTALL_BINS]/Qt5Gui.dll \
    $$[QT_INSTALL_BINS]/Qt5Widgets.dll \
    $$[QT_INSTALL_BINS]/Qt5Concurrent.dll \
    $$[QT_INSTALL_BINS]/Qt5Svg.dll \
    $$[QT_INSTALL_BINS]/Qt5WinExtras.dll

DEP_DLLS_QT_PLATFORMS.files += \
    $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll

export(DEP_DLLS_QT.files)
export(DEP_DLLS_QT_PLATFORMS.files)
