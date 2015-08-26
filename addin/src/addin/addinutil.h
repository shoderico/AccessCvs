#ifndef ADDINUTIL_H
#define ADDINUTIL_H

#include "addin_global.h"

#include <QSize>

class ADDINSHARED_EXPORT AddInUtil
{
public:
    AddInUtil(){}

    enum IconSizeType
    {
        Large,
        Small,
    };

    static QSize ribbonIconSize(IconSizeType sizeType);
};

#endif // ADDINUTIL_H
