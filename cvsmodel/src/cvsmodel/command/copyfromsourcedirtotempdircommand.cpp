#include "copyfromsourcedirtotempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

CopyFromSourceDirToTempDirCommand::CopyFromSourceDirToTempDirCommand(QObject *parent)
    : CommandBase(parent)
{
}

struct CopyFromSourceDirToTempDirFunctionObject
{
    CopyFromSourceDirToTempDirFunctionObject(ObjectProcessor *processor)
        : m_processor(processor)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_processor->copyFromSourceDirToTempDir(objectName);
    }

    ObjectProcessor *m_processor;
};

void CopyFromSourceDirToTempDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProgress(Model::CopyFromSourceDirToTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        processor = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( processor->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(CopyFromSourceDirToTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->copyFromSourceDirToTempDir( (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(objectNames, CopyFromSourceDirToTempDirFunctionObject(processor) ) );
        }
        // */
    }
}

