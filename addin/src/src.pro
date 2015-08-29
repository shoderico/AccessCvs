QT -= gui
QT += axcontainer

#CONFIG += dll
CONFIG += staticlib

TARGET   = addin
TEMPLATE = lib

DEFINES += ADDIN_LIBRARY

SOURCES += \
    addin/addinfactory.cpp \
    addin/addinimpl.cpp \
    addin/addinmain.cpp \
    addin/interface/msaddndr_i.c \
    addin/interface/ribbon_i.c \
    addin/interface/ribboncallback_i.c

HEADERS += \
    addin/addin_global.h \
    addin/addinfactory.h \
    addin/addinimpl.h \
    addin/addinmain.h \
    addin/interface/msaddndr.h \
    addin/interface/ribbon.h \
    addin/interface/ribboncallback.h \
    addin/addincontroller.h

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
