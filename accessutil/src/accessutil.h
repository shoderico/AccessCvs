#ifndef ACCESSUTIL_H
#define ACCESSUTIL_H

#include "accessutil_global.h"

class ACCESSUTILSHARED_EXPORT AccessUtil
{

public:
    AccessUtil(){}

    bool decompile(const QString &fileName);
    bool compactRepair(const QString &fileName);
};

#endif // ACCESSUTIL_H
