#include "copyfromtempdirtosourcedircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

CopyFromTempDirToSourceDirCommand::CopyFromTempDirToSourceDirCommand(QObject *parent)
    : CommandBase(parent)
{
}

struct CopyFromTempDirToSourceDirFunctionObject
{
    CopyFromTempDirToSourceDirFunctionObject(ObjectProcessor *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->copyFromTempDirToSourceDir(objectName);
    }

    ObjectProcessor *m_os;
};

void CopyFromTempDirToSourceDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(Model::CopyFromTempDirToSourceDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];
        os->mkdirSourceObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(CopyFromTempDirToSourceDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->copyFromTempDirToSourceDir( (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, CopyFromTempDirToSourceDirFunctionObject(os) ) );
        }
        // */
    }
}

