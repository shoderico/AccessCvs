# directly dependent libraries for this module
include($${PROJECT_ROOT}/accmodel/accmodel_inc.pri)

# directly dependent qt dlls for this module
DEP_DLLS_QT.files += \
    $$[QT_INSTALL_BINS]/Qt5Gui.dll \
    $$[QT_INSTALL_BINS]/Qt5Widgets.dll
#DEP_DLLS_QT_PLATFORMS +=
