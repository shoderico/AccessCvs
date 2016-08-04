#ifndef ADDIN_GLOBAL_H
#define ADDIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ADDIN_LIBRARY)
#  define ADDIN_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ADDIN_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ADDIN_GLOBAL_H
