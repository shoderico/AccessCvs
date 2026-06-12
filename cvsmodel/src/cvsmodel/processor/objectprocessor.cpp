#include "objectprocessor.h"

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"
#include "util/setting.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/objectitem.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <QFileInfo>

#include "pch.hpp"


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
    // qDebug() << "copyFromTempDirToSourceDir" << objectName;
    deleteAllFileFromSourceDir(objectName);
    copyFile(TempDir, SourceDir, DesignFile,     objectName);
    copyFile(TempDir, SourceDir, ModuleFile,     objectName);
    copyFile(TempDir, SourceDir, DataFile,       objectName);
    copyFile(TempDir, SourceDir, ReportPropFile, objectName);
    copyFile(TempDir, SourceDir, OdbcFile,       objectName);
    return true;
}

bool ObjectProcessor::copyFromSourceDirToTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    copyFile(SourceDir, TempDir, DesignFile,     objectName);
    copyFile(SourceDir, TempDir, ModuleFile,     objectName);
    copyFile(SourceDir, TempDir, DataFile,       objectName);
    copyFile(SourceDir, TempDir, ReportPropFile, objectName);
    copyFile(SourceDir, TempDir, OdbcFile,       objectName);
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
    // odbc
    if (isSame && !m_odbcFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   OdbcFile, objectName),
                                    filePath(SourceDir, OdbcFile, objectName), false/*isSameIfBothNonExist*/ );
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
    // in subclass, override this function
    Q_UNUSED(objectName)
    return true;
}

bool ObjectProcessor::deleteFromTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    return true;
}

// -----------------------------------------------------------------------------
// removeDataFromSourceDir (post-UI-open HasData toggle OFF support, data-only)
// -----------------------------------------------------------------------------
// This is the off symmetric of ensureDataInTempDir (on toggle) + the dataCopyTargets dedicated path.
// Purpose: when user opens UI (refreshItems has already placed .dat in SourceDir for a then-hasData InBoth item),
//          then toggles HasData OFF in the tree, then selects the row and clicks Export, we must actively
//          remove the .dat from SourceDir (tabledefs/ or odbctables/) so that the "data not managed" intent is
//          reflected in the CVS working tree for this Export.
//
// Only the DataFile in SourceDir is deleted via deleteFile (safe if absent). Per explicit user decision
// "leave stale", we do NOT delete the corresponding .dat / .dattmp from TempDir here. They are left behind
// until the next full refreshItems (or manual sanitize). This keeps toggle-time behavior simple (no I/O in setData)
// and matches the on-fix leave-stale choice.
//
// Why this is needed (root cause):
// - setData(HasDataColumn) [objectmodel.cpp:210-222] does item->setHasData(false); setDifferent(Unchecked_OD); saveSettigs();
//   (m_tableDataTargets is updated via the processor's updateSetting). No file I/O at all (B decision: keep toggle responsive).
// - The 4 strict snapshots (targetsInBoth_Different etc.) require exact isDifferent() match at snapshot time.
//   Unchecked_OD (0) matches only InBoth_NotSame (!= SameContents) and therefore never enters the InBoth copy blocks.
// - InSourceDirOnly DeleteFromSourceDirCommand only covers items that are absent from Project (not our InBoth case).
// - copyFromTempDirToSourceDir always does deleteAll + Data copy, but off items never reach any Copy or full-Delete command.
// - ensure loop and dataCopyTargets only look at hasData==true.
// - compareTempDir has a !hasData branch (treats stray data files as different) but it is diagnostic only.
// Result (user symptom): "... SourceDirから .dat が消えないんだよね。"
//
// Approach (user confirmed C):
// - Dedicated lightweight virtual (no-op default) + dataRemoveTargets map built from the *broad* ensureDataCandidates
//   snapshot (which *does* capture the Unchecked InBoth_NotSame items).
// - Filter exactly as dataCopyTargets but inverted: selected && !hasData && InBoth (Present in both).
// - Call removeDataFromSourceDir (Source DataFile only) then same post-process trio used by dataCopyTargets
//   (UpdateFileTimeInTempDirCommand + UpdateItemsExportDateCommand + UpdateItemsDifference(SameContents)).
// - No changes to 4-bucket logic, getItems predicates, InBoth_NotSame block, or any structure-file handling.
// - Temp data left (leave stale); protection against re-copy relies on the item not being present in any
//   targetsInBoth_* or dataCopyTargets (hasData filter + Unchecked state).
//
// Called only for table processors that had hasData=true at refresh time and were toggled off post-open.
// OdbcTable and TableDef both covered via inheritance (no override needed in TableObjectProcessor).
// -----------------------------------------------------------------------------
void ObjectProcessor::removeDataFromSourceDir(const QString &objectName)
{
    // Data-only removal from SourceDir. Structure files are never touched.
    // Temp data files are left per "leave stale" decision (see long comment above).
    deleteFile(SourceDir, DataFile, objectName);
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
    FileUtil::setFileTime( filePath(TempDir, OdbcTempFile, objectName), fileTime, fileTime );
}

void ObjectProcessor::determineCodecForProject()
{
    // do nothing here.
    // if m_codecForProject is requred, override this function in subclass
}

bool ObjectProcessor::deleteCvsFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, DesignFile,     objectName);
    deleteFile(TempDir, ModuleFile,     objectName);
    deleteFile(TempDir, DataFile,       objectName);
    deleteFile(TempDir, ReportPropFile, objectName);
    deleteFile(TempDir, OdbcFile,       objectName);
    return true;
}

bool ObjectProcessor::deleteTempFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, TempFile,     objectName);
    deleteFile(TempDir, DataTempFile, objectName);
    deleteFile(TempDir, OdbcTempFile, objectName);
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
    deleteFile(SourceDir, DesignFile,     objectName);
    deleteFile(SourceDir, ModuleFile,     objectName);
    deleteFile(SourceDir, DataFile,       objectName);
    deleteFile(SourceDir, ReportPropFile, objectName);
    deleteFile(SourceDir, OdbcFile,       objectName);
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
            // qDebug() << "copyFile " << filePath(dirTypeSrc, fileType, objectName) << "to" << filePath(dirTypeDst, fileType, objectName);
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
        case OdbcTempFile:  return m_odbcTempFileExtension;
        case OdbcFile:      return m_odbcFileExtension;
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

Setting *ObjectProcessor::createSetting()
{
    QDir(m_projectContainer->sourcePath()).mkpath(".");

    CodecInfo codec;
    codec.setCodec( QTextCodec::codecForName("UTF-8") );
    codec.setBom(false);
    codec.setLineEnd("\r\n");

    Setting *setting = new Setting(m_projectContainer->sourcePath() + "\\" + m_settingFileName, codec.codec(), codec.bom(), codec.lineEnd());
    return setting;
}
