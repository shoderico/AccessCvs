# inc.pri : final module
includeFinalModule( excelcvs, dll )

defineTest(includeExcelCvs) {
    unset(MY_TARGET)
    MY_TARGET = $$1

    # copy this module's dll to bin directory again.
    # workaround for :
    #    qaxserver's post_link runs AFTER other QMAKE_POST_LINKs in pro.
    #    In other words, even if we set QMAKE_POST_LINK to copy dll in .pro, correct dll will not copied.
    #    so we will copy correct dll by includer of this module.
    QMAKE_PRE_LINK += $(COPY) \"$$system_path($${OUT_PWD}/../../$${MY_TARGET}/src/$${BUILD_TYPE}/$${MY_TARGET}.dll)\" \"$$system_path($${PROJECT_BINARY_DIR})\"$$escape_expand(\n\t)
    export(QMAKE_PRE_LINK)
}
includeExcelCvs( excelcvs )
