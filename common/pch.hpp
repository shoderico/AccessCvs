#ifndef PCH_H
#define PCH_H

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif // PCH_H
