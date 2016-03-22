#include "deletefromsourcedircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QtConcurrent>

DeleteFromSourceDirCommand::DeleteFromSourceDirCommand(QObject *parent)
    : CommandBase(parent)
{
}

struct DeleteFromSourceDirFunctionObject
{
    DeleteFromSourceDirFunctionObject(ObjectProcessor *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->deleteFromSourceDir(objectName);
    }

    ObjectProcessor *m_os;
};

void DeleteFromSourceDirCommand::execute(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(Model::DeleteFromSourceDirProcess, this);
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
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, DeleteFromSourceDirFunctionObject(os) ) );
        }
        // */
    }
}

