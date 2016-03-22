#include "comparetempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"

#include <QtConcurrent>

CompareTempDirCommand::CompareTempDirCommand(QObject *parent)
    : CommandBase(parent)
{
}

struct CompareTempDirFunctionObject : public FunctionObjectBase
{
    CompareTempDirFunctionObject(ObjectProcessor *os, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_os(os)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        bool isDifferent = false;
        m_os->compareTempDir( item->name() , &isDifferent);

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

    ObjectProcessor *m_os;
};


void CompareTempDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProg(Model::CompareTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];


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
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, CompareTempDirFunctionObject(os, &helper, m_items) ) );
        }
        // */
    }

    // update items
    if (helper.isChanged())
    {
        // TODO : how to emit model's signal ?
        //emit dataChanged( createIndex(helper.first(), DifferentColumn), createIndex( helper.last(), DifferentColumn ) );
    }
}

