
# copy header files
#TARGET_INCLUDE_DIR = $${PROJECT_INCLUDE_DIR}/$${TARGET}
#
#copy_headers.commands += $(CHK_DIR_EXISTS) $$system_path($${TARGET_INCLUDE_DIR}) $(MKDIR) $$system_path($${TARGET_INCLUDE_DIR})$$escape_expand(\n\t)
#for(h,HEADERS) {
#    targetDir = $${TARGET_INCLUDE_DIR}/$${h}
#    copy_headers.commands += $(CHK_DIR_EXISTS) $$system_path( $$dirname(targetDir) ) $(MKDIR) $$system_path( $$dirname(targetDir) )$$escape_expand(\n\t)
#    copy_headers.commands += $(COPY) $$system_path($${_PRO_FILE_PWD_}/$${h}) $$system_path($${TARGET_INCLUDE_DIR}/$${h})$$escape_expand(\n\t)
#}
#QMAKE_EXTRA_TARGETS += copy_headers
#POST_TARGETDEPS     += copy_headers


# copy lib*.a fiies to lib,
QMAKE_POST_LINK += $(CHK_DIR_EXISTS) $$system_path($${PROJECT_LIBRARY_DIR}) $(MKDIR) $$system_path($${PROJECT_LIBRARY_DIR})$$escape_expand(\n\t)
QMAKE_POST_LINK += $(COPY) $$system_path($${OUT_PWD}/$${BUILD_TYPE}/lib$${TARGET}.$${LIB_EXT}) $$system_path($${PROJECT_LIBRARY_DIR})$$escape_expand(\n\t)

# copy  *.dll files to bin
contains (CONFIG, dll) {
QMAKE_POST_LINK += $(CHK_DIR_EXISTS) $$system_path($${PROJECT_BINARY_DIR}) $(MKDIR) $$system_path($${PROJECT_BINARY_DIR} )$$escape_expand(\n\t)
QMAKE_POST_LINK += $(COPY) $$system_path($${OUT_PWD}/$${BUILD_TYPE}/$${TARGET}.dll) $$system_path($${PROJECT_BINARY_DIR} )$$escape_expand(\n\t)
}
