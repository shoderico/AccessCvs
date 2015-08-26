#include "addinmain.h"

#include "addinfactory.h"

AddInMain::AddInMain(AddInFactory *factory, QObject *parent)
    : QObject(parent)
    , m_factory(factory)
{
}

QAxAggregated *AddInMain::createAggregate()
{
    return m_factory->createAggregate(this);
}
