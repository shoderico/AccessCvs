#ifndef ACCESSOBJECTPROXYMODEL_H
#define ACCESSOBJECTPROXYMODEL_H

#include "objectproxymodel.h"

class CVSMODEL_SHARED_EXPORT AccessObjectProxyModel : public ObjectProxyModel
{
    Q_OBJECT
public:
    explicit AccessObjectProxyModel(QObject *parent = 0);

signals:

public slots:
};

#endif // ACCESSOBJECTPROXYMODEL_H
