#ifndef REFERENCEPROCESSOR_H
#define REFERENCEPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "vbeprocessor.h"

class ProjectContainer;

class QAxObject;

class ReferenceProcessor : public VBEProcessor
{
public:
    explicit ReferenceProcessor(ProjectContainer *parent);
//    virtual bool        isTargetObject(QAxObject *object) const;
//    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
//    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
//    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

//    virtual bool        prepareItemCollection();
//    virtual int         itemCount();
//    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
private:
//    QString m_objectName;
    // ComPtr<Access::XXX> m_objects; // no need
};


#endif // REFERENCEPROCESSOR_H
