#include "importfromtempdirtoprojectcommand.h"

#include "util/progressnotifier.h"
#include "util/comptr.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/objectitem.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QAxObject>

ImportFromTempDirToProjectCommand::ImportFromTempDirToProjectCommand(QObject *parent)
    : CommandBase(parent)
{
}

void ImportFromTempDirToProjectCommand::execute(ObjectItems *allTargets)
{
    // BLOCKING, cannot be async

    ProgressNotifier mainProg(Model::ImportFromTempDirToProjectProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        if (!os->prepareItemCollection())
            continue;

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        // main process
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);

            for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
            {
                subProg.next();
                ObjectItem *item = targets[ (*it) ];
                if (item->inProject() == Model::Present)
                {
                    ComPtr<QAxObject> object = os->itemUnsafePtr( (*it) );
                    os->importFromTempDirToProject(object.ptr(), (*it));
                }
                else
                    os->importFromTempDirToProject( NULL, (*it) );
            }
        }
        // post process
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);

            for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
            {
                subProg.next();
                ObjectItem *item = targets[ (*it) ];
                if (item->inProject() == Model::Present)
                {
                    ComPtr<QAxObject> object = os->itemUnsafePtr( (*it) );
                    os->afterImportFromTempDirToProject(object.ptr(), (*it));
                }
                else
                    os->afterImportFromTempDirToProject( NULL, (*it) );
            }
        }
    }
}

