#include "relationprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "cvsmodel/projectcontainer.h"

#include "pch.hpp"



//=============================================================================
// Relation
// TODO: imeplement relationship export/import

RelationProcessor::RelationProcessor(ProjectContainer *parent)
    : ObjectProcessor(parent)
{
    m_objectType          = Model::Relation;
    m_selectObjectType    = Model::RelationObjectType;
    m_accessObjectType    = -1;
    m_objectPathName      = "relations";
    m_containerName       = "";
    m_iconPath            = ":/images/page_link.png";
    m_uiText              = "Relation";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rel";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}
