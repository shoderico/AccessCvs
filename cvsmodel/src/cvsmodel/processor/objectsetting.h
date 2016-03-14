#ifndef OBJECTSETTING_H
#define OBJECTSETTING_H

#include "cvsmodel/cvsmodel_global.h"

#include <QObject>

#include <QFileInfo>
#include <QReadWriteLock>
#include <QVariant>

#include "cvsmodel/objectitem.h"
#include "util/comptr.h"

class ProjectSetting;
class AccessDesignObjectSanitizer;
class TableDefSanitizer;
class TableDataSanitizer;
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
namespace VBIDE {
class VBProject;
}


class CVSMODEL_SHARED_EXPORT ObjectProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ObjectProcessor(ProjectSetting *parent = 0);

    virtual bool isTargetObject(QAxObject *object) const = 0;

    Model::ObjectType objectType() const { return m_objectType; }

    QString existCheckExtension() const { return m_existCheckExtension; }

public:
    // object path
    QString sourceObjectPath() const;
    QString tempObjectPath() const; // ObjectSettings only

    // object path
    void mkdirSourceObjectPath();
    void mkdirTempObjectPath();


public:
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0) = 0;
    virtual ObjectItem *createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent = 0);

    virtual bool exportFromProjectToTempDir(QAxObject* object, const QString &objectName) = 0;
    virtual bool importFromTempDirToProject(QAxObject* object, const QString &objectName) = 0;
    virtual bool afterImportFromTempDirToProject(QAxObject* object, const QString &objectName);

    virtual bool sanitizeTempDir(QAxObject* object, const QString &objectName) = 0;
    virtual bool desanitizeTempDir(QAxObject* object, const QString &objectName) = 0;

    virtual bool copyFromTempDirToSourceDir(const QString &objectName);
    virtual bool copyFromSourceDirToTempDir(const QString &objectName);

    virtual bool compareTempDir(const QString &objectName, bool *pisDifferent);

    virtual bool deleteFromSourceDir(const QString &objectName);
    virtual bool deleteFromProject(const QString &objectName);
    virtual bool deleteFromTempDir(const QString &objectName);

    virtual bool prepareItemCollection();
    virtual int itemCount();
    virtual QAxObject *itemUnsafePtr(const QVariant &index);

    void updateFileTimeInTempDir(const QString &objectName, const QDateTime &fileTime);

    virtual void loadSettings(QSettings *settings) { Q_UNUSED(settings) }
    virtual void saveSettings(QSettings *settings) { Q_UNUSED(settings) }

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
        ModuleFile,
        DataTempFile,
        DataFile,
        ReportPropFile,
    };
    bool deleteCvsFileFromTempDir(const QString &objectName);
    bool deleteTempFileFromTempDir(const QString &objectName);
    bool deleteAllFileFromTempDir(const QString &objectName);
    bool deleteAllFileFromSourceDir(const QString &objectName);

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
    QString m_dataTempFileExtension;
    QString m_dataFileExtension;
    QString m_reportPropFileExtension;

    ProjectSetting *m_projectSetting;

    CodecInfo *m_codecForCvs;
    CodecInfo *m_codecForProject;
};



class TableDefProcessor : public ObjectProcessor
{
public:
    explicit TableDefProcessor(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual ObjectItem *createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent = 0);
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
    virtual void determineCodecForProject();
protected:
    ComPtr<DAO::TableDefs> m_tableDefs;
    QStringList m_tableDataTargets;
    TableDefSanitizer *m_tableDefSanitizer;
    TableDataSanitizer *m_tableDataSanitizer;
};

//class TableDataSetting : public ObjectSetting
//{
//public:
//    explicit TableDataSetting(ProjectSetting *parent);
//};

class RelationProcessor : public ObjectProcessor
{
public:
    explicit RelationProcessor(ProjectSetting *parent);
};


class QueryAsSqlProcessor : public ObjectProcessor
{
public:
    explicit QueryAsSqlProcessor(ProjectSetting *parent);
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

class QueryAsObjectProcessor : public QueryAsSqlProcessor
{
public:
    explicit QueryAsObjectProcessor(ProjectSetting *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
};




class AccessObjectProcessor : public ObjectProcessor
{
public:
    explicit AccessObjectProcessor(ProjectSetting *parent);
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




class AccessDesignObjectProcessor : public AccessObjectProcessor
{
public:
    explicit AccessDesignObjectProcessor(ProjectSetting *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual void determineCodecForProject();

protected:
    virtual bool afterSanitizeTempDir(QAxObject* object, const QString &objectName, AccessDesignObjectSanitizer *sanitizer);
    AccessDesignObjectSanitizer *m_sanitizer;
};




class FormProcessor : public AccessDesignObjectProcessor
{
public:
    explicit FormProcessor(ProjectSetting *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllForms> m_objects;
};

class ReportProcessor : public AccessDesignObjectProcessor
{
public:
    explicit ReportProcessor(ProjectSetting *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
    virtual bool        afterImportFromTempDirToProject(QAxObject* object, const QString &objectName);
protected:
    virtual bool afterSanitizeTempDir(QAxObject *object, const QString &objectName, AccessDesignObjectSanitizer *sanitizer);
    ComPtr<Access::AllReports> m_objects;
    QReadWriteLock m_lock;
};

class MacroProcessor : public AccessDesignObjectProcessor
{
public:
    explicit MacroProcessor(ProjectSetting *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllMacros> m_objects;
};

class ModuleProcessor : public AccessObjectProcessor
{
public:
    explicit ModuleProcessor(ProjectSetting *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
    virtual void determineCodecForProject();
protected:
    ComPtr<Access::AllModules> m_objects;
};





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



class ReferenceSetting : public ProjectLevelObjectProcessor
{
public:
    explicit ReferenceSetting(ProjectSetting *parent);
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




class VBProjectProcessor : public ProjectLevelObjectProcessor
{
public:
    explicit VBProjectProcessor(ProjectSetting *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);

private:
    VBIDE::VBProject *currentVBProject();
};


#endif // OBJECTSETTING_H
