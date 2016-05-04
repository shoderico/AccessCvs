#include "macroprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/comptr.h"

#include "cvsmodel/projectsetting.h"
#include "cvsmodel/setting.h"


//=============================================================================
// Macro

MacroProcessor::MacroProcessor(ProjectSetting *parent)
    : AccessDesignObjectProcessor(parent)
{
    m_objectType          = Model::Macro;
    m_selectObjectType    = Model::MacroObjectType;
    m_accessObjectType    = Access::acMacro;
    m_objectPathName      = "macros";
    m_containerName       = "Scripts";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "mcr";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}

bool MacroProcessor::prepareItemCollection()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectProcessor::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllMacros() );

    return m_objects.is();
}

int MacroProcessor::itemCount()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectProcessor::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *MacroProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectProcessor::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}
