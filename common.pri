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

DEP_DLLS_QT.files += \
    $$[QT_INSTALL_BINS]/libstdc++-6.dll \
    $$[QT_INSTALL_BINS]/libwinpthread-1.dll \
    $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
    $$[QT_INSTALL_BINS]/Qt5Core.dll


INCLUDEPATH += $${PROJECT_INCLUDE_DIR}




#----------------------------------------------------------------
# copy files
defineTest(copyFiles) {
    unset(arg)
    arg = $$1
    destdirs = $$eval($${arg}.destdirs)
    srcfiles = $$eval($${arg}.files)
    #message($$srcfiles)

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

#----------------------------------------------------------------
# inc.pri helper
defineTest(includeSharedLib) {
    unset(myTARGET)
    myTARGET = $$1
    myFILES  = $${PROJECT_BINARY_DIR}/$${myTARGET}.dll

    LIBS           += -L$${PROJECT_LIBRARY_DIR}/ -l$${myTARGET}
    INCLUDEPATH    += $${PROJECT_ROOT}/$${myTARGET}/src
    PRE_TARGETDEPS += $${PROJECT_LIBRARY_DIR}/lib$${myTARGET}.a
    export(LIBS)
    export(INCLUDEPATH)
    export(PRE_TARGETDEPS)

    # register dlls to module/external
    DEP_DLLS_MODULE.files += $${myFILES}
    #DEP_DLLS_EXTERNAL.files += $${myFILES}
    export(DEP_DLLS_MODULE.files)

    # load self dependencies
    include($${PROJECT_ROOT}/$${myTARGET}/$${myTARGET}_dep.pri)

    # export <MyTARGET>_FILES
    MyTARGET = $$upper($${myTARGET})
    eval($${MyTARGET}_FILES = $${myFILES})
    export($${MyTARGET}_FILES)
}
#----------------------------------------------------------------
defineTest(includeStaticLib) {
    unset(myTARGET)
    myTARGET = $$1

    LIBS           += -L$${PROJECT_LIBRARY_DIR}/ -l$${myTARGET}
    INCLUDEPATH    += $${PROJECT_ROOT}/$${myTARGET}/src
    PRE_TARGETDEPS += $${PROJECT_LIBRARY_DIR}/lib$${myTARGET}.a
    export(LIBS)
    export(INCLUDEPATH)
    export(PRE_TARGETDEPS)

    # load self dependencies
    include($${PROJECT_ROOT}/$${myTARGET}/$${myTARGET}_dep.pri)
}
#----------------------------------------------------------------
defineTest(includeFinalModule) {
    unset(myTARGET)
    unset(myEXT)
    myTARGET = $$1
    myEXT    = $$2
    myFILES  = $${PROJECT_BINARY_DIR}/$${myTARGET}.$${myEXT}

    #LIBS          += -L$${PROJECT_LIBRARY_DIR}/ -l$${myTARGET}
    INCLUDEPATH    += $${PROJECT_ROOT}/$${myTARGET}/src
    PRE_TARGETDEPS += $${PROJECT_BINARY_DIR}/$${myTARGET}.$${myEXT} # spacial case
    export(INCLUDEPATH)
    export(PRE_TARGETDEPS)

    # register dlls to module/external
    DEP_DLLS_MODULE.files += $${myFILES}
    #DEP_DLLS_EXTERNAL.files += $${}
    export(DEP_DLLS_MODULE.files)

    # load self dependencies
    include($${PROJECT_ROOT}/$${myTARGET}/$${myTARGET}_dep.pri)

    # export <MyTARGET>_FILES
    MyTARGET = $$upper($${myTARGET})
    eval($${MyTARGET}_FILES = $${myFILES})
    export($${MyTARGET}_FILES)
}



#----------------------------------------------------------------
# dep.pri helper
defineTest(includeDepModule) {
    unset(myTARGET)
    myTARGET = $$1
    include($${PROJECT_ROOT}/$${myTARGET}/$${myTARGET}_inc.pri)
}
