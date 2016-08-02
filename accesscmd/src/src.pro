#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T14:21:08
#
#-------------------------------------------------

QT += core
QT -= gui
#QT += axcontainer

TARGET = accesscmd
CONFIG += console
CONFIG -= app_bundle

LIBS += -lole32

TEMPLATE = app

SOURCES += main.cpp

include(../../common.pri) # directory declarations
include(../../subexe.pri) # copy *.exe to build/*/bin
include(../$${TARGET}_dep.pri) # dependencies for self

# copy dependent files for execution to OUT_PWD?
installQtDlls( $${OUT_PWD}/$${BUILD_TYPE} )
installModuleDlls( $${OUT_PWD}/$${BUILD_TYPE} )
installExternalDlls( $${OUT_PWD}/$${BUILD_TYPE} )
