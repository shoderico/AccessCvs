#ifndef ADDINUTIL_GLOBAL_H
#define ADDINUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ADDINUTIL_LIBRARY)
#  define ADDINUTIL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ADDINUTIL_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ADDINUTIL_GLOBAL_H
