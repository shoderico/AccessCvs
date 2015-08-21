PROJECT_ROOT = $$PWD
LIBRARY_ROOT = $$PWD/..
BUILD_ROOT = $${PROJECT_ROOT}/build
Release:BUILD_TYPE = release
Debug:  BUILD_TYPE = debug
#export(BUILD_TYPE)
#BUILD_TYPE = $${BUILD_TYPE}
OBJECTS_DIR = $${BUILD_TYPE}/.obj
MOC_DIR     = $${BUILD_TYPE}/.moc
RCC_DIR     = $${BUILD_TYPE}/.rcc
UI_DIR      = $${BUILD_TYPE}/.ui

PROJECT_INCLUDE_DIR     = $${BUILD_ROOT}/include
PROJECT_INCLUDE_DIR_WNT = $${PROJECT_INCLUDE_DIR}
PROJECT_INCLUDE_DIR_WNT ~= s,/,\\,g

PROJECT_LIBRARY_DIR     = $${BUILD_ROOT}/$${BUILD_TYPE}/lib
PROJECT_LIBRARY_DIR_WNT = $${PROJECT_LIBRARY_DIR}
PROJECT_LIBRARY_DIR_WNT ~= s,/,\\,g

OUT_PWD_WNT = $${OUT_PWD}
OUT_PWD_WNT ~= s,/,\\,g

PWD_WNT = $${_PRO_FILE_PWD_}
PWD_WNT ~= s,/,\\,g

# message($${PWD}) # return this file's dir
# message($${OUT_PWD}) # return caller's dir
# message($$_PRO_FILE_PWD_) # return caller's pro dir
# message($${TARGET}) # return caller's target
# for (h,HEADERS) {
#    message($${h})
# } # return caller's headers
