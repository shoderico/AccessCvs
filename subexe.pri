# copy *.exe files to bin
QMAKE_POST_LINK += $(CHK_DIR_EXISTS) $${PROJECT_BINARY_DIR_WNT} $(MKDIR)   $${PROJECT_BINARY_DIR_WNT}$$escape_expand(\n\t)
QMAKE_POST_LINK += $(COPY) $${OUT_PWD_WNT}\\$${BUILD_TYPE}\\$${TARGET}.exe $${PROJECT_BINARY_DIR_WNT}$$escape_expand(\n\t)
