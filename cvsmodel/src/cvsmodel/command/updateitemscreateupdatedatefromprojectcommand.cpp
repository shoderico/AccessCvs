#include "updateitemscreateupdatedatefromprojectcommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/datachangedhelper.h"
#include "util/comptr.h"
#include <QAxObject>

#include <QtConcurrent>

UpdateItemsCreateUpdateDateFromProjectCommand::UpdateItemsCreateUpdateDateFromProjectCommand(QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(application, items, parent)
{

}

void UpdateItemsCreateUpdateDateFromProjectCommand::execute(ObjectItems *allTargets)
{
    // BLOCKING, cannot be asynch
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsCreateUpdateDateFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        processor = setting[ objectType ];
        if (!processor->prepareItemCollection())
            continue;

        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProgress.next();

            ComPtr<QAxObject> object = processor->itemUnsafePtr( (*it)->name() );
            ObjectItem *item = processor->createItemFromProject( object.ptr(), 0 );
            (*it)->setCreateDate( item->createDate() );
            (*it)->setUpdateDate( item->updateDate() );
            delete item;
            helper.changed( m_items->indexOf( (*it) ) );
        }
    }
    if (helper.isChanged())
    {
        // TODO : how to emit model's signal ?
//        emit dataChanged( createIndex(helper.first(), Model::CreateDateColumn), createIndex(helper.last(), Model::CreateDateColumn) );
//        emit dataChanged( createIndex(helper.first(), Model::UpdateDateColumn), createIndex(helper.last(), Model::UpdateDateColumn) );
    }
}

