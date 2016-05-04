#include "loaditemsfromsourcedircommand.h"

#include "util/progressnotifier.h"
#include "util/concurrentmaphelper.h"
#include "cvsmodel/projectsetting.h"
#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/objectitems.h"
#include "cvsmodel/cvsmodel_const.h"

//#include "cvsmodel/objectitem.h"
//#include "util/comptr.h"

#include <QtConcurrent>
//#include <QAxObject>

LoadItemsFromSourceDirCommand::LoadItemsFromSourceDirCommand(QAxObject *application, QList<ObjectItem *> *items, QObject *parent)
    : CommandBase(application, items, parent)
{

}

void LoadItemsFromSourceDirCommand::execute(ObjectItems *allTargets)
{
    // FIXME: non-blocking, can be async ? require append ?
    ProgressNotifier mainProgress(Model::LoadItemFromSourceDirProcess, this);
    ProjectSetting setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes() )
    {
        processor = setting[ objectType ];

        QDir objectDir( processor->sourceObjectPath() );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*." + processor->existCheckExtension() ) ) );
            QFileInfoList fileInfos = objectDir.entryInfoList( QDir::Files );
            ProgressNotifier subProgress(mainProgress.type(), fileInfos.length(), this);

            for (QFileInfoList::iterator it = fileInfos.begin(); it != fileInfos.end(); ++it )
            {
                subProgress.next();
                m_items->append( processor->createItemFromSourceDir( (*it), this) );
            }
        }
    }
}

