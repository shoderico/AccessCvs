# inc.pri : external module
# OpenSSL
OPENSSL_BIN_DIR = C:/OpenSSL-Win32
OPENSSL_FILES   = $${OPENSSL_BIN_DIR}/libeay32.dll \
                  $${OPENSSL_BIN_DIR}/ssleay32.dll

# register this module dll files to dependent external files
DEP_DLLS_EXTERNAL.files += $${OPENSSL_FILES}

# load self dependencies
include(openssl_dep.pri)
