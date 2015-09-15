#ifndef COMUTIL_H
#define COMUTIL_H

#include "comutil_global.h"

//#include <ocidl.h>
struct IPictureDisp;

#include <QString>
#include <QPixmap>

class COMUTIL_SHARED_EXPORT ComUtil
{
public:
    ComUtil(){}

    static IPictureDisp *loadPicture(const QString &imagePath);
    static IPictureDisp *loadPicture(const QPixmap &pixmap);
    static IPictureDisp *loadPictureFromSvg(const QString &imagePath, const QSize &size);

    static QPixmap renderSvg(const QString &imagePath, const QSize &size);
};

#endif // COMUTIL_H
