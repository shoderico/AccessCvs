#include "addinmain.h"

#include "addinimpl.h"

AddInMain::AddInMain(QObject *parent)
    : QObject(parent)
{
}

QAxAggregated *AddInMain::createAggregate()
{
    return new AddInImpl();
}
