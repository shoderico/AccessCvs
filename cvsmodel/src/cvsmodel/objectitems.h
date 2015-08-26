#ifndef OBJECTITEMS_H
#define OBJECTITEMS_H

#include "cvsmodel_global.h"

#include <QMap>
#include <QString>
#include "objectitem.h"

class CVSMODELSHARED_EXPORT ObjectItems : public QMap< Model::ObjectType, QMap< QString, ObjectItem* > >
{
public:
    ObjectItems();

    void clearItems();
};

#endif // OBJECTITEMS_H
