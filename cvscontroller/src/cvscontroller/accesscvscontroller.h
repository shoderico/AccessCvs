#ifndef ACCESSCVSCONTROLLER_H
#define ACCESSCVSCONTROLLER_H

#include "cvscontroller.h"

class CVSCONTROLLER_SHARED_EXPORT AccessCvsController : public CvsController
{
    Q_OBJECT
public:
    explicit AccessCvsController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // ACCESSCVSCONTROLLER_H
