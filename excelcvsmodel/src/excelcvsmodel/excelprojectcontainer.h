#ifndef EXCELPROJECTCONTAINER_H
#define EXCELPROJECTCONTAINER_H

#include "excelcvsmodel_global.h"
#include "cvsmodel/projectcontainer.h"

namespace Excel {
class Application;
}

class EXCELCVSMODEL_SHARED_EXPORT ExcelProjectContainer : public ProjectContainer
{
    Q_OBJECT
public:
    explicit ExcelProjectContainer(QObject *parent = 0);

    virtual void initialize(QAxObject* application);
    virtual bool isProjectOpened() const;

private:
    Excel::Application *m_application;
    int m_fileFormat;
};

#endif // EXCELPROJECTCONTAINER_H
