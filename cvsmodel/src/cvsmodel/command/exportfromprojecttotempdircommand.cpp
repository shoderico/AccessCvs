#include "exportfromprojecttotempdircommand.h"

#include "util/progressnotifier.h"
#include "util/comptr.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/objectitem.h"
#include "cvsmodel/cvsmodel_const.h"

#include <QAxObject>

ExportFromProjectToTempDirCommand::ExportFromProjectToTempDirCommand(QObject *parent)
    : CommandBase(parent)
{
}

void ExportFromProjectToTempDirCommand::execute(ObjectItems *allTargets)
{
    // BLOCKING, cannot be async

    // export to temp directory
    //      for objects existing in both Project and SourceDir
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

    ProgressNotifier mainProg(Model::ExportFromProjectToTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *os;
    setting.initialize(m_application);

    foreach ( const Model::ObjectType &objectType, setting.objectTypes() )
    {
        os = setting[ objectType ];
        os->mkdirTempObjectPath();

        if (!os->prepareItemCollection())
            continue;

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            ComPtr<QAxObject> object = os->itemUnsafePtr( (*it) );
            os->exportFromProjectToTempDir(object.ptr(), (*it) );
        }
    }
}

