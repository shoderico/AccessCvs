QT -= gui
QT += axcontainer
CONFIG += dll

TARGET   = officelib
TEMPLATE = lib

DEFINES += OFFICELIB_LIBRARY

SOURCES += \
    access.cpp \
    adodb.cpp \
    dao.cpp \
    office.cpp \
    vbide.cpp

HEADERS +=\
    access.h \
    adodb.h \
    dao.h \
    office.h \
    vbide.h \
    officelib.h \
    officelib_global.h

include(../../common.pri)

TARGET_INCLUDE_DIR_WNT = $${PROJECT_INCLUDE_DIR_WNT}\\$${TARGET}
#TARGET_LIBRARY_DIR_WNT = $${PROJECT_LIBRARY_DIR_WNT}\\$${TARGET}


copy_headers.commands += $(CHK_DIR_EXISTS) $${TARGET_INCLUDE_DIR_WNT} $(MKDIR) $${TARGET_INCLUDE_DIR_WNT}$$escape_expand(\n\t)
for(h,HEADERS) {
    copy_headers.commands += $(COPY) $${PWD_WNT}\\$${h} $${TARGET_INCLUDE_DIR_WNT}$$escape_expand(\n\t)
}

#copy_library.commands += $(CHK_DIR_EXISTS) $${PROJECT_LIBRARY_DIR_WNT} $(MKDIR)   $${PROJECT_LIBRARY_DIR_WNT}$$escape_expand(\n\t)
#copy_library.commands += $(COPY) $${OUT_PWD_WNT}\\$${BUILD_TYPE}\\$${TARGET}.dll  $${PROJECT_LIBRARY_DIR_WNT}$$escape_expand(\n\t)
#copy_library.commands += $(COPY) $${OUT_PWD_WNT}\\$${BUILD_TYPE}\\lib$${TARGET}.a $${PROJECT_LIBRARY_DIR_WNT}$$escape_expand(\n\t)
QMAKE_POST_LINK += $(CHK_DIR_EXISTS) $${PROJECT_LIBRARY_DIR_WNT} $(MKDIR)   $${PROJECT_LIBRARY_DIR_WNT}$$escape_expand(\n\t)
QMAKE_POST_LINK += $(COPY) $${OUT_PWD_WNT}\\$${BUILD_TYPE}\\$${TARGET}.dll  $${PROJECT_LIBRARY_DIR_WNT}$$escape_expand(\n\t)
QMAKE_POST_LINK += $(COPY) $${OUT_PWD_WNT}\\$${BUILD_TYPE}\\lib$${TARGET}.a $${PROJECT_LIBRARY_DIR_WNT}$$escape_expand(\n\t)

QMAKE_EXTRA_TARGETS += copy_headers
POST_TARGETDEPS     += copy_headers
#QMAKE_POST_LINK     += copy_library
