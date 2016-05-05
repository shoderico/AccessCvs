#include "objectprocessor.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <QFileInfo>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/objectitem.h"


ObjectProcessor::ObjectProcessor(ProjectContainer *parent)
    : QObject(parent)
    , m_projectContainer(parent)
    , m_codecForCvs(new CodecInfo(this))
    , m_codecForProject(0)
{
    m_codecForCvs->setCodec( QTextCodec::codecForName("UTF-8") );
    m_codecForCvs->setBom(false);
    m_codecForCvs->setLineEnd("\r\n");
}


QString ObjectProcessor::sourceObjectPath() const
{
    return objectPath(SourceDir);
}

QString ObjectProcessor::tempObjectPath() const
{
    return objectPath(TempDir);
}

void ObjectProcessor::mkdirTempObjectPath()
{
    mkpathObjectPath(TempDir);
}

void ObjectProcessor::mkdirSourceObjectPath()
{
    mkpathObjectPath(SourceDir);
}



ObjectItem *ObjectProcessor::createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( m_objectType );
    item->setSelectObjectType( m_selectObjectType );
    item->setIconPath( m_iconPath );
    item->setName( fileInfo.completeBaseName() );
    item->setInSourceDir( Model::Present );

    return item;
}

bool ObjectProcessor::afterImportFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    return true;
}

bool ObjectProcessor::copyFromTempDirToSourceDir(const QString &objectName)
{
    deleteAllFileFromSourceDir(objectName);
    copyFile(TempDir, SourceDir, DesignFile, objectName);
    copyFile(TempDir, SourceDir, ModuleFile, objectName);
    copyFile(TempDir, SourceDir, DataFile,   objectName);
    copyFile(TempDir, SourceDir, ReportPropFile, objectName);
    return true;
}

bool ObjectProcessor::copyFromSourceDirToTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    copyFile(SourceDir, TempDir, DesignFile, objectName);
    copyFile(SourceDir, TempDir, ModuleFile, objectName);
    copyFile(SourceDir, TempDir, DataFile,   objectName);
    copyFile(SourceDir, TempDir, ReportPropFile, objectName);
    return true;
}

bool ObjectProcessor::compareTempDir(const QString &objectName, const ObjectItem *item, bool *pisDifferent)
{
    bool isSame = true;

    // design
    if (isSame && !m_designFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   DesignFile, objectName),
                                    filePath(SourceDir, DesignFile, objectName), false/*isSameIfBothNonExist*/ );
    }
    // module
    if (isSame && !m_moduleFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   ModuleFile, objectName),
                                    filePath(SourceDir, ModuleFile, objectName), false/*isSameIfBothNonExist*/ );
    }
    // data
    if (isSame && !m_dataFileExtension.isEmpty())
    {
        // if item has data, compare normally
        if ( item->hasData() )
        {
            isSame = FileUtil::compare( filePath(TempDir,   DataFile, objectName),
                                        filePath(SourceDir, DataFile, objectName), false/*isSameIfBothNonExist*/ );
        }
        // if item NOT has data, treat as different if any file exists.
        else
        {
            QFile fileA( filePath(TempDir,   DataFile, objectName) );
            QFile fileB( filePath(SourceDir, DataFile, objectName) );
            if ( fileA.exists() || fileB.exists() )
                isSame = false;
        }
    }
    // report-prop
    if (isSame && !m_reportPropFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   ReportPropFile, objectName),
                                    filePath(SourceDir, ReportPropFile, objectName), false/*isSameIfBothNonExist*/ );
    }

    *pisDifferent = !isSame;

    return true;
}

bool ObjectProcessor::deleteFromSourceDir(const QString &objectName)
{
    deleteAllFileFromSourceDir(objectName);
    return true;
}

bool ObjectProcessor::deleteFromProject(const QString &objectName)
{
    // TODO: cache DoCmd object
    if (m_accessObjectType != -1)
    {
        ComPtr<Access::DoCmd> doCmd = m_projectContainer->application<Access::Application>()->DoCmd();
        doCmd->DeleteObject( (Access::AcObjectType)m_accessObjectType, objectName );
    }
    return true;
}

bool ObjectProcessor::deleteFromTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    return true;
}

