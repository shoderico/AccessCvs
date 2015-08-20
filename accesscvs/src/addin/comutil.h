#ifndef COMUTIL_H
#define COMUTIL_H

#include <ocidl.h>

#include <QString>
#include <QPixmap>

class ComUtil
{
public:
    ComUtil(){}

    static IPictureDisp *loadPicture(const QString &imagePath);
    static IPictureDisp *loadPicture(const QPixmap &pixmap);
    static IPictureDisp *loadPictureFromSvg(const QString &imagePath, const QSize &size);
};

#endif // COMUTIL_H
