#include "objectitemmap.h"

#include "pch.hpp"

ObjectItemMap::ObjectItemMap(const QList<Model::ObjectType> &objectTypes)
{
    // add Object-Level object types ( not Project-Level )
    foreach ( Model::ObjectType objectType, objectTypes )
        insert( objectType, QMap< QString, ObjectItem* >() );
}

