#ifndef OBJECTSETTING_H
#define OBJECTSETTING_H

#include <QObject>

#include <QFileInfo>

#include "objectitem.h"
#include "util/comptr.h"

class ProjectSetting;
class SanitizeSetting;
class CodecInfo;

class QAxObject;
class QSettings;


namespace DAO {
class TableDefs;
class QueryDefs;
class Containers;
class Container;
class Documents;
}
namespace Access {
class AllForms;
class AllReports;
class AllMacros;
class AllModules;
}



class ObjectSetting : public QObject
{
    Q_OBJECT
public:
    explicit ObjectSetting(ProjectSetting *parent = 0);


    virtual bool isTargetObject(QAxObject *object) const = 0;

    Model::ObjectType objectType() const { return m_objectType; }
//    int accessObjectType() const { return m_accessObjectType; } // unused

//    QString objectPathName() const { return m_objectPathName; } // unused
//    QString containerName() const { return m_containerName; } // unused

//    QString tempFileExtension()   const { return m_tempFileExtension; } // unused
//    QString designFileExtension() const { return m_designFileExtension; } // unused
//    QString moduleFileExtension() const { return m_moduleFileExtension; } // unused
    QString existCheckExtension() const { return m_existCheckExtension; }

public:
    // file path
//    QString tempFilePathInTempDir  (const QString &objectName); // ObjectSettings only    // unused
    QString designFilePathInTempDir(const QString &objectName); // ObjectSettings only
    QString moduleFilePathInTempDir(const QString &objectName); // ObjectSettings only

    QString tempFilePathInSourceDir  (const QString &objectName); // ObjectSettings only
    QString designFilePathInSourceDir(const QString &objectName); // ObjectSettings only
    QString moduleFilePathInSourceDir(const QString &objectName); // ObjectSettings only

    // object path
    QString sourceObjectPath() const;
    QString tempObjectPath() const; // ObjectSettings only

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

    virtual bool prepareItemCollection();
    virtual int itemCount();
    virtual QAxObject *itemUnsafePtr(const QVariant &index);

    void updateFileTimeInTempDir(const QString &objectName, const QDateTime &fileTime);

    bool deleteTempFileFromTempDir(const QString &objectName);

    virtual void loadSettings(QSettings *settings) { Q_UNUSED(settings) }
    virtual void saveSettings(QSettings *settings) { Q_UNUSED(settings) }

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
    bool deleteCvsFileFromTempDir(const QString &objectName);
    bool copyFile(DirectoryType dirTypeSrc, DirectoryType dirTypeDst, FileType fileType, const QString &objectName);
    bool deleteFile(DirectoryType dirType, FileType fileType, const QString &objectName);

    QString rootPath(DirectoryType dirType) const;
    QString objectPath(DirectoryType dirType) const;
    QString fileExtension(FileType fileType) const;
    QString filePath(DirectoryType dirType, FileType fileType, const QString &objectName) const;

    void mkpathObjectPath(DirectoryType dirType);

    virtual void determineCodecForProject();


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

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);

    virtual void loadSettings(QSettings *settings);
    virtual void saveSettings(QSettings *settings);

    void setTableDataTargets(QStringList *newTargets);
protected:
    virtual void determineCodecForProject();
    ComPtr<DAO::TableDefs> m_tableDefs;
    QStringList m_tableDataTargets;
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


class QueryAsSqlSetting : public ObjectSetting
{
public:
    explicit QueryAsSqlSetting(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<DAO::QueryDefs> m_queryDefs;
};

class QueryAsObjectSetting : public QueryAsSqlSetting
{
public:
    explicit QueryAsObjectSetting(ProjectSetting *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
};




class AccessObjectSetting : public ObjectSetting
{
public:
    explicit AccessObjectSetting(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<DAO::Containers> m_containers;
    ComPtr<DAO::Container> m_container;
    ComPtr<DAO::Documents> m_documents;
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
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllForms> m_objects;
};

class ReportSetting : public AccessDesignObjectSetting
{
public:
    explicit ReportSetting(ProjectSetting *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllReports> m_objects;
};

class MacroSetting : public AccessDesignObjectSetting
{
public:
    explicit MacroSetting(ProjectSetting *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllMacros> m_objects;
};

class ModuleSetting : public AccessObjectSetting
{
public:
    explicit ModuleSetting(ProjectSetting *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    virtual void determineCodecForProject();
    ComPtr<Access::AllModules> m_objects;
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

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
private:
    QString m_objectName;
    // ComPtr<Access::XXX> m_objects; // no need
};


#endif // OBJECTSETTING_H
