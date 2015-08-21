PROJECT_ROOT = $$PWD
LIBRARY_ROOT = $$PWD/..
BUILD_ROOT = $${PROJECT_ROOT}/build
Release:BUILD_TYPE = release
Debug:  BUILD_TYPE = debug
#export(BUILD_TYPE)
#BUILD_TYPE = $${BUILD_TYPE}
OBJECTS_DIR = $${BUILD_TYPE}/.obj
MOC_DIR     = $${BUILD_TYPE}/.moc
RCC_DIR     = $${BUILD_TYPE}/.rcc
UI_DIR      = $${BUILD_TYPE}/.ui

PROJECT_INCLUDE_DIR     = $${BUILD_ROOT}/include
PROJECT_INCLUDE_DIR_WNT = $${PROJECT_INCLUDE_DIR}
PROJECT_INCLUDE_DIR_WNT ~= s,/,\\,g

PROJECT_LIBRARY_DIR     = $${BUILD_ROOT}/$${BUILD_TYPE}/lib
PROJECT_LIBRARY_DIR_WNT = $${PROJECT_LIBRARY_DIR}
PROJECT_LIBRARY_DIR_WNT ~= s,/,\\,g

PROJECT_BINARY_DIR = $${BUILD_ROOT}/$${BUILD_TYPE}/bin
PROJECT_BINARY_DIR_WNT = $${PROJECT_BINARY_DIR}
PROJECT_BINARY_DIR_WNT ~= s,/,\\,g

OUT_PWD_WNT = $${OUT_PWD}
OUT_PWD_WNT ~= s,/,\\,g

PWD_WNT = $${_PRO_FILE_PWD_}
PWD_WNT ~= s,/,\\,g

# message($${PWD}) # return this file's dir
# message($${OUT_PWD}) # return caller's dir
# message($$_PRO_FILE_PWD_) # return caller's pro dir
# message($${TARGET}) # return caller's target
# for (h,HEADERS) {
#    message($${h})
# } # return caller's headers


DEP_DLLS_QT.destdir = .
DEP_DLLS_QT_PLATFORMS.destdir = platforms
DEP_DLLS_MODULE.destdir = .
DEP_DLLS_EXTERNAL.destdir = .






#----------------------------------------------------------------
# copy files
defineTest(copyFiles) {
    unset(arg)
    arg = $$1
    destdirs = $$eval($${arg}.destdirs)
    srcfiles = $$eval($${arg}.files)

    for (destdir, destdirs) {
        QMAKE_PRE_LINK += $(CHK_DIR_EXISTS) $$system_path($${destdir}) $(MKDIR) $$system_path($${destdir})$$escape_expand(\n\t)
        for (file, srcfiles) {
            QMAKE_PRE_LINK += $(COPY) \"$$system_path($${file})\" \"$$system_path($${destdir})\"$$escape_expand(\n\t)
        }
    }
    export(QMAKE_PRE_LINK)
}

#----------------------------------------------------------------
# installQtDlls
defineTest(installQtDlls) {
    unset(destdirs)
    destdirs = $$1
    installDlls( $$destdirs, DEP_DLLS_QT)
    installDlls( $$destdirs, DEP_DLLS_QT_PLATFORMS)
}
defineTest(installModuleDlls) {
    unset(destdirs)
    destdirs = $$1
    installDlls( $$destdirs, DEP_DLLS_MODULE)
}
defineTest(installExternalDlls) {
    unset(destdirs)
    destdirs = $$1
    installDlls( $$destdirs, DEP_DLLS_EXTERNAL)
}
defineTest(installDlls) {
    unset(destdirs)
    destdirs = $$1
    unset(target)
    target = $$2

    unset(toBeCopied)
    for (destdir, destdirs) {
        toBeCopied.destdirs += $${destdir}/$$eval($${target}.destdir)
    }
    toBeCopied.files = $$eval($${target}.files)
    copyFiles(toBeCopied)
}




