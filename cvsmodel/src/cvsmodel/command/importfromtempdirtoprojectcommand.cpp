#include "importfromtempdirtoprojectcommand.h"

#include "util/progressnotifier.h"
#include "util/comptr.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/objectitem.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QAxObject>

#include "pch.hpp"

ImportFromTempDirToProjectCommand::ImportFromTempDirToProjectCommand(ProjectContainer *project, QAxObject *application, QObject *parent)
    : CommandBase(project, application, parent)
{
}

void ImportFromTempDirToProjectCommand::execute(ObjectItemMap *allTargets)
{
    // BLOCKING, cannot be async

    ProgressNotifier mainProgress(Model::ImportFromTempDirToProjectProcess, this);
    ObjectProcessor *processor;

    foreach (const Model::ObjectType &objectType, m_project->objectTypes())
    {
        processor = m_project->operator []( objectType );

        if (!processor->prepareItemCollection())
            continue;

        QMap<QString, ObjectItem*> targets = allTargets->value( processor->objectType() );
        QStringList objectNames = targets.keys();

        // main process
        {
            ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);

            for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
            {
                subProgress.next();
                ObjectItem *item = targets[ (*it) ];
                if (item->inProject() == Model::Present)
                {
                    ComPtr<QAxObject> object = processor->itemUnsafePtr( (*it) );
                    processor->importFromTempDirToProject(object.ptr(), (*it));
                }
                else
                    processor->importFromTempDirToProject( NULL, (*it) );
            }
        }
        // post process
        {
            ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);

            for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
            {
                subProgress.next();
                ObjectItem *item = targets[ (*it) ];
                if (item->inProject() == Model::Present)
                {
                    ComPtr<QAxObject> object = processor->itemUnsafePtr( (*it) );
                    processor->afterImportFromTempDirToProject(object.ptr(), (*it));
                }
                else
                    processor->afterImportFromTempDirToProject( NULL, (*it) );
            }
        }
    }
}

