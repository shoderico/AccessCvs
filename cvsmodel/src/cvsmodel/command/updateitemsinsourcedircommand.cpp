#include "updateitemsinsourcedircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"

#include <QtConcurrent>

UpdateItemsInSourceDirCommand::UpdateItemsInSourceDirCommand(const int existence, QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(application, items, parent)
    , m_existence(existence)
{

}

struct UpdateItemsInSourceDirFunctionObject : public FunctionObjectBase
{
    UpdateItemsInSourceDirFunctionObject(Model::ObjectExistence existence, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_existence(existence)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        item->setInSourceDir(m_existence);
        registerChanged( item );
    }

    Model::ObjectExistence m_existence;
};

void UpdateItemsInSourceDirCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsInSourceDirProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsInSourceDirProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            (*it)->setInSourceDir(existence);
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
            mapHelper.run( QtConcurrent::map(items, UpdateItemsInSourceDirFunctionObject((Model::ObjectExistence)m_existence, &helper, m_items) ) );
        }
        // */
    }
    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::InSourceDirColumn, Model::InSourceDirColumn);
    }

}


