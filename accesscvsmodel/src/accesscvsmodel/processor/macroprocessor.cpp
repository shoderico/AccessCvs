#include "macroprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/comptr.h"

#include "accesscvsmodel/accessprojectcontainer.h"
#include "util/setting.h"

#include "pch.hpp"


//=============================================================================
// Macro

MacroProcessor::MacroProcessor(ProjectContainer *parent)
    : AccessDesignObjectProcessor(parent)
{
    m_objectType          = Model::Macro;
    m_selectObjectType    = Model::MacroObjectType;
    m_accessObjectType    = Access::acMacro;
    m_objectPathName      = "macros";
    m_containerName       = "Scripts";
    m_iconPath            = ":/images/script.png";
    m_uiText              = "Macros";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "mcr";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}

bool MacroProcessor::prepareItemCollection()
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessDesignObjectProcessor::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectContainer->application<Access::Application>()->CurrentProject();
    m_objects.set( currentProject->AllMacros() );

    return m_objects.is();
}

int MacroProcessor::itemCount()
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessDesignObjectProcessor::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *MacroProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!projectContainer<AccessProjectContainer>()->isADP() && !projectContainer<AccessProjectContainer>()->isMDB())
        return AccessDesignObjectProcessor::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}
