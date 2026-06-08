#include "updateitemsdifferencecommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"

#include <QtConcurrent>

#include "pch.hpp"

UpdateItemsDifferenceCommand::UpdateItemsDifferenceCommand(const int difference, ProjectContainer *project, QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(project, application, items, parent)
    , m_difference(difference)
{

}

struct UpdateItemsDifferenceFunctionObject : public FunctionObjectBase
{
    UpdateItemsDifferenceFunctionObject(Model::ObjectDifference difference, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_difference(difference)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        item->setDifferent(m_difference);
        registerChanged( item );
    }

    Model::ObjectDifference m_difference;
};

void UpdateItemsDifferenceCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsDifferenceProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        emit currentObjectTypeChanged(objectType);

        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsDifferenceProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            (*it)->setDifferent(difference);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        // */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent map
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsDifferenceFunctionObject((Model::ObjectDifference)m_difference, &helper, m_items) ) );
        }
        // */

    }
    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::DifferentColumn, Model::DifferentColumn);
    }
}

