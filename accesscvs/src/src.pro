#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T22:28:05
#
#-------------------------------------------------

QT += gui
QT += axserver
QT += axcontainer
QT += winextras
QT += concurrent
QT += svg
CONFIG += dll
#CONFIG += qaxserver_no_postlink
CONFIG += c++11

LIBS += -loleacc

TARGET   = AccessCvs
TEMPLATE = lib
#VERSION  = 1.0

DEF_FILE = addin/addin.def
RC_FILE  = addin/addin.rc

DEFINES += ACCESSCVS_LIBRARY
DEFINES += QT_MESSAGELOGCONTEXT

include(../../common.pri)

INCLUDEPATH += $${PROJECT_INCLUDE_DIR}


# directory where *.dll output to
DESTDIR = $${BUILD_ROOT}/$${BUILD_TYPE}/bin
DESTDIR_WNT = $${DESTDIR}
DESTDIR_WNT ~= s,/,\\,g



# officelib
OFFICELIB_DIR = $${PROJECT_LIBRARY_DIR}
LIBS += -L$${OFFICELIB_DIR}/ -lofficelib

# util
UTIL_DIR = $${PROJECT_LIBRARY_DIR}
LIBS += -L$${UTIL_DIR}/ -lutil


# LibQGit2
LIBQGIT_DIR = $${LIBRARY_ROOT}/LibQGit2/install
LIBS        += -L$${LIBQGIT_DIR}/lib/ -lqgit2.dll
LIBS        += -L$${LIBQGIT_DIR}/lib/ -lgit2.dll
INCLUDEPATH +=   $${LIBQGIT_DIR}/include
DEPENDPATH  +=   $${LIBQGIT_DIR}/include


# QtSolutions
QTSOLUTIONS_DIR = $${LIBRARY_ROOT}/QtSolutions/qtwinmigrate
include($${QTSOLUTIONS_DIR}/src/qtwinmigrate.pri)

# OpenSSL
OPENSSL_DIR = C:/OpenSSL-Win32

# Depend : *.dll
DEPEND_DLL_FILES += \
    $$[QT_INSTALL_BINS]/libstdc++-6.dll \
    $$[QT_INSTALL_BINS]/libwinpthread-1.dll \
    $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
    $$[QT_INSTALL_BINS]/Qt5Concurrent.dll \
    $$[QT_INSTALL_BINS]/Qt5Core.dll \
    $$[QT_INSTALL_BINS]/Qt5Gui.dll \
    $$[QT_INSTALL_BINS]/Qt5Svg.dll \
    $$[QT_INSTALL_BINS]/Qt5Widgets.dll \
    $$[QT_INSTALL_BINS]/Qt5WinExtras.dll \
    $${QTSOLUTIONS_DIR}/lib/QtSolutions_MFCMigrationFramework-head.dll \

DEPEND_DLL_EXTERNAL_FILES += \
    $${OPENSSL_DIR}/libeay32.dll \
    $${OPENSSL_DIR}/ssleay32.dll \
    $${LIBQGIT_DIR}/bin/libgit2.dll \
    $${LIBQGIT_DIR}/bin/libqgit2.dll \
    $${OFFICELIB_DIR}/officelib.dll \
    $${UTIL_DIR}/util.dll

# copy dlls to DESTDIR
DEPEND_DLL_FILES_WNT = $${DEPEND_DLL_FILES} $${DEPEND_DLL_EXTERNAL_FILES}
DEPEND_DLL_FILES_WNT ~= s,/,\\,g
for(FILE,DEPEND_DLL_FILES_WNT){
    QMAKE_PRE_LINK += copy /y \"$${FILE}\" $${DESTDIR_WNT}$$escape_expand(\n\t)
}

# for idc.exe, we have to copy dlls to OUT_PWD
DEPEND_DLL_EXTERNAL_FILES_WNT = $${DEPEND_DLL_EXTERNAL_FILES}
DEPEND_DLL_EXTERNAL_FILES_WNT ~= s,/,\\,g
for(f,DEPEND_DLL_EXTERNAL_FILES_WNT) {
    QMAKE_PRE_LINK += $(COPY) \"$${f}\" $${OUT_PWD_WNT}$$escape_expand(\n\t)
}


# Depend : platforms/*.dll
DEPEND_PLUGIN_FILES += \
    $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll

# copy platforms/*.dll to DESTDIR
DEPEND_PLUGIN_FILES_COPY = $${DEPEND_PLUGIN_FILES}
DEPEND_PLUGIN_FILES_COPY ~= s,/,\\,g
DESTDIR_PLUGIN_COPY = $${DESTDIR}/platforms
DESTDIR_PLUGIN_COPY ~= s,/,\\,g
QMAKE_PRE_LINK += $(CHK_DIR_EXISTS) $${DESTDIR_PLUGIN_COPY} $(MKDIR) $${DESTDIR_PLUGIN_COPY}$$escape_expand(\n\t)
for(FILE_PLUGIN,DEPEND_PLUGIN_FILES_COPY){
    QMAKE_PRE_LINK += copy /y \"$${FILE_PLUGIN}\" $${DESTDIR_PLUGIN_COPY}$$escape_expand(\n\t)
}



SOURCES += \
    addin/msaddndr_i.c \
    addin/addinmain.cpp \
    addin/addinimpl.cpp \
    addin/addinfactory.cpp \
    addin/ribbon_i.c \
    addin/ribboncallback_i.c \
    ui/maindialog.cpp \
    model/objectitem.cpp \
    model/objectmodel.cpp \
    model/objectsetting.cpp \
    model/projectsetting.cpp \
    model/objectitems.cpp \
    model/sanitizesetting.cpp \
    addin/comutil.cpp \
    addin/logfile.cpp \
    view/checkboxitemdelegate.cpp \
    view/boolcheckboxitemdelegate.cpp \
    model/tabledatasanitizesetting.cpp \
    model/tabledefsanitizesetting.cpp \
    model/objectproxymodel.cpp \
    git/gitmanager.cpp \
    ui/actionmanager.cpp \
    addin/addinutil.cpp

#    util/comptr.cpp \
#    util/getcomobject.cpp \
#    util/fileutil.cpp \
#    util/codecinfo.cpp \
#    util/progressnotifier.cpp \
#    util/datachangedhelper.cpp \
#    util/concurrentmaphelper.cpp \

HEADERS +=\
    addin/msaddndr.h \
    addin/addinmain.h \
    addin/addinimpl.h \
    addin/addinfactory.h \
    addin/ribbon.h \
    addin/ribboncallback.h \
    ui/maindialog.h \
    model/objectitem.h \
    model/objectmodel.h \
    accesscvs_global.h \
    model/objectsetting.h \
    model/projectsetting.h \
    model/objectitems.h \
    model/sanitizesetting.h \
    addin/comutil.h \
    addin/logfile.h \
    view/checkboxitemdelegate.h \
    view/boolcheckboxitemdelegate.h \
    model/tabledatasanitizesetting.h \
    model/tabledefsanitizesetting.h \
    model/objectproxymodel.h \
    git/gitmanager.h \
    ui/actionmanager.h \
    addin/addinutil.h

#    util/comptr.h \
#    util/getcomobject.h \
#    util/fileutil.h \
#    util/codecinfo.h \
#    util/progressnotifier.h \
#    util/datachangedhelper.h \
#    util/concurrentmaphelper.h \

DISTFILES += \
    addin/addin.def \
    addin/addin.rc \
    addin/ribboncallback.idl \
    addin/ribbon.xml

RESOURCES += \
    addin/addin.qrc \
    ui/resource.qrc

FORMS += \
    ui/maindialog.ui

