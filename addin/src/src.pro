QT += gui
QT += axcontainer
#QT += axserver
QT += winextras
QT += svg

#CONFIG += dll
CONFIG += staticlib
#CONFIG += qaxserver_no_postlink

TARGET   = addin
TEMPLATE = lib

#DEF_FILE = addin/addin.def
#RC_FILE  = addin/addin.rc

DEFINES += ADDIN_LIBRARY

#LIBS += -lQt5AxServer
#LIBS += -lqaxserver

SOURCES += \
    addin/addinfactory.cpp \
    addin/addinimpl.cpp \
    addin/addinmain.cpp \
    addin/addinutil.cpp \
    addin/comutil.cpp \
    addin/logfile.cpp \
    addin/interface/msaddndr_i.c \
    addin/interface/ribbon_i.c \
    addin/interface/ribboncallback_i.c

HEADERS += \
    addin/addin_global.h \
    addin/addinfactory.h \
    addin/addinimpl.h \
    addin/addinmain.h \
    addin/addinutil.h \
    addin/comutil.h \
    addin/logfile.h \
    addin/interface/msaddndr.h \
    addin/interface/ribbon.h \
    addin/interface/ribboncallback.h

#DISTFILES += \
#    addin/addin.rc \
#    addin/ribboncallback.idl

OTHER_FILES += \
    addin/distfile/addin.def \
    addin/distfile/addin.rc \
    addin/idl/msaddndr.idl \
    addin/idl/ribbon.idl \
    addin/idl/ribboncallback.idl



include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri) # dependencies for self

#LIBS += -loleacc

#installQtDlls(       $$OUT_PWD/$${BUILD_TYPE} )
#installModuleDlls(   $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
#installExternalDlls( $$OUT_PWD/$${BUILD_TYPE} $$OUT_PWD )
