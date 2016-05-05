#include "moduleprocessor.h"

#include <QDebug>
#include <QTextCodec>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/accessprojectcontainer.h"
#include "cvsmodel/setting.h"

ModuleProcessor::ModuleProcessor(ProjectContainer *parent)
    : AccessObjectProcessor(parent)
{
    m_objectType          = Model::Module;
    m_selectObjectType    = Model::ModuleObjectType;
    m_accessObjectType    = Access::acModule;
    m_objectPathName      = "modules";
    m_containerName       = "Modules";
    m_iconPath            = ":/images/page.png";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = m_moduleFileExtension;
}

bool ModuleProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // we have to convert ONLY codec.

    // codec
//    determineCodecForProject();

    // FIXME: use fromLocal8Bit
    FileUtil::copyContents( filePath(TempDir, TempFile,   objectName), m_codecForProject,
                            filePath(TempDir, ModuleFile, objectName), m_codecForCvs, true/*removeTrailingSpaces*/ );
//    FileUtil::deleteFile(     tempFilePathInTempDir(objectName) ); // keep original files

    return true;
}

bool ModuleProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // we have to convert ONLY codec.

    // codec
//    determineCodecForProject();

    // FIXME: use toLocal8Bit
    FileUtil::copyContents( filePath(TempDir, ModuleFile, objectName), m_codecForCvs,
                            filePath(TempDir, TempFile,   objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
//    FileUtil::deleteFile(   moduleFilePathInTempDir(objectName) ); // keep original files

    return true;
}

bool ModuleProcessor::prepareItemCollection()
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessObjectProcessor::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectContainer->application<Access::Application>()->CurrentProject();
    m_objects.set( currentProject->AllModules() );

    return m_objects.is();
}

int ModuleProcessor::itemCount()
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessObjectProcessor::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *ModuleProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessObjectProcessor::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}

void ModuleProcessor::determineCodecForProject()
{
    // Module is always saved in Shift_JIS.
    // so we have to convert ONLY codec.
    if (!m_codecForProject)
    {
        m_codecForProject = new CodecInfo(this);
        m_codecForProject->setCodec( QTextCodec::codecForName("Shift_JIS") );
        m_codecForProject->setBom(false);
        m_codecForProject->setLineEnd("\r\n");
    }
}
