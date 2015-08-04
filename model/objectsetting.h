#ifndef OBJECTSETTING_H
#define OBJECTSETTING_H

#include <QObject>

#include <QFileInfo>

#include "objectitem.h"

class ProjectSetting;
class SanitizeSetting;
class CodecInfo;

class QAxObject;






class ObjectSetting : public QObject
{
    Q_OBJECT
public:
    explicit ObjectSetting(ProjectSetting *parent = 0);


    virtual bool isTargetObject(QAxObject *object) const = 0;

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
    QString tempFilePathInTempDir  (const QString &objectName);
    QString designFilePathInTempDir(const QString &objectName);
    QString moduleFilePathInTempDir(const QString &objectName);

    QString tempFilePathInSourceDir  (const QString &objectName);
    QString designFilePathInSourceDir(const QString &objectName);
    QString moduleFilePathInSourceDir(const QString &objectName);

    // object path
    QString sourceObjectPath() const;
    QString tempObjectPath() const;

    // object path
    void mkdirSourceObjectPath();
    void mkdirTempObjectPath();


public:
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0) = 0;
    virtual ObjectItem *createItemFromFileSystem(QFileInfo &fileInfo, QObject *parent = 0);

    virtual bool exportFromProjectToTempDir(QAxObject* object, const QString &objectName) = 0;
    virtual bool importFromTempDirToProject(QAxObject* object, const QString &objectName) = 0;

    virtual bool sanitizeTempDir(QAxObject* object, const QString &objectName) = 0;
    virtual bool desanitizeTempDir(QAxObject* object, const QString &objectName) = 0;

    virtual bool copyFromTempDirToFileSystem(const QString &objectName);
    virtual bool copyFromFileSystemToTempDir(const QString &objectName);

    virtual bool compareTempDir(const QString &objectName, bool *pisDifferent);

    virtual bool deleteFromFileSystem(const QString &objectName);
    virtual bool deleteFromProject(const QString &objectName);

    virtual void determineCodecForProject();

protected:
    enum DirectoryType
    {
        TempDir,
        SourceDir
    };
    enum FileType
    {
        TempFile,
        DesignFile,
        ModuleFile
    };
    bool deleteFromTempDir(const QString &objectName);
    bool copyFile(DirectoryType dirTypeSrc, DirectoryType dirTypeDst, FileType fileType, const QString &objectName);
    bool deleteFile(DirectoryType dirType, FileType fileType, const QString &objectName);

    QString rootPath(DirectoryType dirType) const;
    QString objectPath(DirectoryType dirType) const;
    QString fileExtension(FileType fileType) const;
    QString filePath(DirectoryType dirType, FileType fileType, const QString &objectName) const;

    void mkpathObjectPath(DirectoryType dirType);


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

    CodecInfo *m_codecForCvs;
    CodecInfo *m_codecForProject;
};

/*
    ObjectSetting
        TableDefSetting
        TableDataSetting
        RelationSetting
        QuerySetting
        FormSetting
        ReportSetting
        MacroSetting
        ModuleSetting
        ReferenceSetting

*/


class TableDefSetting : public ObjectSetting
{
public:
    explicit TableDefSetting(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
protected:
    virtual void determineCodecForProject();
};

class TableDataSetting : public ObjectSetting
{
public:
    explicit TableDataSetting(ProjectSetting *parent);
};

class RelationSetting : public ObjectSetting
{
public:
    explicit RelationSetting(ProjectSetting *parent);
};


class QuerySetting : public ObjectSetting
{
public:
    explicit QuerySetting(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
};




class AccessObjectSetting : public ObjectSetting
{
public:
    explicit AccessObjectSetting(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
};




class AccessDesignObjectSetting : public AccessObjectSetting
{
public:
    explicit AccessDesignObjectSetting(ProjectSetting *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
protected:
    virtual void determineCodecForProject();
    SanitizeSetting *m_sanitizer;
};




class FormSetting : public AccessDesignObjectSetting
{
public:
    explicit FormSetting(ProjectSetting *parent);
};

class ReportSetting : public AccessDesignObjectSetting
{
public:
    explicit ReportSetting(ProjectSetting *parent);
};

class MacroSetting : public AccessDesignObjectSetting
{
public:
    explicit MacroSetting(ProjectSetting *parent);
};

class ModuleSetting : public AccessObjectSetting
{
public:
    explicit ModuleSetting(ProjectSetting *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
protected:
    virtual void determineCodecForProject();
};








class ReferenceSetting : public ObjectSetting
{
public:
    explicit ReferenceSetting(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
private:
    QString m_objectName;
};


#endif // OBJECTSETTING_H
