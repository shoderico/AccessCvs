#ifndef QUERYASOBJECTPROCESSOR_H
#define QUERYASOBJECTPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "queryassqlprocessor.h"

class ProjectContainer;

class QAxObject;

class QueryAsObjectProcessor : public QueryAsSqlProcessor
{
public:
    explicit QueryAsObjectProcessor(ProjectContainer *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
};


#endif // QUERYASOBJECTPROCESSOR_H
