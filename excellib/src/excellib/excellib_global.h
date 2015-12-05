#ifndef EXCELLIB_GLOBAL_H
#define EXCELLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EXCELLIB_LIBRARY)
#  define EXCELLIB_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXCELLIB_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EXCELLIB_GLOBAL_H

