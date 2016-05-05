#include "exportfromprojecttotempdircommand.h"

#include "util/progressnotifier.h"
#include "util/comptr.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/objectitem.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QAxObject>

ExportFromProjectToTempDirCommand::ExportFromProjectToTempDirCommand(QAxObject *application, QObject *parent)
    : CommandBase(application, parent)
{
}

void ExportFromProjectToTempDirCommand::execute(ObjectItemMap *allTargets)
{
    // BLOCKING, cannot be async

    // export to temp directory
    //      for objects existing in both Project and SourceDir
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

    ProgressNotifier mainProgress(Model::ExportFromProjectToTempDirProcess, this);
    ProjectContainer setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach ( const Model::ObjectType &objectType, setting.objectTypes() )
    {
        processor = setting[ objectType ];
        processor->mkdirTempObjectPath();

        if (!processor->prepareItemCollection())
            continue;

        QMap<QString, ObjectItem*> targets = allTargets->value( processor->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProgress(mainProgress.type(), objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProgress.next();
            ComPtr<QAxObject> object = processor->itemUnsafePtr( (*it) );
            processor->exportFromProjectToTempDir(object.ptr(), (*it) );
        }
    }
}

