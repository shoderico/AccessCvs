#ifndef SRC_GLOBAL_H
#define SRC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HELPCONTROLLER_LIBRARY)
#  define HELPCONTROLLER_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define HELPCONTROLLER_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SRC_GLOBAL_H
