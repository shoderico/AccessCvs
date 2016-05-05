#ifndef EXCELOBJECTMODEL_H
#define EXCELOBJECTMODEL_H

#include "objectmodel.h"

namespace Excel {
class Application;
}

class CVSMODEL_SHARED_EXPORT ExcelObjectModel : public ObjectModel
{
    Q_OBJECT
public:
    explicit ExcelObjectModel(QObject *parent = 0);

    virtual void setApplication(QAxObject *application);

    virtual bool checkProjectState();

private:
    Excel::Application *m_application;
};

#endif // EXCELOBJECTMODEL_H
