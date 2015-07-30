#include "objectitem.h"

#include "officelib/officelib.h"

ObjectItem::ObjectItem(QObject *parent)
    : QObject(parent)
    , m_inProject(false)
    , m_inFileSystem(false)
    , m_isSelected(false)
{

}

ObjectItem *ObjectItem::fromTableDef(DAO::TableDef *tableDef, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( Model::Table );
    item->setName( tableDef->Name() );
    item->setInProject( true );
    item->setCreateDate( tableDef->DateCreated().toDateTime() );
    item->setUpdateDate( tableDef->LastUpdated().toDateTime() );

    return item;
}

ObjectItem *ObjectItem::fromQueryDef(DAO::QueryDef *queryDef, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( Model::Query );
    item->setName( queryDef->Name() );
    item->setInProject( true );
    item->setCreateDate( queryDef->DateCreated().toDateTime() );
    item->setUpdateDate( queryDef->LastUpdated().toDateTime() );

    return item;
}

ObjectItem *ObjectItem::fromAccessObject(Model::ObjectType objectType, Access::AccessObject *accessObject, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( objectType );
    item->setName( accessObject->Name() );
    item->setInProject( true );
    item->setCreateDate( accessObject->DateCreated() );
    item->setUpdateDate( accessObject->DateModified() );

    return item;
}

ObjectItem *ObjectItem::fromDAODocument(Model::ObjectType objectType, DAO::Document *daoDocument, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( objectType );
    item->setName( daoDocument->Name() );
    item->setInProject( true );
    item->setCreateDate( daoDocument->DateCreated().toDateTime() );
    item->setUpdateDate( daoDocument->LastUpdated().toDateTime() );

    return item;
}

ObjectItem *ObjectItem::fromFileInfo(Model::ObjectType objectType, QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( objectType );
    item->setName( fileInfo.completeBaseName() );
    item->setInFileSystem( true );

    return item;
}

