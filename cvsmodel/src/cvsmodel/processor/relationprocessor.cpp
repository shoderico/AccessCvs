#include "relationprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "cvsmodel/projectsetting.h"



//=============================================================================
// Relation
// TODO: imeplement relationship export/import

RelationProcessor::RelationProcessor(ProjectSetting *parent)
    : ObjectProcessor(parent)
{
    m_objectType          = Model::Relation;
    m_accessObjectType    = -1;
    m_objectPathName      = "relations";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rel";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}
