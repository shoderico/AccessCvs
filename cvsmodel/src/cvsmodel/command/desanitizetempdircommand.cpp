#include "desanitizetempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

DesanitizeTempDirCommand::DesanitizeTempDirCommand(QAxObject *application, QObject *parent)
    : CommandBase(application, parent)
{

}


struct DesanitizeTempDirFunctionObject
{
    DesanitizeTempDirFunctionObject(ObjectProcessor *processor)
        : m_processor(processor)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_processor->desanitizeTempDir(NULL, objectName);
    }

    ObjectProcessor *m_processor;
};


void DesanitizeTempDirCommand::execute(ObjectItemMap *allTargets)
{
    // non-blocking
    ProgressNotifier mainProgress(Model::DesanitizeTempDirProcess, this);
    ProjectContainer setting(this);
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
        ProgressNotifier subProg(DesanitizeTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->desanitizeTempDir(NULL, (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProgress );
            mapHelper.run( QtConcurrent::map(objectNames, DesanitizeTempDirFunctionObject(processor) ) );
        }
        // */
    }
}

