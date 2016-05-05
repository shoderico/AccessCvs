#include "comparetempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"

#include <QtConcurrent>

CompareTempDirCommand::CompareTempDirCommand(QAxObject *application, QList<ObjectItem*> *items, QObject *parent)
    : CommandBase(application, items, parent)
{
}

struct CompareTempDirFunctionObject : public FunctionObjectBase
{
    CompareTempDirFunctionObject(ObjectProcessor *processor, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_processor(processor)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        bool isDifferent = false;
        m_processor->compareTempDir( item->name(), item, &isDifferent);

        if (isDifferent == true && item->isDifferent() != Model::DifferentContents )
        {
            item->setDifferent( Model::DifferentContents );
            registerChanged( item );
        }
        else if (isDifferent == false && item->isDifferent() != Model::SameContents )
        {
            item->setDifferent( Model::SameContents );
            registerChanged( item );
        }
    }

    ObjectProcessor *m_processor;
};


void CompareTempDirCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::CompareTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        processor = setting[ objectType ];


        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(CompareTempDirProcess, objectNames.count(), this);
        bool isDifferent = false;
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            isDifferent = false;
            os->compareTempDir( (*it) , &isDifferent);

            ObjectItem *item = targets[ (*it) ];
            if (isDifferent == true && item->isDifferent() != Model::DifferentContents )
            {
                item->setDifferent( Model::DifferentContents );
            }
            else if (isDifferent == false && item->isDifferent() != Model::SameContents )
            {
                item->setDifferent( Model::SameContents );
            }
            helper.changed( m_items.indexOf( item ) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        //
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, CompareTempDirFunctionObject(processor, &helper, m_items) ) );
        }
        // */
    }

    // update items
    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::DifferentColumn, Model::DifferentColumn);
    }
}

