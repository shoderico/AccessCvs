#ifndef PROJECTLEVELOBJECTPROCESSOR_H
#define PROJECTLEVELOBJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "objectprocessor.h"

class ProjectSetting;
class ObjectItem;

class QAxObject;

class ProjectLevelObjectProcessor : public ObjectProcessor
{
public:
    explicit ProjectLevelObjectProcessor(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
//    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
//    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    QString m_objectName;
    // ComPtr<Access::XXX> m_objects; // no need
};


#endif // PROJECTLEVELOBJECTPROCESSOR_H
