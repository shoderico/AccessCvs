#include "deletefromprojectcommand.h"

#include "util/progressnotifier.h"
//#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

DeleteFromProjectCommand::DeleteFromProjectCommand(QObject *parent)
    : CommandBase(parent)
{
}

void DeleteFromProjectCommand::execute(ObjectItems *allTargets)
{
    // BLOCKING, cannot be async

    ProgressNotifier mainProg(Model::DeleteFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromProject( (*it) );
        }
    }
}

