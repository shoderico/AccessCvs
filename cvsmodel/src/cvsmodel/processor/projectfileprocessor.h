#ifndef PROJECTFILEPROCESSOR_H
#define PROJECTFILEPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "projectlevelobjectprocessor.h"

class ProjectSetting;

class QAxObject;

class ProjectFileProcessor : public ProjectLevelObjectProcessor
{
public:
    explicit ProjectFileProcessor(ProjectSetting *parent);
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

private:
    struct ProjectFileProperty
    {
        QString Name;
        int Type;
        QVariant Value;
        ProjectFileProperty(const QString name, const int type, const QVariant &value)
            : Name(name), Type(type), Value(value)
        {
        }
    };
    void loadProperties( QMap<QString, ProjectFileProperty*> &propMap );
};


#endif // PROJECTFILEPROCESSOR_H
