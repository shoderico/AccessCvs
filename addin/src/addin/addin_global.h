#ifndef ADDIN_GLOBAL_H
#define ADDIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ADDIN_LIBRARY)
#  define ADDINSHARED_EXPORT
#else
#  define ADDINSHARED_EXPORT
#endif

#endif // ADDIN_GLOBAL_H
