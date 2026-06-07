#ifndef ACCESSCVSADDINCONTROLLER_H
#define ACCESSCVSADDINCONTROLLER_H

#include "accesscvsaddincontroller_global.h"
#include "cvsaddincontroller/cvsaddincontroller.h"

class ACCESSCVSADDINCONTROLLER_SHARED_EXPORT AccessCvsAddinController : public CvsAddinController
{
    Q_OBJECT
public:
    explicit AccessCvsAddinController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // ACCESSCVSADDINCONTROLLER_H
