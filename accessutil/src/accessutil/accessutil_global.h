#ifndef ACCESSUTIL_GLOBAL_H
#define ACCESSUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACCESSUTIL_LIBRARY)
#  define ACCESSUTIL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACCESSUTIL_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACCESSUTIL_GLOBAL_H
