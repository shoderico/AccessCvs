#ifndef CVSADDINFACTORY_H
#define CVSADDINFACTORY_H

#include "addin/addinfactory.h"

class CvsAddInFactory : public AddInFactory
{
    Q_OBJECT
public:
    CvsAddInFactory(const QUuid &app, const QUuid &lib);

    virtual QAxAggregated *createAggregate(QObject *parent = 0);
};

#endif // CVSADDINFACTORY_H
