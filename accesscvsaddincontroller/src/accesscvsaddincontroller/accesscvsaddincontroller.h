#ifndef ACCESSCVSADDINCONTROLLER_H
#define ACCESSCVSADDINCONTROLLER_H

#include "accesscvsaddincontroller_global.h"
#include "cvscontroller/cvscontroller.h"

class ACCESSCVSADDINCONTROLLER_SHARED_EXPORT AccessCvsAddinController : public CvsController
{
    Q_OBJECT
public:
    explicit AccessCvsAddinController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // ACCESSCVSADDINCONTROLLER_H
