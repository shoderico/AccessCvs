#ifndef CVSMODEL_GLOBAL_H
#define CVSMODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CVSMODEL_LIBRARY)
#  define CVSMODEL_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define CVSMODEL_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CVSMODEL_GLOBAL_H
