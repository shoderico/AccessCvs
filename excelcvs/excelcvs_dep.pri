# directly dependent libraries for this module
includeDepModule( excellib )
includeDepModule( resource )
includeDepModule( addin )
includeDepModule( util )
# controllers
#includeDepModule( accesscontroller )
includeDepModule( excelcvsaddincontroller )
includeDepModule( helpcontroller )


# directly dependent qt dlls for for this module
DEP_DLLS_QT.files += \
    $$[QT_INSTALL_BINS]/Qt5Gui$${DEP_DLLS_POSTFIX}.dll \
    $$[QT_INSTALL_BINS]/Qt5Widgets$${DEP_DLLS_POSTFIX}.dll \
    $$[QT_INSTALL_BINS]/Qt5Concurrent$${DEP_DLLS_POSTFIX}.dll \
    $$[QT_INSTALL_BINS]/Qt5Svg$${DEP_DLLS_POSTFIX}.dll \
    $$[QT_INSTALL_BINS]/Qt5WinExtras$${DEP_DLLS_POSTFIX}.dll

DEP_DLLS_QT_PLATFORMS.files += \
    $$[QT_INSTALL_PLUGINS]/platforms/qwindows$${DEP_DLLS_POSTFIX}.dll

export(DEP_DLLS_QT.files)
export(DEP_DLLS_QT_PLATFORMS.files)
