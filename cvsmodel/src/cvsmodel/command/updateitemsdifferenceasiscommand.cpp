#include "updateitemsdifferenceasiscommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"

#include <QtConcurrent>

UpdateItemsDifferenceAsIsCommand::UpdateItemsDifferenceAsIsCommand(ProjectContainer *project, QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(project, application, items, parent)
{

}

struct UpdateItemsDifferenceAsIsFunctionObject : public FunctionObjectBase
{
    UpdateItemsDifferenceAsIsFunctionObject(ObjectProcessor *processor, Model::ObjectDifference difference, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_processor(processor)
        , m_difference(difference)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        if ( item->isModified() )
            return;

        bool isDifferent;
        m_processor->compareTempDir( item->name(), item, &isDifferent );
        if ( isDifferent )
            return;

        item->setDifferent(m_difference);
        registerChanged( item );
    }

    ObjectProcessor *m_processor;
    Model::ObjectDifference m_difference;
};

void UpdateItemsDifferenceAsIsCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsDifferenceAsIsProcess, this);
    ObjectProcessor *processor;

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        processor = m_project->operator []( objectType );

        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsDifferenceAsIsProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();

            if ( (*it)->isModified() )
                continue;

            bool isDifferent;
            os->compareTempDir( (*it)->name(), &isDifferent );
            if ( !isDifferent )
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
            mapHelper.run( QtConcurrent::map(items, UpdateItemsDifferenceAsIsFunctionObject(processor, Model::SameContents, NULL, NULL) ) );
        }
        // */

    }
    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::DifferentColumn, Model::DifferentColumn);
    }
}

