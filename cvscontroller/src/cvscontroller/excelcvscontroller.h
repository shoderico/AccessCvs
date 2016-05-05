#ifndef EXCELCVSCONTROLLER_H
#define EXCELCVSCONTROLLER_H

#include "cvscontroller.h"

class CVSCONTROLLER_SHARED_EXPORT ExcelCvsController : public CvsController
{
    Q_OBJECT
public:
    explicit ExcelCvsController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // EXCELCVSCONTROLLER_H
