#ifndef OBJECTITEMS_H
#define OBJECTITEMS_H

#include "cvsmodel_global.h"
#include "cvsmodel_const.h"

#include <QMap>
#include <QString>

class ObjectItem;

class CVSMODEL_SHARED_EXPORT ObjectItems : public QMap< Model::ObjectType, QMap< QString, ObjectItem* > >
{
public:
    ObjectItems();

    void clearItems();
};

#endif // OBJECTITEMS_H
