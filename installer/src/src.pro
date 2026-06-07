TEMPLATE = aux

TARGET    = installer

include(../../qmake/common.pri)
#include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

# clear unecessary variables
LIBS =
#PRE_TARGETDEPS =


#----------------------------------------------------------------------------------------------
# build timestamp (yyyyMMdd_hhmmss)
BUILD_TIMESTAMP = $$system("powershell -NoProfile -Command \"(Get-Date).ToString('yyyyMMdd_HHmmss')\"")
message($${BUILD_TIMESTAMP})

#----------------------------------------------------------------------------------------------
# installer name
INSTALLER = AccessCvsInstaller_$${APP_VERSION}_$${APP_ARCH_TYPE}_$${BUILD_IDENTIFIER}_$${BUILD_TYPE}_$${BUILD_TIMESTAMP}
INSTALLER_ONLINE = $${INSTALLER}_online
INSTALLER_OFFLINE = $${INSTALLER}_offline



#----------------------------------------------------------------------------------------------
# directories and files
Release|Debug:BUILD_DIR = $${BUILD_ROOT}/$${BUILD_TYPE}/installer
DST_PACKAGE_DIR  = $${BUILD_DIR}/packages
SRC_CONFIG_DIR   = $${PWD}/config
SRC_PACKAGE_DIR  = $${PWD}/packages

SRC_CONFIG_FILE  = $${SRC_CONFIG_DIR}/config.xml

IFW_BIN_DIR      = C:/Qt/QtIFW2.0.3/bin

SRC_REPOS_DIR    = $${PWD}/repository
DST_REPOS_DIR    = $${BUILD_DIR}/repository

PKG_ACCESSCVS_DIR = com.shoderitz.AccessCvs
PKG_ACCESSCVS_VER = APP_VERSION


#----------------------------------------------------------------------------------------------
# process *.in files
processConfigureIn( $${SRC_CONFIG_FILE} )
processConfigureIn( $${SRC_PACKAGE_DIR}/$${PKG_ACCESSCVS_DIR}/meta/package.xml )


#----------------------------------------------------------------------------------------------
# re-create package configs in build dir
copy_configs.commands += if exist \"$$system_path($${DST_PACKAGE_DIR})\" rmdir \"$$system_path($${DST_PACKAGE_DIR})\" /s /q $$escape_expand(\n\t)
copy_configs.commands += xcopy /e /y \"$$system_path($${SRC_PACKAGE_DIR})\" \"$$system_path($${DST_PACKAGE_DIR}/)\"$$escape_expand(\n\t)
QMAKE_EXTRA_TARGETS += copy_configs


#----------------------------------------------------------------------------------------------
# copy files
win32-g++ {
    DEP_DLLS_QT.files += \
        $$[QT_INSTALL_BINS]/libstdc++-6.dll \
        $$[QT_INSTALL_BINS]/libwinpthread-1.dll \
        $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll
}

installQtDlls(       $${DST_PACKAGE_DIR}/$${PKG_ACCESSCVS_DIR}/data )
installModuleDlls(   $${DST_PACKAGE_DIR}/$${PKG_ACCESSCVS_DIR}/data )
installExternalDlls( $${DST_PACKAGE_DIR}/$${PKG_ACCESSCVS_DIR}/data )

copy_files.commands += $${QMAKE_PRE_LINK}
QMAKE_PRE_LINK = # clear variables
copy_files.depends  += copy_configs
QMAKE_EXTRA_TARGETS += copy_files


#----------------------------------------------------------------------------------------------
# installer
INPUT = $${SRC_CONFIG_FILE} \
        $${SRC_PACKAGES_DIR}

installer_online.input     = INPUT
installer_online.output    = $${INSTALLER_ONLINE}
installer_online.commands += $${IFW_BIN_DIR}/binarycreator --online-only -c $${SRC_CONFIG_FILE} -p $${DST_PACKAGE_DIR} $${BUILD_TYPE}/${QMAKE_FILE_OUT}
installer_online.CONFIG   += combine no_link target_predeps
installer_online.depends  += copy_files copy_configs

QMAKE_EXTRA_COMPILERS += installer_online

installer_offline.input     = INPUT
installer_offline.output    = $${INSTALLER_OFFLINE}
installer_offline.commands += $${IFW_BIN_DIR}/binarycreator --offline-only -c $${SRC_CONFIG_FILE} -p $${DST_PACKAGE_DIR} $${BUILD_TYPE}/${QMAKE_FILE_OUT}
installer_offline.CONFIG   += combine no_link target_predeps
installer_offline.depends  += copy_files copy_configs

QMAKE_EXTRA_COMPILERS += installer_offline


#----------------------------------------------------------------------------------------------
# repository

REPOS_OUTPUT += \
    $${DST_REPOS_DIR}/Updates.xml \
    $${DST_REPOS_DIR}/$${PKG_ACCESSCVS_DIR}/$${PKG_ACCESSCVS_VER}content.7z \
    $${DST_REPOS_DIR}/$${PKG_ACCESSCVS_DIR}/$${PKG_ACCESSCVS_VER}content.7z.sha1 \
    $${DST_REPOS_DIR}/$${PKG_ACCESSCVS_DIR}/$${PKG_ACCESSCVS_VER}meta.7z \
    $${DST_REPOS_DIR}/$${PKG_ACCESSCVS_DIR}/$${PKG_ACCESSCVS_VER}platforms.7z \
    $${DST_REPOS_DIR}/$${PKG_ACCESSCVS_DIR}/$${PKG_ACCESSCVS_VER}platforms.7z.sha1 \

repository.input     = INPUT
repository.output    = $${DST_REPOS_DIR}/Updates.xml
repository.clean     = $${REPOS_OUTPUT}
repository.commands += $${IFW_BIN_DIR}/repogen --remove --packages $${DST_PACKAGE_DIR} $${DST_REPOS_DIR}
repository.CONFIG   += combine no_link target_predeps
repository.depends  += copy_files copy_configs

QMAKE_EXTRA_COMPILERS += repository

#--packages <directory> : Use <directory> as the package directory. This is mandatory.
#--update-new-packages  : Update only packages that are new or have a newer version. The list can be further filtered with the -i, -e parameters.
#--remove               : Force removal of existing target directory before generating it again.


QMAKE_POST_LINK += $(COPY) \"$$system_path($${OUT_PWD}/$${BUILD_TYPE}/$${INSTALLER_ONLINE}.exe)\" \"$$system_path($${BUILD_DIR})\"$$escape_expand(\n\t)
QMAKE_POST_LINK += $(COPY) \"$$system_path($${OUT_PWD}/$${BUILD_TYPE}/$${INSTALLER_OFFLINE}.exe)\" \"$$system_path($${BUILD_DIR})\"$$escape_expand(\n\t)


# other files
OTHER_FILES = \
    $$PWD/README \
    $$PWD/config/config.xml.in \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/package.xml.in \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/installscript.qs \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/cc0.txt \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/gpl3.txt \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/data/install_accesscvs.bat \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/data/uninstall_accesscvs.bat \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/data/uninstall_accesscvs.reg
