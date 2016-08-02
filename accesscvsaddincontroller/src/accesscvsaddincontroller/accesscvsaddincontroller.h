#ifndef ACCESSCVSCONTROLLER_H
#define ACCESSCVSCONTROLLER_H

#include "accesscvsaddincontroller_global.h"
#include "cvscontroller/cvscontroller.h"

class ACCESSCVSADDINCONTROLLER_SHARED_EXPORT AccessCvsController : public CvsController
{
    Q_OBJECT
public:
    explicit AccessCvsController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // ACCESSCVSCONTROLLER_H
