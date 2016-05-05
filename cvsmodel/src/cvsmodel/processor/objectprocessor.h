#ifndef OBJECTPROCESSOR_H
#define OBJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QObject>


class ProjectContainer;
class CodecInfo;
class ObjectItem;

class QAxObject;
class QSettings;
class QFileInfo;

class CVSMODEL_SHARED_EXPORT ObjectProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ObjectProcessor(ProjectContainer *parent = 0);

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

    virtual bool compareTempDir(const QString &objectName, const ObjectItem *item, bool *pisDifferent);

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
    Model::SelectObjectType m_selectObjectType;
    QString m_objectPathName;
    QString m_containerName;
    QString m_iconPath;

    QString m_tempFileExtension;
    QString m_designFileExtension;
    QString m_moduleFileExtension;
    QString m_existCheckExtension;
    QString m_dataTempFileExtension;
    QString m_dataFileExtension;
    QString m_reportPropFileExtension;

    ProjectContainer *m_projectContainer;

    CodecInfo *m_codecForCvs;
    CodecInfo *m_codecForProject;
};


#endif // OBJECTPROCESSOR_H
