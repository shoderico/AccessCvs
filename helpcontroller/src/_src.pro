#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T09:39:09
#
#-------------------------------------------------

QT       -= gui

TARGET = src
TEMPLATE = lib

DEFINES += SRC_LIBRARY

SOURCES += src.cpp

HEADERS += src.h\
        src_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
