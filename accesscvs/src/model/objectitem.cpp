#include "objectitem.h"

ObjectItem::ObjectItem(QObject *parent)
    : QObject(parent)
    , m_isSelected(false)
    , m_hasData(false)
    , m_inProject(Model::OE_Unchecked)
    , m_inSourceDir(Model::OE_Unchecked)
    , m_isDifferent(Model::OD_Unchecked)
    , m_objectType(Model::Unknwon)
    , m_gitStatusInIndex(Model::GS_Unknown)
    , m_gitStatusInWorkTree(Model::GS_Unknown)
{

}

ObjectItem::ObjectItem(ObjectItem *object, QObject *parent)
    : QObject(parent)
{
    // copy member
    this->m_name = object->m_name;

    this->m_createDate = object->m_createDate;
    this->m_updateDate = object->m_updateDate;
    this->m_exportDate = object->m_exportDate;
    this->m_isSelected = object->m_isSelected;
    this->m_hasData = object->m_hasData;
    this->m_inProject = object->m_inProject;
    this->m_inSourceDir = object->m_inSourceDir;
    this->m_isDifferent = object->m_isDifferent;
    this->m_objectType = object->m_objectType;
    this->m_gitStatusInIndex = object->m_gitStatusInIndex;
    this->m_gitStatusInWorkTree = object->m_gitStatusInWorkTree;

}

bool ObjectItem::isModified() const
{
    return !( inProject() && inSourceDir() &&
              updateDate().isValid() && exportDate().isValid() &&
              updateDate() <= exportDate() );
}
