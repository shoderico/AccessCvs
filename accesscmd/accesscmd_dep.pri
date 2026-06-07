# directly dependent libraries for this module
includeDepModule( accessmodel )


# directly dependent qt dlls for this module
DEP_DLLS_QT.files += \
    $$[QT_INSTALL_BINS]/Qt5Gui.dll \
    $$[QT_INSTALL_BINS]/Qt5Widgets.dll
#DEP_DLLS_QT_PLATFORMS +=
export(DEP_DLLS_QT.files)
