#ifndef QUERYASOBJECTPROCESSOR_H
#define QUERYASOBJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "queryassqlprocessor.h"

class ProjectSetting;

class QAxObject;

class QueryAsObjectProcessor : public QueryAsSqlProcessor
{
public:
    explicit QueryAsObjectProcessor(ProjectSetting *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
};


#endif // QUERYASOBJECTPROCESSOR_H
