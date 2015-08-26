#ifndef COMUTIL_H
#define COMUTIL_H

#include "addin_global.h"

#include <ocidl.h>

#include <QString>
#include <QPixmap>

class ADDINSHARED_EXPORT ComUtil
{
public:
    ComUtil(){}

    static IPictureDisp *loadPicture(const QString &imagePath);
    static IPictureDisp *loadPicture(const QPixmap &pixmap);
    static IPictureDisp *loadPictureFromSvg(const QString &imagePath, const QSize &size);
};

#endif // COMUTIL_H
