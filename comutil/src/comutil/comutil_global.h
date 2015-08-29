#ifndef COMUTIL_GLOBAL_H
#define COMUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMUTIL_LIBRARY)
#  define COMUTIL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMUTIL_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMUTIL_GLOBAL_H
