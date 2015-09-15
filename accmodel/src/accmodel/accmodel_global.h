#ifndef ACCMODEL_GLOBAL_H
#define ACCMODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ACCMODEL_LIBRARY)
#  define ACCMODEL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACCMODEL_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACCMODEL_GLOBAL_H
