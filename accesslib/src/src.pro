QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = accesslib
TEMPLATE = lib

DEFINES += ACCESSLIB_LIBRARY

SOURCES += \
    accesslib/access.cpp \
    accesslib/adodb.cpp \
    accesslib/dao.cpp \
    accesslib/access_impl.cpp \
    accesslib/adodb_impl.cpp \
    accesslib/dao_impl.cpp

HEADERS +=\
    accesslib/access.h \
    accesslib/adodb.h \
    accesslib/dao.h \
    accesslib/accesslib.h \
    accesslib/accesslib_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
