#ifndef ADDINUTIL_H
#define ADDINUTIL_H

#include "addinutil_global.h"

#include <QSize>

class ADDINUTIL_SHARED_EXPORT AddInUtil
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
