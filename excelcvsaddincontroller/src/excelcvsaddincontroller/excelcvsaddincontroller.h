#ifndef EXCELCVSADDINCONTROLLER_H
#define EXCELCVSADDINCONTROLLER_H

#include "excelcvsaddincontroller_global.h"
#include "cvsaddincontroller/cvsaddincontroller.h"

class EXCALCVSADDINCONTROLLER_SHARED_EXPORT ExcelCvsAddinController : public CvsAddinController
{
    Q_OBJECT
public:
    explicit ExcelCvsAddinController(QObject *parent = 0);

protected:
    virtual void init();
};

#endif // EXCELCVSADDINCONTROLLER_H
