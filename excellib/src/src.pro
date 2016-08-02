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

include(../../qmake/common.pri)
include(../../qmake/submodule.pri)
include(../$${TARGET}_dep.pri)

win32-msvc* {
    QMAKE_CXXFLAGS += -bigobj
}