bool ObjectProcessor::prepareItemCollection()
{
    // in subclass, override this function
    // and prepare collection object to call Count() and Item().
    return true;
}

int ObjectProcessor::itemCount()
{
    // in subclass, override this function
    // and return the value of Count().
    return 0;
}

QAxObject *ObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    // in subclass, override this function
    // and return QAxObject pointer.
    // this pointer must be deleted by CALLER.
    Q_UNUSED(index)
    return NULL;
}

void ObjectProcessor::updateFileTimeInTempDir(const QString &objectName, const QDateTime &fileTime)
{
    FileUtil::setFileTime( filePath(TempDir, TempFile,     objectName), fileTime, fileTime );
    FileUtil::setFileTime( filePath(TempDir, DataTempFile, objectName), fileTime, fileTime );
}

void ObjectProcessor::determineCodecForProject()
{
    // do nothing here.
    // if m_codecForProject is requred, override this function in subclass
}

bool ObjectProcessor::deleteCvsFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, DesignFile, objectName);
    deleteFile(TempDir, ModuleFile, objectName);
    deleteFile(TempDir, DataFile,   objectName);
    deleteFile(TempDir, ReportPropFile, objectName);
    return true;
}

bool ObjectProcessor::deleteTempFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, TempFile,     objectName);
    deleteFile(TempDir, DataTempFile, objectName);
    return true;
}

bool ObjectProcessor::deleteAllFileFromTempDir(const QString &objectName)
{
    deleteTempFileFromTempDir(objectName);
    deleteCvsFileFromTempDir(objectName);
    return true;
}

bool ObjectProcessor::deleteAllFileFromSourceDir(const QString &objectName)
{
    deleteFile(SourceDir, DesignFile, objectName);
    deleteFile(SourceDir, ModuleFile, objectName);
    deleteFile(SourceDir, DataFile,   objectName);
    deleteFile(SourceDir, ReportPropFile, objectName);
    return true;
}





bool ObjectProcessor::copyFile(ObjectProcessor::DirectoryType dirTypeSrc, ObjectProcessor::DirectoryType dirTypeDst, ObjectProcessor::FileType fileType, const QString &objectName)
{
    if (!fileExtension(fileType).isEmpty())
    {
        QFile fileSrc(    filePath(dirTypeSrc, fileType, objectName) );
        if (fileSrc.exists())
        {
            mkpathObjectPath(dirTypeDst);
            fileSrc.copy( filePath(dirTypeDst, fileType, objectName) );
        }
    }
    return true;
}

bool ObjectProcessor::deleteFile(ObjectProcessor::DirectoryType dirType, ObjectProcessor::FileType fileType, const QString &objectName)
{
    if (!fileExtension(fileType).isEmpty())
        FileUtil::deleteFile( filePath(dirType, fileType, objectName) );

    return true;
}








QString ObjectProcessor::rootPath(ObjectProcessor::DirectoryType dirType) const
{
    switch (dirType)
    {
        case TempDir:      return m_projectContainer->tempPath();
        case SourceDir:    return m_projectContainer->sourcePath();
    }
    return QString();
}

QString ObjectProcessor::objectPath(ObjectProcessor::DirectoryType dirType) const
{
    QString path = rootPath(dirType);
    if ( !m_objectPathName.isEmpty() )
        path += "\\" + m_objectPathName;
    return path;
}

QString ObjectProcessor::fileExtension(ObjectProcessor::FileType fileType) const
{
    switch (fileType)
    {
        case TempFile:      return m_tempFileExtension;
        case DesignFile:    return m_designFileExtension;
        case ModuleFile:    return m_moduleFileExtension;
        case DataTempFile:  return m_dataTempFileExtension;
        case DataFile:      return m_dataFileExtension;
        case ReportPropFile: return m_reportPropFileExtension;
    }
    return QString();
}

QString ObjectProcessor::filePath(ObjectProcessor::DirectoryType dirType, ObjectProcessor::FileType fileType, const QString &objectName) const
{
    return objectPath(dirType) + "\\" + objectName + "." + fileExtension(fileType);
}

void ObjectProcessor::mkpathObjectPath(ObjectProcessor::DirectoryType dirType)
{
    QDir dir( objectPath(dirType) );
    dir.mkpath(".");
}
