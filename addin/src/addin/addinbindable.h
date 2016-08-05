#ifndef ADDINBINDABLE_H
#define ADDINBINDABLE_H

#include "addin_global.h"

#include <QObject>
#include <QAxBindable>

class AddInFactory;

class ADDIN_SHARED_EXPORT AddInBindable : public QObject, public QAxBindable
{
    Q_OBJECT
public:
    explicit AddInBindable(AddInFactory *factory, QObject *parent = 0);
    virtual ~AddInBindable();
    QAxAggregated *createAggregate();
private:
    AddInFactory *m_factory;
};

#endif // ADDINBINDABLE_H
