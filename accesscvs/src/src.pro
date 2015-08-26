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

#DEF_FILE = addin/addin.def
#RC_FILE  = addin/addin.rc

DEFINES += ACCESSCVS_LIBRARY
DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += \
    ui/maindialog.cpp \
    view/checkboxitemdelegate.cpp \
    view/boolcheckboxitemdelegate.cpp \
    git/gitmanager.cpp \
    managers/actionmanager.cpp \
    managers/accessutilmanager.cpp \
    managers/windowwidgetmanager.cpp \
    view/uiblocker.cpp \
    cvsaddin/cvsaddinfactory.cpp \
    cvsaddin/cvsaddinimpl.cpp

#    addin/msaddndr_i.c \
#    addin/addinmain.cpp \
#    addin/addinimpl.cpp \
#    addin/addinfactory.cpp \
#    addin/ribbon_i.c \
#    addin/ribboncallback_i.c \
#    addin/comutil.cpp \
#    addin/logfile.cpp \
#    addin/addinutil.cpp \

#    util/comptr.cpp \
#    util/getcomobject.cpp \
#    util/fileutil.cpp \
#    util/codecinfo.cpp \
#    util/progressnotifier.cpp \
#    util/datachangedhelper.cpp \
#    util/concurrentmaphelper.cpp \

HEADERS +=\
    ui/maindialog.h \
    accesscvs_global.h \
    view/checkboxitemdelegate.h \
    view/boolcheckboxitemdelegate.h \
    git/gitmanager.h \
    managers/actionmanager.h \
    managers/accessutilmanager.h \
    managers/windowwidgetmanager.h \
    view/uiblocker.h \
    cvsaddin/cvsaddinfactory.h \
    cvsaddin/cvsaddinimpl.h

#    addin/addinutil.h \
#    addin/msaddndr.h \
#    addin/addinmain.h \
#    addin/addinimpl.h \
#    addin/addinfactory.h \
#    addin/ribbon.h \
#    addin/ribboncallback.h \
#    addin/comutil.h \
#    addin/logfile.h \

#    util/comptr.h \
#    util/getcomobject.h \
#    util/fileutil.h \
#    util/codecinfo.h \
#    util/progressnotifier.h \
#    util/datachangedhelper.h \
#    util/concurrentmaphelper.h \

DISTFILES += \
    cvsaddin/ribbon.xml
#    addin/addin.def \
#    addin/addin.rc \
#    addin/ribboncallback.idl \



RESOURCES += \
    cvsaddin/cvsaddin.qrc
#    ui/resource.qrc

FORMS += \
    ui/maindialog.ui



include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self


# directory where *.dll output to
#DESTDIR = $${BUILD_ROOT}/$${BUILD_TYPE}/bin

installQtDlls( $$OUT_PWD/$${BUILD_TYPE} )
installModuleDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
