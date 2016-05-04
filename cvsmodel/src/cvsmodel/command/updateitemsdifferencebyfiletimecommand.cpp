#include "updateitemsdifferencebyfiletimecommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"

#include <QtConcurrent>

UpdateItemsDifferenceByFileTimeCommand::UpdateItemsDifferenceByFileTimeCommand(QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(application, items, parent)
{

}

struct UpdateItemsDifferenceByFileTimeFunctionObject : public FunctionObjectBase
{
    UpdateItemsDifferenceByFileTimeFunctionObject(DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        if ( !item->isModified() )
        {
            item->setDifferent( Model::SameContents );
            registerChanged( item );
        }
    }
};

void UpdateItemsDifferenceByFileTimeCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsDifferenceByFileTimeProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsDifferenceByFileTimeProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            if ( !(*it)->isModified() )
            {
                (*it)->setDifferent( Model::SameContents );
                helper.changed( m_items.indexOf( (*it) ) );
            }
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsDifferenceByFileTimeFunctionObject( &helper, m_items ) ) );
        }
        // */

    }
    if (helper.isChanged())
    {
        // TODO : how to emit model's signal ?
//        emit dataChanged( createIndex(helper.first(), Model::DifferentColumn), createIndex(helper.last(), Model::DifferentColumn) );
    }

}

