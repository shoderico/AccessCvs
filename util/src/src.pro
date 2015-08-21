QT -= gui
QT += axcontainer
CONFIG += dll

TARGET = util
TEMPLATE = lib

LIBS += -loleacc

HEADERS += \
    util_global.h \
    codecinfo.h \
    comptr.h \
    concurrentmaphelper.h \
    datachangedhelper.h \
    fileutil.h \
    getcomobject.h \
    progressnotifier.h

DEFINES += UTIL_LIBRARY


SOURCES += \
    codecinfo.cpp \
    comptr.cpp \
    concurrentmaphelper.cpp \
    datachangedhelper.cpp \
    fileutil.cpp \
    getcomobject.cpp \
    progressnotifier.cpp

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
