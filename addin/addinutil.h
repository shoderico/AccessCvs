#ifndef ADDINUTIL_H
#define ADDINUTIL_H

#include <QSize>

class AddInUtil
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
