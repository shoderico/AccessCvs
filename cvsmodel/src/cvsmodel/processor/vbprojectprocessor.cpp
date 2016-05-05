#include "vbprojectprocessor.h"

#include <QDebug>
#include <QFile>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"

#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/setting.h"


VBProjectProcessor::VBProjectProcessor(ProjectContainer *parent)
    : ProjectLevelObjectProcessor(parent)
{
    m_objectName          = "VBEProject";

    m_objectType          = Model::VBProject;
    m_selectObjectType    = Model::VBProjectType;
    m_accessObjectType    = -1;
    m_objectPathName      = "";
    m_containerName       = "";
    m_iconPath            = ":/images/page_link.png";

    m_tempFileExtension   = "vbeproj";
    m_designFileExtension = m_tempFileExtension;
    m_moduleFileExtension = "";
    m_existCheckExtension = m_tempFileExtension;
}

bool VBProjectProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    deleteAllFileFromTempDir(m_objectName);

    // QSettings
    /*
    QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    ComPtr<VBIDE::VBProject> vbProject = currentVBProject();
    if (vbProject.is())
    {
        settings.setValue("Name",           vbProject->Name());
        settings.setValue("Description",    vbProject->Description());
        settings.setValue("HelpContextID",  vbProject->HelpContextID());
        settings.setValue("HelpFile",       vbProject->HelpFile());
    }
    */

    // Implement Own

    // Setting
    Setting setting( filePath(TempDir, TempFile, m_objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    ComPtr<VBIDE::VBProject> vbProject = currentVBProject();
    if (vbProject.is())
    {
        SettingElement *element = setting.append("VBProject");
        element->append("Name",             vbProject->Name());
        element->append("Description",      vbProject->Description());
        element->append("HelpContextID",    vbProject->HelpContextID());
        element->append("HelpFile",         vbProject->HelpFile());
    }
    setting.save();

    return true;
}

bool VBProjectProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    if ( !QFile( filePath(TempDir, TempFile, m_objectName) ).exists())
        return true;

    // QSettings
    /*
    QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    ComPtr<VBIDE::VBE> vbe = m_projectSetting->application()->VBE();
    ComPtr<VBIDE::VBProject> vbProject = currentVBProject();
    if (vbProject.is())
    {
        vbProject->SetName(             settings.value("Name",          QString()).toString() );
        vbProject->SetDescription(      settings.value("Description",   QString()).toString() );
        vbProject->SetHelpContextID(    settings.value("HelpContextID", 0).toInt() );
        vbProject->SetHelpFile(         settings.value("HelpFile",      QString()).toString() );
    }
    */

    // Implement Own

    // Setting
    Setting setting( filePath(TempDir, TempFile, m_objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    if (!setting.load())
        return false;

    SettingElement *element = setting.at(0)->toElement();
    Q_ASSERT(element != NULL);
    Q_ASSERT(element->name() == "VBProject");

    ComPtr<VBIDE::VBProject> vbProject = currentVBProject();
    if (vbProject.is())
    {
        vbProject->SetName(             element->value("Name"            ).toString() );
        vbProject->SetDescription(      element->value("Description"     ).toString() );
        vbProject->SetHelpContextID(    element->value("HelpContextID", 0).toInt() );
        vbProject->SetHelpFile(         element->value("HelpFile"        ).toString() );
    }

    return true;
}

VBIDE::VBProject *VBProjectProcessor::currentVBProject()
{
    ComPtr<Access::CurrentProject> currentProject = m_projectContainer->application<Access::Application>()->CurrentProject();
    QString fileName = currentProject->FullName();

    ComPtr<VBIDE::VBE> vbe = m_projectContainer->application<Access::Application>()->VBE();
    ComPtr<VBIDE::VBProjects> vbProjects = vbe->VBProjects();
    for ( int i = 1 ; i <= vbProjects->Count(); ++i )
    {
        ComPtr<VBIDE::VBProject> vbp = vbProjects->Item(i);
        if (vbp->FileName() == fileName)
        {
            return vbProjects->Item(i);
        }
    }
    return NULL;
}





