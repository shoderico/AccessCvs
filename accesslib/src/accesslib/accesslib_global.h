#ifndef ACCESSLIB_GLOBAL_H
#define ACCESSLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACCESSLIB_LIBRARY)
#  define ACCESSLIB_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACCESSLIB_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACCESSLIB_GLOBAL_H

