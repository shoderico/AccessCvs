#ifndef ACCESSPROJECTCONTAINER_H
#define ACCESSPROJECTCONTAINER_H

#include "projectcontainer.h"

namespace Access {
class Application;
}

class CVSMODEL_SHARED_EXPORT AccessProjectContainer : public ProjectContainer
{
    Q_OBJECT
public:
    explicit AccessProjectContainer(QObject *parent = 0);

    virtual void initialize(QAxObject* application);
    virtual bool isProjectOpened() const;

    bool isMDB() const;
    bool isADP() const;

private:
    Access::Application *m_application;
    int m_projectType;
};

#endif // ACCESSPROJECTCONTAINER_H
