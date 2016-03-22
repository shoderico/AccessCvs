#include "deletefromtempdircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

DeleteFromTempDirCommand::DeleteFromTempDirCommand(QObject *parent)
    : CommandBase(parent)
{
}

struct DeleteFromTempDirFunctionObject
{
    DeleteFromTempDirFunctionObject(ObjectProcessor *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->deleteFromTempDir(objectName);
    }

    ObjectProcessor *m_os;
};

void DeleteFromTempDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(Model::DeleteFromTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // syncronous call
        /*
        ProgressNotifier subProg(DeleteFromTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromTempDir( (*it) );
        }
        // */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, DeleteFromTempDirFunctionObject(os) ) );
        }
        // */
    }
}

