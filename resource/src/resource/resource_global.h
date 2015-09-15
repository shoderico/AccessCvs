#ifndef RESOURCE_GLOBAL_H
#define RESOURCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RESOURCE_LIBRARY)
#  define RESOURCE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define RESOURCE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RESOURCE_GLOBAL_H
