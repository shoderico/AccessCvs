#include "commandbase.h"

#include "util/datachangedhelper.h"

CommandBase::CommandBase(QObject *parent) : QObject(parent)
{

}

void CommandBase::setApplication(QAxObject *application)
{
    m_application = application;
}

void CommandBase::setItems(QList<ObjectItem *> *items)
{
    m_items = items;
}

void CommandBase::execute(ObjectItems *allTargets)
{
    Q_UNUSED(allTargets)
}


FunctionObjectBase::FunctionObjectBase(DataChangedHelper *dataChangedHelper, QList<ObjectItem *> *items)
    : m_dataChangedHelper(dataChangedHelper)
    , m_items(items)
{
}

void FunctionObjectBase::registerChanged(ObjectItem *item)
{
    if (m_dataChangedHelper && m_items)
        m_dataChangedHelper->changed( m_items->indexOf(item) );
}
