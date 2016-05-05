#include "deletefromsourcedircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

DeleteFromSourceDirCommand::DeleteFromSourceDirCommand(ProjectContainer *project, QAxObject *application, QObject *parent)
    : CommandBase(project, application, parent)
{
}

struct DeleteFromSourceDirFunctionObject
{
    DeleteFromSourceDirFunctionObject(ObjectProcessor *processor)
        : m_processor(processor)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_processor->deleteFromSourceDir(objectName);
    }

    ObjectProcessor *m_processor;
};

void DeleteFromSourceDirCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    ProgressNotifier mainProgress(Model::DeleteFromSourceDirProcess, this);
    ObjectProcessor *processor;

    foreach (const Model::ObjectType &objectType, m_project->objectTypes())
    {
        processor = m_project->operator []( objectType );

        QMap<QString, ObjectItem*> targets = allTargets->value( processor->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // syncronous call
        /*
        ProgressNotifier subProg(DeleteFromSourceDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromSourceDir( (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(objectNames, DeleteFromSourceDirFunctionObject(processor) ) );
        }
        // */
    }
}

