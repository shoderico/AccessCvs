#include "commandbase.h"

#include "util/datachangedhelper.h"

CommandBase::CommandBase(QObject *parent) : QObject(parent)
  , m_application(0)
  , m_items(0)
{
    assign();
}

CommandBase::CommandBase(QAxObject *application, QObject *parent) : QObject(parent)
  , m_application(application)
  , m_items(0)
{
    assign();
}

CommandBase::CommandBase(QAxObject *application, QList<ObjectItem *> *items, QObject *parent) : QObject(parent)
  , m_application(application)
  , m_items(items)
{
    assign();
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

void CommandBase::assign()
{
    connect(this, SIGNAL(progressStart(int,int)),   parent(), SIGNAL(progressStart(int,int)) );
    connect(this, SIGNAL(progressChange(int,int)),  parent(), SIGNAL(progressChange(int,int)) );
    connect(this, SIGNAL(progressEnd(int)),         parent(), SIGNAL(progressEnd(int)) );

    connect(this, SIGNAL(dataChanged(int,int,int,int)),         parent(), SLOT(itemDataChanged(int,int,int,int)) );
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
