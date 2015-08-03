#ifndef COMUTIL_H
#define COMUTIL_H

#include <ocidl.h>

#include <QString>
class ComUtil
{
public:
    ComUtil(){}

    static IPictureDisp *loadPicture(const QString &imagePath);
};

#endif // COMUTIL_H
