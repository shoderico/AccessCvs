#ifndef VIEW_GLOBAL_H
#define VIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIEW_LIBRARY)
#  define VIEW_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define VIEW_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VIEW_GLOBAL_H
