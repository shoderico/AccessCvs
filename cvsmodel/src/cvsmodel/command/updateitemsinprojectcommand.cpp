#include "updateitemsinprojectcommand.h"

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

UpdateItemsInProjectCommand::UpdateItemsInProjectCommand(const int existence, ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent)
    : CommandBase(project, application, items, parent)
    , m_existence(existence)
{

}

struct UpdateItemsInProjectFunctionObject : public FunctionObjectBase
{
    UpdateItemsInProjectFunctionObject(Model::ObjectExistence existence, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_existence(existence)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        item->setInProject(m_existence);
        registerChanged( item );
    }

    Model::ObjectExistence m_existence;
};

void UpdateItemsInProjectCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsInProjectProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        emit currentObjectTypeChanged(objectType);

        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsInProjectProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            (*it)->setInProject(existence);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsInProjectFunctionObject((Model::ObjectExistence)m_existence, &helper, m_items) ) );
        }
        // */
    }
    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::InProjectColumn, Model::InProjectColumn);
    }

}

