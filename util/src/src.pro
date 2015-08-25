QT -= gui
QT += axcontainer
CONFIG += dll

TARGET = util
TEMPLATE = lib

LIBS += -loleacc

HEADERS += \
    util/util_global.h \
    util/codecinfo.h \
    util/comptr.h \
    util/concurrentmaphelper.h \
    util/datachangedhelper.h \
    util/fileutil.h \
    util/getcomobject.h \
    util/progressnotifier.h \
    util/officeutil.h \
    util/threadedinvoker.h

DEFINES += UTIL_LIBRARY


SOURCES += \
    util/codecinfo.cpp \
    util/comptr.cpp \
    util/concurrentmaphelper.cpp \
    util/datachangedhelper.cpp \
    util/fileutil.cpp \
    util/getcomobject.cpp \
    util/progressnotifier.cpp \
    util/officeutil.cpp \
    util/threadedinvoker.cpp

include(../../common.pri)
include(../../submodule.pri)
include(../$${TARGET}_dep.pri)
