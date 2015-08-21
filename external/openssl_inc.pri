# OpenSSL
OPENSSL_DIR   = C:/OpenSSL-Win32
OPENSSL_FILES = $${OPENSSL_DIR}/libeay32.dll \
                $${OPENSSL_DIR}/ssleay32.dll

# register dlls to module/external
DEP_DLLS_EXTERNAL.files += $${OPENSSL_FILES}

# load self dependencies
include(openssl_dep.pri)
