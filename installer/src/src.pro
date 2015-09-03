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


#----------------------------------------------------------------------------------------------
# re-create package configs in build dir
copy_configs.commands += if exist \"$$system_path($${DST_PACKAGE_DIR})\" rmdir \"$$system_path($${DST_PACKAGE_DIR})\" /s /q $$escape_expand(\n\t)
copy_configs.commands += xcopy /e /y \"$$system_path($${SRC_PACKAGE_DIR})\" \"$$system_path($${DST_PACKAGE_DIR}/)\"$$escape_expand(\n\t)
QMAKE_EXTRA_TARGETS += copy_configs


#----------------------------------------------------------------------------------------------
# copy files
installQtDlls(       $${DST_PACKAGE_DIR}/com.shoderitz.AccessCvs/data )
installModuleDlls(   $${DST_PACKAGE_DIR}/com.shoderitz.AccessCvs/data )
installExternalDlls( $${DST_PACKAGE_DIR}/com.shoderitz.AccessCvs/data )

copy_files.commands += $${QMAKE_PRE_LINK}
copy_files.depends  += copy_configs
QMAKE_EXTRA_TARGETS += copy_files
# clear variables
QMAKE_PRE_LINK =


#----------------------------------------------------------------------------------------------
# installer
INPUT = $${SRC_CONFIG_FILE} \
        $${SRC_PACKAGES_DIR}

#INSTALLER_OPTION = --online-only
INSTALLER_OPTION = --offline-only

installer.input     = INPUT
installer.output    = $${INSTALLER}
#installer.commands = C:/Qt/QtIFW2.0.1/bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
installer.commands += $${IFW_BIN_DIR}/binarycreator $${INSTALLER_OPTION} -c $${SRC_CONFIG_FILE} -p $${DST_PACKAGE_DIR} $${BUILD_TYPE}/${QMAKE_FILE_OUT}
installer.CONFIG   += combine no_link target_predeps
installer.depends  += copy_files copy_configs

QMAKE_EXTRA_COMPILERS += installer


#----------------------------------------------------------------------------------------------
# repository

repository.input     = INPUT
repository.output    = $${DST_REPOS_DIR}
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
    $$PWD/packages/com.shoderitz.AccessCvs/meta/package.xml \
    $$PWD/packages/com.shoderitz.AccessCvs/meta/installscript.qs \
    $$PWD/packages/com.shoderitz.AccessCvs/meta/cc0.txt \
    $$PWD/packages/com.shoderitz.AccessCvs/meta/gpl3.txt
