#ifndef EXCELCVSADDINCONTROLLER_H
#define EXCELCVSADDINCONTROLLER_H

#include "excelcvsaddincontroller_global.h"
#include "cvscontroller/cvscontroller.h"

class EXCALCVSADDINCONTROLLER_SHARED_EXPORT ExcelCvsAddinController : public CvsController
{
    Q_OBJECT
public:
    explicit ExcelCvsAddinController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // EXCELCVSADDINCONTROLLER_H
