#ifndef ACCESSOBJECTMODEL_H
#define ACCESSOBJECTMODEL_H

#include "accesscvsmodel_global.h"
#include "cvsmodel/objectmodel.h"

namespace Access {
class Application;
}

class ACCESSCVSMODEL_SHARED_EXPORT AccessObjectModel : public ObjectModel
{
    Q_OBJECT
public:
    explicit AccessObjectModel(QObject *parent = 0);

    virtual void setApplication(QAxObject *application);

    virtual bool executeImport();
    virtual bool checkProjectState();

signals:

public slots:

private:
    Access::Application *m_application;
};

#endif // ACCESSOBJECTMODEL_H
