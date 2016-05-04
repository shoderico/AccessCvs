#include "sanitizetempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

SanitizeTempDirCommand::SanitizeTempDirCommand(QObject *parent)
    : CommandBase(parent)
{

}


struct SanitizeTempDirFunctionObject
{
    SanitizeTempDirFunctionObject(ObjectProcessor *processor)
        : m_processor(processor)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_processor->sanitizeTempDir(NULL, objectName);
    }

    ObjectProcessor *m_processor;
};


void SanitizeTempDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProgress(Model::SanitizeTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        processor = setting[ objectType ];
        processor->determineCodecForProject();

        QMap<QString, ObjectItem*> targets = allTargets->value( processor->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(SanitizeTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->sanitizeTempDir(NULL, (*it) );
        }
        // */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(objectNames, SanitizeTempDirFunctionObject(processor) ) );
        }
        // */
    }
}

