#include "objectitem.h"

#include "officelib/officelib.h"

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

//ObjectItem *ObjectItem::fromTableDef(DAO::TableDef *tableDef, QObject *parent)
//{
//    ObjectItem *item = new ObjectItem(parent);

//    item->setObjectType( Model::TableDef );
//    item->setName( tableDef->Name() );
//    item->setInProject( Model::Present );
//    item->setCreateDate( tableDef->DateCreated().toDateTime() );
//    item->setUpdateDate( tableDef->LastUpdated().toDateTime() );

//    return item;
//}

//ObjectItem *ObjectItem::fromQueryDef(DAO::QueryDef *queryDef, QObject *parent)
//{
//    ObjectItem *item = new ObjectItem(parent);

//    item->setObjectType( Model::Query );
//    item->setName( queryDef->Name() );
//    item->setInProject( Model::Present );
//    item->setCreateDate( queryDef->DateCreated().toDateTime() );
//    item->setUpdateDate( queryDef->LastUpdated().toDateTime() );

//    return item;
//}

//ObjectItem *ObjectItem::fromAccessObject(Model::ObjectType objectType, Access::AccessObject *accessObject, QObject *parent)
//{
//    ObjectItem *item = new ObjectItem(parent);

//    item->setObjectType( objectType );
//    item->setName( accessObject->Name() );
//    item->setInProject( Model::Present );
//    item->setCreateDate( accessObject->DateCreated() );
//    item->setUpdateDate( accessObject->DateModified() );

//    return item;
//}

//ObjectItem *ObjectItem::fromDAODocument(Model::ObjectType objectType, DAO::Document *daoDocument, QObject *parent)
//{
//    ObjectItem *item = new ObjectItem(parent);

//    item->setObjectType( objectType );
//    item->setName( daoDocument->Name() );
//    item->setInProject( Model::Present );
//    item->setCreateDate( daoDocument->DateCreated().toDateTime() );
//    item->setUpdateDate( daoDocument->LastUpdated().toDateTime() );

//    return item;
//}

ObjectItem *ObjectItem::fromFileInfo(Model::ObjectType objectType, QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( objectType );
    item->setName( fileInfo.completeBaseName() );
    item->setInFileSystem( Model::Present );

    return item;
}

