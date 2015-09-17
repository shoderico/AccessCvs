TEMPLATE = aux

# installer name
INSTALLER = AccessCvsInstaller
TARGET    = installer

include(../../common.pri)
#include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

# clear unecessary variables
LIBS =
#PRE_TARGETDEPS =


#----------------------------------------------------------------------------------------------
# directories and files
BUILD_DIR        = $${BUILD_ROOT}/$${BUILD_TYPE}/installer
DST_PACKAGE_DIR  = $${BUILD_DIR}/packages
SRC_CONFIG_DIR   = $${PWD}/config
SRC_PACKAGE_DIR  = $${PWD}/packages

SRC_CONFIG_FILE  = $${SRC_CONFIG_DIR}/config.xml

IFW_BIN_DIR      = C:/Qt/QtIFW2.0.1/bin

SRC_REPOS_DIR    = $${PWD}/repository
DST_REPOS_DIR    = $${BUILD_DIR}/repository

PKG_ACCESSCVS_DIR = com.shoderitz.AccessCvs
PKG_ACCESSCVS_VER = 1.0.3


#----------------------------------------------------------------------------------------------
# re-create package configs in build dir
copy_configs.commands += if exist \"$$system_path($${DST_PACKAGE_DIR})\" rmdir \"$$system_path($${DST_PACKAGE_DIR})\" /s /q $$escape_expand(\n\t)
copy_configs.commands += xcopy /e /y \"$$system_path($${SRC_PACKAGE_DIR})\" \"$$system_path($${DST_PACKAGE_DIR}/)\"$$escape_expand(\n\t)
QMAKE_EXTRA_TARGETS += copy_configs


#----------------------------------------------------------------------------------------------
# copy files
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

INSTALLER_OPTION = --online-only
#INSTALLER_OPTION = --offline-only

installer.input     = INPUT
installer.output    = $${INSTALLER}
#installer.commands = C:/Qt/QtIFW2.0.1/bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
installer.commands += $${IFW_BIN_DIR}/binarycreator $${INSTALLER_OPTION} -c $${SRC_CONFIG_FILE} -p $${DST_PACKAGE_DIR} $${BUILD_TYPE}/${QMAKE_FILE_OUT}
installer.CONFIG   += combine no_link target_predeps
installer.depends  += copy_files copy_configs

QMAKE_EXTRA_COMPILERS += installer


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


QMAKE_POST_LINK += $(COPY) \"$$system_path($${OUT_PWD}/$${BUILD_TYPE}/$${INSTALLER}.exe)\" \"$$system_path($${BUILD_DIR})\"$$escape_expand(\n\t)


# other files
OTHER_FILES = \
    $$PWD/README \
    $$PWD/config/config.xml \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/package.xml \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/installscript.qs \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/cc0.txt \
    $$PWD/packages/$${PKG_ACCESSCVS_DIR}/meta/gpl3.txt
