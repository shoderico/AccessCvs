QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = excellib
TEMPLATE = lib

DEFINES += EXCELLIB_LIBRARY

SOURCES += \
    excellib/excel.cpp \
    excellib/excel_impl.cpp

HEADERS +=\
    excellib/excel.h \
    excellib/excellib.h \
    excellib/excellib_global.h

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
