#include "updateitemsexportdatecommand.h"

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

UpdateItemsExportDateCommand::UpdateItemsExportDateCommand(const QDateTime &exportDate, const int differenceTypes, ProjectContainer *project, QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(project, application, items, parent)
    , m_exportDate(exportDate)
    , m_differenceTypes(differenceTypes)
{

}

struct UpdateItemsExportDateFunctionObject : public FunctionObjectBase
{
    UpdateItemsExportDateFunctionObject(QDateTime exportDate, Model::ObjectDifferenceTypes differenceTypes, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_exportDate(exportDate)
        , m_differenceTypes(differenceTypes)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        switch( item->isDifferent() )
        {
            case Model::SameContents:       if (!(m_differenceTypes & Model::SameContentsType      )) return;
            case Model::DifferentContents:  if (!(m_differenceTypes & Model::DifferentContentsTypes)) return;
            default: break;
        }

        item->setExportDate(m_exportDate);
        registerChanged( item );
    }

    QDateTime m_exportDate;
    Model::ObjectDifferenceTypes m_differenceTypes;
};

void UpdateItemsExportDateCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateItemsExportDateProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsExportDateProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            switch( (*it)->isDifferent() )
            {
                case Model::SameContents:       if (!(differenceTypes & SameContentsType      )) continue; break;
                case Model::DifferentContents:  if (!(differenceTypes & DifferentContentsTypes)) continue; break;
                default: break;
            }

            (*it)->setExportDate(exportDate);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        // */


        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsExportDateFunctionObject(m_exportDate, (Model::ObjectDifferenceTypes)m_differenceTypes, &helper, m_items) ) );
        }
        // */
    }
    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::ExportDateColumn, Model::ExportDateColumn);
    }

}

void UpdateItemsExportDateCommand::setExportDate(const QDateTime &exportDate)
{
    m_exportDate = exportDate;
}

void UpdateItemsExportDateCommand::setDifferenceTypes(const int differenceTypes)
{
    m_differenceTypes = differenceTypes;
}

