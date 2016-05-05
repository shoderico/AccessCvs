#ifndef ODBCTABLEPROCESSOR_H
#define ODBCTABLEPROCESSOR_H

#include "tableobjectprocessor.h"

class OdbcTableProcessor : public TableObjectProcessor
{
public:
    explicit OdbcTableProcessor(ProjectContainer *parent);

    virtual bool        isTargetObject(QAxObject *object) const;

    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);

    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

};

#endif // ODBCTABLEPROCESSOR_H
