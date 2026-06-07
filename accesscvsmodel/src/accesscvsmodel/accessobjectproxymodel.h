#ifndef ACCESSOBJECTPROXYMODEL_H
#define ACCESSOBJECTPROXYMODEL_H

#include "accesscvsmodel_global.h"
#include "cvsmodel/objectproxymodel.h"

class ACCESSCVSMODEL_SHARED_EXPORT AccessObjectProxyModel : public ObjectProxyModel
{
    Q_OBJECT
public:
    explicit AccessObjectProxyModel(QObject *parent = 0);

signals:

public slots:
};

#endif // ACCESSOBJECTPROXYMODEL_H
