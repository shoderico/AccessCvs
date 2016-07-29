#include "addinbindable.h"

#include "addinfactory.h"

#include "pch.hpp"

AddInBindable::AddInBindable(AddInFactory *factory, QObject *parent)
    : QObject(parent)
    , m_factory(factory)
{
}

QAxAggregated *AddInBindable::createAggregate()
{
    return m_factory->createAggregate(this);
}
