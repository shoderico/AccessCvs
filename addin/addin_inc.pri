# inc.pri : static module
includeStaticLib( addin )

defineTest(includeAddIn) {
    unset(MY_TARGET)
    MY_TARGET = $$1
    DEF_FILE = $${PROJECT_ROOT}/$${MY_TARGET}/src/$${MY_TARGET}/distfile/addin.def
    RC_FILE  = $${PROJECT_ROOT}/$${MY_TARGET}/src/$${MY_TARGET}/distfile/addin.rc
    export(DEF_FILE)
    export(RC_FILE)
}
includeAddIn( addin )
