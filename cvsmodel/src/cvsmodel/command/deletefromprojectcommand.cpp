#include "deletefromprojectcommand.h"

#include "util/progressnotifier.h"
//#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

DeleteFromProjectCommand::DeleteFromProjectCommand(QAxObject *application, QObject *parent)
    : CommandBase(application, parent)
{
}

void DeleteFromProjectCommand::execute(ObjectItemMap *allTargets)
{
    // BLOCKING, cannot be async

    ProgressNotifier mainProgress(Model::DeleteFromProjectProcess, this);
    ProjectContainer setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        processor = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( processor->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProgress.next();
            processor->deleteFromProject( (*it) );
        }
    }
}

