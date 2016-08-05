#include "addinbindable.h"

#include "addinfactory.h"

#include <QDebug>

#include "pch.hpp"

AddInBindable::AddInBindable(AddInFactory *factory, QObject *parent)
    : QObject(parent)
    , m_factory(factory)
{
}

AddInBindable::~AddInBindable()
{
    qDebug() << "destructor";
}

QAxAggregated *AddInBindable::createAggregate()
{
    return m_factory->createAggregate(this);
}
