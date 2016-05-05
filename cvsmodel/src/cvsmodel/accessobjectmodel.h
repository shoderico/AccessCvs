#ifndef ACCESSOBJECTMODEL_H
#define ACCESSOBJECTMODEL_H

#include "objectmodel.h"

namespace Access {
class Application;
}

class CVSMODEL_SHARED_EXPORT AccessObjectModel : public ObjectModel
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
