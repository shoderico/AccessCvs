#include "formprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/comptr.h"

#include "cvsmodel/projectcontainer.h"


FormProcessor::FormProcessor(ProjectContainer *parent)
    : AccessDesignObjectProcessor(parent)
{
    m_objectType          = Model::Form;
    m_selectObjectType    = Model::FormObjectType;
    m_accessObjectType    = Access::acForm;
    m_objectPathName      = "forms";
    m_containerName       = "Forms";
    m_iconPath            = ":/images/application_form.png";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "frm";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = m_designFileExtension;
}

bool FormProcessor::prepareItemCollection()
{
    if (!m_projectContainer->isADP() && !m_projectContainer->isMDB())
        return AccessDesignObjectProcessor::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectContainer->application()->CurrentProject();
    m_objects.set( currentProject->AllForms() );

    return m_objects.is();
}

int FormProcessor::itemCount()
{
    if (!m_projectContainer->isADP() && !m_projectContainer->isMDB())
        return AccessDesignObjectProcessor::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *FormProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectContainer->isADP() && !m_projectContainer->isMDB())
        return AccessDesignObjectProcessor::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}
