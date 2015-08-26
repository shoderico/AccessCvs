#ifndef CVSMODEL_GLOBAL_H
#define CVSMODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CVSMODEL_LIBRARY)
#  define CVSMODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CVSMODELSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CVSMODEL_GLOBAL_H
