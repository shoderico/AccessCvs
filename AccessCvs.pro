#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T22:28:05
#
#-------------------------------------------------

QT += gui
QT += axserver
QT += axcontainer
QT += winextras
CONFIG += dll
#CONFIG += qaxserver_no_postlink
CONFIG += c++11

LIBS += -loleacc

TARGET   = AccessCvs
TEMPLATE = lib
#VERSION  = 1.0

DEF_FILE = addin/addin.def
RC_FILE  = addin/addin.rc

include(../QtSolutions/qtwinmigrate/src/qtwinmigrate.pri)

DEFINES += ACCESSCVS_LIBRARY
DEFINES += QT_MESSAGELOGCONTEXT

# dynamic link
LIBS        += -L$$PWD/../LibQGit2/install/lib/ -lqgit2.dll
LIBS        += -L$$PWD/../LibQGit2/install/lib/ -lgit2.dll
INCLUDEPATH +=   $$PWD/../LibQGit2/install/include
DEPENDPATH  +=   $$PWD/../LibQGit2/install/include


SOURCES += \
    addin/msaddndr_i.c \
    addin/addinmain.cpp \
    addin/addinimpl.cpp \
    addin/addinfactory.cpp \
    addin/ribbon_i.c \
    addin/ribboncallback_i.c \
    ui/maindialog.cpp \
    officelib/access.cpp \
    officelib/adodb.cpp \
    officelib/dao.cpp \
    officelib/office.cpp \
    officelib/vbide.cpp \
    model/objectitem.cpp \
    model/objectmodel.cpp \
    util/comptr.cpp \
    util/getcomobject.cpp \
    model/objectsetting.cpp \
    model/projectsetting.cpp \
    model/objectitems.cpp \
    util/fileutil.cpp \
    util/codecinfo.cpp \
    model/sanitizesetting.cpp \
    addin/comutil.cpp \
    addin/logfile.cpp \
    util/progressnotifier.cpp \
    util/datachangedhelper.cpp \
    view/checkboxitemdelegate.cpp \
    view/boolcheckboxitemdelegate.cpp \
    model/tabledatasanitizesetting.cpp \
    model/tabledefsanitizesetting.cpp \
    model/objectproxymodel.cpp \
    git/gitmanager.cpp

HEADERS +=\
    addin/msaddndr.h \
    addin/addinmain.h \
    addin/addinimpl.h \
    addin/addinfactory.h \
    addin/ribbon.h \
    addin/ribboncallback.h \
    ui/maindialog.h \
    officelib/access.h \
    officelib/adodb.h \
    officelib/dao.h \
    officelib/office.h \
    officelib/vbide.h \
    model/objectitem.h \
    model/objectmodel.h \
    util/comptr.h \
    officelib/officelib.h \
    util/getcomobject.h \
    accesscvs_global.h \
    model/objectsetting.h \
    model/projectsetting.h \
    model/objectitems.h \
    util/fileutil.h \
    util/codecinfo.h \
    model/sanitizesetting.h \
    addin/comutil.h \
    addin/logfile.h \
    util/progressnotifier.h \
    util/datachangedhelper.h \
    view/checkboxitemdelegate.h \
    view/boolcheckboxitemdelegate.h \
    model/tabledatasanitizesetting.h \
    model/tabledefsanitizesetting.h \
    model/objectproxymodel.h \
    git/gitmanager.h

DISTFILES += \
    addin/addin.def \
    addin/addin.rc \
    addin/ribboncallback.idl \
    addin/ribbon.xml

RESOURCES += \
    addin/addin.qrc \
    ui/resource.qrc

FORMS += \
    ui/maindialog.ui

