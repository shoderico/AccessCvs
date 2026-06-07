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
    ObjectItemMap(const QList<Model::ObjectType> &objectTypes);
};

#endif // OBJECTITEMMAP_H
