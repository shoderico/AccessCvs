#ifndef OFFICEUTIL_H
#define OFFICEUTIL_H

#include "util_global.h"

#include <QString>

class UTILSHARED_EXPORT OfficeUtil
{
public:
    OfficeUtil(){}

    static QString getExePath(const QString &progId);
};

#endif // OFFICEUTIL_H
