#include "objectitemmap.h"

ObjectItemMap::ObjectItemMap()
{
    // add Object-Level object types ( not Project-Level )
    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                   ;
    foreach ( Model::ObjectType objectType, objectTypes )
        insert( objectType, QMap< QString, ObjectItem* >() );
}

void ObjectItemMap::clearItems()
{
    // TODO: clear all sub lists.
}

