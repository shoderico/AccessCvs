#include "updatefiletimeintempdirbyexportdatecommand.h"

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

UpdateFileTimeInTempDirByExportDateCommand::UpdateFileTimeInTempDirByExportDateCommand(const int differenceTypes, ProjectContainer *project, QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(project, application, items, parent)
    , m_differenceTypes(differenceTypes)
{

}

struct UpdateFileTimeInTempDirByExportDateFunctionObject : public FunctionObjectBase
{
    UpdateFileTimeInTempDirByExportDateFunctionObject(ObjectProcessor *processor, const Model::ObjectDifferenceTypes differenceTypes, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_processor(processor)
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

        if ( item->exportDate().isValid() )
            m_processor->updateFileTimeInTempDir( item->name(), item->exportDate() );
        else
        {
            // treat as 1 sec. old from udpateDate
            QDateTime exportDate = item->updateDate().addSecs(-1);
            m_processor->updateFileTimeInTempDir( item->name(), exportDate );
            item->setExportDate( exportDate );
            registerChanged( item );
        }
    }

    ObjectProcessor *m_processor;
    Model::ObjectDifferenceTypes m_differenceTypes;
};

void UpdateFileTimeInTempDirByExportDateCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items->count() );
    ProgressNotifier mainProgress(Model::UpdateFileTimeInTempDirByExportDateProcess, this);
    ObjectProcessor *processor;

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        processor = m_project->operator []( objectType );
        QList<ObjectItem*> items = allTargets->value( objectType ).values();


        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateFileTimeInTempDirByExportDateProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            switch( (*it)->isDifferent() )
            {
                case Model::SameContents:       if (!(differenceTypes & SameContentsType      )) continue; break;
                case Model::DifferentContents:  if (!(differenceTypes & DifferentContentsTypes)) continue; break;
                default: break;
            }

            if ( (*it)->exportDate().isValid() )
                os->updateFileTimeInTempDir( (*it)->name(), (*it)->exportDate() );
            else
            {
                // treat as 1 sec. old from udpateDate
                QDateTime exportDate = (*it)->updateDate().addSecs(-1);
                os->updateFileTimeInTempDir( (*it)->name(), exportDate );
                (*it)->setExportDate( exportDate );

                int row = m_items.indexOf( (*it) );
                emit dataChanged( createIndex(row, ExportDateColumn), createIndex(row, ExportDateColumn) );

            }
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, UpdateFileTimeInTempDirByExportDateFunctionObject(processor, (Model::ObjectDifferenceTypes)m_differenceTypes, &helper, m_items) ) );
        }
        // */
    }

    if (helper.isChanged())
    {
        emit dataChanged(helper.first(), helper.last(), Model::ExportDateColumn, Model::ExportDateColumn);
    }

}

void UpdateFileTimeInTempDirByExportDateCommand::setDifferenceTypes(const int differenceTypes)
{
    m_differenceTypes = differenceTypes;
}

