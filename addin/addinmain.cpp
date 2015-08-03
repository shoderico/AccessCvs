#include "addinmain.h"

#include "addinimpl.h"

AddInMain::AddInMain(ITypeInfo *pTypeInfo, QObject *parent)
    : QObject(parent)
    , m_pTypeInfo(pTypeInfo)
{
}

QAxAggregated *AddInMain::createAggregate()
{
    return new AddInImpl(m_pTypeInfo);
}
