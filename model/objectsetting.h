#ifndef OBJECTSETTING_H
#define OBJECTSETTING_H

#include <QObject>

#include "objectitem.h"

class ProjectSetting;

class QAxObject;

class ObjectSetting : public QObject
{
    Q_OBJECT
public:
    explicit ObjectSetting(ProjectSetting *parent);


    virtual bool isTargetObject(QAxObject *object) const;

    Model::ObjectType objectType() const { return m_objectType; }
    int accessObjectType() const { return m_accessObjectType; }

    QString objectPathName() const { return m_objectPathName; }
    QString containerName() const { return m_containerName; }

    QString tempFileExtension()   const { return m_tempFileExtension; }
    QString designFileExtension() const { return m_designFileExtension; }
    QString moduleFileExtension() const { return m_moduleFileExtension; }
    QString existCheckExtension() const { return m_existCheckExtension; }

public:
    // file path
    QString tempFileInTempPath  (const QString &objectName);
    QString designFileInTempPath(const QString &objectName);
    QString moduleFileInTempPath(const QString &objectName);

    // object path
    QString sourceObjectPath() const;
    QString tempObjectPath() const;

    // object path
    void mkdirTempObjectPath();
    void mkdirSourceObjectPath();

    void saveToFile(const QString &contents, const QString &filePath);

protected:
    Model::ObjectType m_objectType;
    int m_accessObjectType;
    QString m_objectPathName;
    QString m_containerName;

    QString m_tempFileExtension;
    QString m_designFileExtension;
    QString m_moduleFileExtension;
    QString m_existCheckExtension;

    ProjectSetting *m_projectSetting;
};



class TableDefSetting : public ObjectSetting
{
public:
    explicit TableDefSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class TableDataSetting : public ObjectSetting
{
public:
    explicit TableDataSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class RelationSetting : public ObjectSetting
{
public:
    explicit RelationSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};


class QuerySetting : public ObjectSetting
{
public:
    explicit QuerySetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class FormSetting : public ObjectSetting
{
public:
    explicit FormSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class ReportSetting : public ObjectSetting
{
public:
    explicit ReportSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class MacroSetting : public ObjectSetting
{
public:
    explicit MacroSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class ModuleSetting : public ObjectSetting
{
public:
    explicit ModuleSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};

class ReferenceSetting : public ObjectSetting
{
public:
    explicit ReferenceSetting(ProjectSetting *parent);
//    virtual bool isTargetObject(const QAxObject *object) const;
};


#endif // OBJECTSETTING_H
