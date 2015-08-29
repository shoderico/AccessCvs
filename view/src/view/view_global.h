#ifndef VIEW_GLOBAL_H
#define VIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIEW_LIBRARY)
#  define VIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VIEWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VIEW_GLOBAL_H
