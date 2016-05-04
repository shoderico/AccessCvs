#ifndef OBJECTITEMMAP_H
#define OBJECTITEMMAP_H

#include "cvsmodel_global.h"
#include "cvsmodel_const.h"

#include <QMap>
#include <QString>

class ObjectItem;

class CVSMODEL_SHARED_EXPORT ObjectItemMap : public QMap< Model::ObjectType, QMap< QString, ObjectItem* > >
{
public:
    ObjectItemMap();

    void clearItems();
};

#endif // OBJECTITEMMAP_H
