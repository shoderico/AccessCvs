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

# output directories
BUILDDIR = $$_PRO_FILE_PWD_/build
Release:DESTDIR = $$BUILDDIR/release/bin
Debug:DESTDIR = $$BUILDDIR/debug/bin
OBJECTS_DIR = $$DESTDIR/../.obj
MOC_DIR = $$DESTDIR/../.moc
RCC_DIR = $$DESTDIR/../.rcc
UI_DIR = $$DESTDIR/../.ui


# LibQGit2
LIBQGIT_DIR = $$PWD/../LibQGit2/install
LIBS        += -L$$LIBQGIT_DIR/lib/ -lqgit2.dll
LIBS        += -L$$LIBQGIT_DIR/lib/ -lgit2.dll
INCLUDEPATH +=   $$LIBQGIT_DIR/include
DEPENDPATH  +=   $$LIBQGIT_DIR/include


# QtSolutions
QTSOLUTIONS_DIR = $$PWD/../QtSolutions/qtwinmigrate
include($$QTSOLUTIONS_DIR/src/qtwinmigrate.pri)

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
    $$[QT_INSTALL_BINS]/Qt5Widgets.dll \
    $$[QT_INSTALL_BINS]/Qt5WinExtras.dll \
    $$QTSOLUTIONS_DIR/lib/QtSolutions_MFCMigrationFramework-head.dll \
    $$OPENSSL_DIR/libeay32.dll \
    $$OPENSSL_DIR/ssleay32.dll \
    $$LIBQGIT_DIR/bin/libgit2.dll \
    $$LIBQGIT_DIR/bin/libqgit2.dll \

DEPEND_DLL_FILES_COPY = $${DEPEND_DLL_FILES}
DEPEND_DLL_FILES_COPY ~= s,/,\\,g
DESTDIR_DLL_COPY = $${DESTDIR}
DESTDIR_DLL_COPY ~= s,/,\\,g
for(FILE,DEPEND_DLL_FILES_COPY){
    QMAKE_PRE_LINK += copy /y \"$${FILE}\" $${DESTDIR_DLL_COPY}$$escape_expand(\n\t)
}

# Depend : platforms/*.dll
DEPEND_PLUGIN_FILES += \
    $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll

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
    officelib/access.cpp \
    officelib/adodb.cpp \
    officelib/dao.cpp \
    officelib/office.cpp \
    officelib/vbide.cpp \
    model/objectitem.cpp \
    model/objectmodel.cpp \
    util/comptr.cpp \
    util/getcomobject.cpp \
    model/objectsetting.cpp \
    model/projectsetting.cpp \
    model/objectitems.cpp \
    util/fileutil.cpp \
    util/codecinfo.cpp \
    model/sanitizesetting.cpp \
    addin/comutil.cpp \
    addin/logfile.cpp \
    util/progressnotifier.cpp \
    util/datachangedhelper.cpp \
    view/checkboxitemdelegate.cpp \
    view/boolcheckboxitemdelegate.cpp \
    model/tabledatasanitizesetting.cpp \
    model/tabledefsanitizesetting.cpp \
    model/objectproxymodel.cpp \
    git/gitmanager.cpp \
    ui/actionmanager.cpp \
    util/concurrentmaphelper.cpp

HEADERS +=\
    addin/msaddndr.h \
    addin/addinmain.h \
    addin/addinimpl.h \
    addin/addinfactory.h \
    addin/ribbon.h \
    addin/ribboncallback.h \
    ui/maindialog.h \
    officelib/access.h \
    officelib/adodb.h \
    officelib/dao.h \
    officelib/office.h \
    officelib/vbide.h \
    model/objectitem.h \
    model/objectmodel.h \
    util/comptr.h \
    officelib/officelib.h \
    util/getcomobject.h \
    accesscvs_global.h \
    model/objectsetting.h \
    model/projectsetting.h \
    model/objectitems.h \
    util/fileutil.h \
    util/codecinfo.h \
    model/sanitizesetting.h \
    addin/comutil.h \
    addin/logfile.h \
    util/progressnotifier.h \
    util/datachangedhelper.h \
    view/checkboxitemdelegate.h \
    view/boolcheckboxitemdelegate.h \
    model/tabledatasanitizesetting.h \
    model/tabledefsanitizesetting.h \
    model/objectproxymodel.h \
    git/gitmanager.h \
    ui/actionmanager.h \
    util/concurrentmaphelper.h

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

