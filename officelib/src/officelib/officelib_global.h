#ifndef OFFICELIB_GLOBAL_H
#define OFFICELIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OFFICELIB_LIBRARY)
#  define OFFICELIB_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define OFFICELIB_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OFFICELIB_GLOBAL_H

