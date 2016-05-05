#include "loaditemsfromprojectcommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitemmap.h"
#include "cvsmodel/cvsmodel_const.h"

#include "cvsmodel/objectitem.h"
#include "util/comptr.h"

#include <QtConcurrent>
#include <QAxObject>

LoadItemsFromProjectCommand::LoadItemsFromProjectCommand(ProjectContainer *project, QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(project, application, items, parent)
{

}

void LoadItemsFromProjectCommand::execute(ObjectItemMap *allTargets)
{
    // BLOCKING, cannot be async
    ProgressNotifier mainProgress(Model::LoadItemFromProjectProcess, this);
    ObjectProcessor *processor;

    foreach (const Model::ObjectType &objectType, m_project->objectTypes() )
    {
        processor = m_project->operator []( objectType );

        if ( !processor->prepareItemCollection() )
            continue;

        ProgressNotifier subProgress(mainProgress.type(), processor->itemCount(), this);
        for ( int i = 0 ; i < processor->itemCount() ; ++i )
        {
            subProgress.next();
            ComPtr<QAxObject> object = processor->itemUnsafePtr(i);
            if ( processor->isTargetObject( object.ptr() ) )
            {
                m_items->append( processor->createItemFromProject(object.ptr(), this) );
            }
        }
    }
}

