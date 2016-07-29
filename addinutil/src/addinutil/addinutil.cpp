#include "addinutil.h"

#include <QGuiApplication>
#include <QScreen>

#include "pch.hpp"


QSize AddInUtil::ribbonIconSize(AddInUtil::IconSizeType sizeType)
{
    qreal dpi = 96;
    if ( QGuiApplication::screens().count() )
    {
        QScreen *screen =  QGuiApplication::screens().at(0);
        if (screen)
            dpi = screen->logicalDotsPerInch();
    }

    int size = 0;
    switch (sizeType) {
        case Small:
        {
                 if (dpi >= 192) size = 32;
            else if (dpi >= 144) size = 24;
            else if (dpi >= 120) size = 20;
            else if (dpi >=  96) size = 16;
            else                 size = 16;
            break;
        }
        case Large:
        {
                 if (dpi >= 192) size = 64;
            else if (dpi >= 144) size = 48;
            else if (dpi >= 120) size = 40;
            else if (dpi >=  96) size = 32;
            else                 size = 32;
            break;
        }
    }
    return QSize(size,size);
}
