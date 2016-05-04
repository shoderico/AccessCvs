#include "updatefiletimeintempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"

#include <QtConcurrent>

UpdateFileTimeInTempDirCommand::UpdateFileTimeInTempDirCommand(const QDateTime &fileTime, const int differenceTypes, QAxObject *application, QObject *parent)
    : CommandBase(application, parent)
    , m_fileTime(fileTime)
    , m_differenceTypes(differenceTypes)
{

}

struct UpdateFileTimeInTempDirFunctionObject : public FunctionObjectBase
{
    UpdateFileTimeInTempDirFunctionObject(ObjectProcessor *processor, const QDateTime &fileTime, const Model::ObjectDifferenceTypes differenceTypes, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : FunctionObjectBase(dataChangedHelper, items)
        , m_processor(processor)
        , m_fileTime(fileTime)
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

        m_processor->updateFileTimeInTempDir( item->name(), m_fileTime );
        registerChanged( item );
    }

    ObjectProcessor *m_processor;
    QDateTime m_fileTime;
    Model::ObjectDifferenceTypes m_differenceTypes;
};

void UpdateFileTimeInTempDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProgress(Model::UpdateFileTimeInTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        processor = setting[ objectType ];
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateFileTimeInTempDirProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            switch( (*it)->isDifferent() )
            {
                case Model::SameContents:       if (!(differenceTypes & SameContentsType      )) continue; break;
                case Model::DifferentContents:  if (!(differenceTypes & DifferentContentsTypes)) continue; break;
                default: break;
            }

            os->updateFileTimeInTempDir( (*it)->name(), fileTime );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(items, UpdateFileTimeInTempDirFunctionObject(processor, m_fileTime, (Model::ObjectDifferenceTypes)m_differenceTypes, NULL, NULL) ) );
        }
        // */

    }
}

