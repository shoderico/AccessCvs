#ifndef OBJECTITEMS_H
#define OBJECTITEMS_H

#include <QMap>
#include <QString>
#include "objectitem.h"

class ObjectItems : public QMap< Model::ObjectType, QMap< QString, ObjectItem* > >
{
public:
    ObjectItems();

    void clearItems();
};

#endif // OBJECTITEMS_H
