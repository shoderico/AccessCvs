#include "objectitem.h"

ObjectItem::ObjectItem(QObject *parent)
    : QObject(parent)
    , m_isSelected(false)
    , m_inProject(Model::OE_Unchecked)
    , m_inFileSystem(Model::OE_Unchecked)
    , m_isDifferent(Model::OD_Unchecked)
    , m_objectType(Model::Unknwon)
    , m_gitStatusInIndex(Model::GS_Unknown)
    , m_gitStatusInWorkTree(Model::GS_Unknown)
{

}
