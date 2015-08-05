#include "objectmodel.h"

#include <QIcon>
#include <QDir>

#include <QApplication> // requred for QApplication::processEvents();
#include <QDebug>

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/progressnotifier.h"

#include "projectsetting.h"
#include "objectsetting.h"


ObjectModel::ObjectModel(QObject * parent)
    : QAbstractItemModel(parent)
    , m_application(0)
{
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    // name,objectType
    // inProject,inRepository
    // createDate,updateDate
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

int ObjectModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.count();
}

QVariant ObjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();
    if ( orientation == Qt::Horizontal )
    {
        switch (section)
        {
            case NameColumn: return tr("Name");
            case InProjectColumn: return tr("InProject");
            case InFileSystemColumn: return tr("InRepository");
            case CreateDateColumn: return tr("CreateDate");
            case UpdateDateColumn: return tr("UpdateDate");
            case ObjectTypeColumn: return tr("ObjectType");
            case DifferentColumn: return tr("Different");
        }
    }

    return section + 1;
}

Qt::ItemFlags ObjectModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid())
    {
        flags |= Qt::ItemIsSelectable
                | Qt::ItemIsEnabled
                | Qt::ItemIsUserCheckable
                ;

    }
    return flags;
}

QVariant ObjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() < 0 || index.row() >= m_items.count() ||
            index.column() < 0 || index.column() >= ColumnCount)
        return QVariant();

    const ObjectItem *item = m_items.at(index.row());

    if (role == Qt::SizeHintRole)
    {
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column()) {
            case NameColumn:          return item->name();
            case InProjectColumn:     return (int)item->inProject();
            case InFileSystemColumn:  return (int)item->inFileSystem();
            case CreateDateColumn:    return item->createDate();
            case UpdateDateColumn:    return item->updateDate();
            case ObjectTypeColumn:    return item->objectType();
            case DifferentColumn:    return item->isDifferent();
        default:
            break;
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case NameColumn:  return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
        default: return static_cast<int>(Qt::AlignVCenter | Qt::AlignCenter);
        }
    }

    if (role == Qt::DecorationRole && index.column() == NameColumn)
    {
        // return QIcon

        // TODO: make icon manager
        switch (item->objectType()) {
            case Model::TableDef:   return QIcon( ":ui/images/table.png" );
            case Model::Query:      return QIcon( ":ui/images/table_multiple.png" );
            case Model::Form:       return QIcon( ":ui/images/application_form.png" );
            case Model::Report:     return QIcon( ":ui/images/report.png" );
            case Model::Macro:      return QIcon( ":ui/images/script.png" );
            case Model::Module:     return QIcon( ":ui/images/page.png" );

        default:
            break;
        }
        return QVariant();
    }

    if (role == Qt::CheckStateRole)
    {
        switch (index.column())
        {
        case NameColumn: return static_cast<int>(item->isSelected() ? Qt::Checked : Qt::Unchecked);
        //case InProject: return static_cast<int>(item->inProject() ? Qt::Checked : Qt::Unchecked);
        //case InRepository: return static_cast<int>(item->inRepository() ? Qt::Checked : Qt::Unchecked);
        }
    }

    return QVariant();
}

QModelIndex ObjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= ColumnCount || row >= m_items.count() ||
            parent.isValid())
        return QModelIndex();

    ObjectItem *item = m_items.at(row);
    return createIndex(row, column, item);
}

QModelIndex ObjectModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

bool ObjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.column() != NameColumn || index.row() < 0 || index.row() >= m_items.count())
        return false;
    ObjectItem *item = m_items.at(index.row());
    if (role == Qt::CheckStateRole)
    {
        item->setSelected( value.toBool() );
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void ObjectModel::setApplication(Access::Application *application)
{
    m_application = application;
}

void ObjectModel::prepareInit()
{
    //-------------------------------------------------------------------------------------------
    // `git init <directory>`

    //-------------------------------------------------------------------------------------------
    // ... prepareCommit()
    // ... executeCommit()

    //-------------------------------------------------------------------------------------------
    // ... preparePush()
    // ... executePush()
}

void ObjectModel::prepareClone()
{
    //-------------------------------------------------------------------------------------------
    // `git clone <repo-url> <directory>`

    //-------------------------------------------------------------------------------------------
    // ... openProject()    : done by user
    // ... createProject()  : done by user

    //-------------------------------------------------------------------------------------------
    // ... prepareMerge()
    // ... executeMerge()
}

void ObjectModel::prepareCommit()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:prepare to determine target objects to reflect

    // access save & compile
    // `git checkout <branch>`              // make sure we are in proper branch


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to reflect from Project to FileSytem
    refreshItems();

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:confirm to reflect from Project to FileSystem


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to FileSystem
    executeExport();


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //: and now, we are in git problem
    //

    //-------------------------------------------------------------------------------------------
    // `git status --porcelain`             // ... and show status of objects
    //      MADRCU?!

    //-------------------------------------------------------------------------------------------
    // `git diff`                           // ... and show diffs in external gui

    //-------------------------------------------------------------------------------------------
    //                                      // ... and select target objects ( extensions )
    // for unstaged files :
    //      `git add <file>`         : will be commit
    //      `git checkout -- <file>` : discard change in working directory
    // for staged files :
    //      `git reset HEAD <file>`  : unstage

    //-------------------------------------------------------------------------------------------
    //                                      // ... and show commit dialog
    // `git commit -m <commit message> ...` // ... and enter commit message and `git commit`


    //                                      // ... and if user changed from default selection, Merge will be required.
}

void ObjectModel::prepareMerge()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:prepare to determine target objects to be imported

    // `git checkout <branch>`              // make sure we are in proper branch
    // `git pull`                           // pull recent updates from remote


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to be imported
    refreshItems();

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:confirm

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute
    executeImport();

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:and now we are in access problem
}

bool ObjectModel::refreshItems()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to reflect from Project to FileSytem

    // refresh model items
    loadItemFromProject();                          //                  : BLOCK :                   :
    loadItemFromFileSystem();                       //                  :       :                   :

    clearTempDir();

    // for InProjectOnly
    {
        // .
    }

    // for InFileSytemOnly
    {
        // .
    }

    // for InBoth
    {
        ObjectItems targets;
        getItems(&targets, InBoth);

        exportFromProjectToTempDir(&targets);   // InBoth           : BLOCK :                   :
        sanitizeTempDir(&targets);              // InBoth           :       :                   :
        compareTempDir(&targets);               // InBoth           :       :                   :
        //.
    }

    return true;
}





bool ObjectModel::executeExport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to FileSystem

    // TODO: targets must be selected only

    // for InProjectOnly
    {
        ObjectItems targets;
        getItems(&targets, InProjectOnly);
        exportFromProjectToTempDir(&targets);       // InProjectOnly    : BLOCK :                   :
        sanitizeTempDir(&targets);                  // InProjectOnly    :       :                   :
        copyFromTempDirToFileSystem(&targets);      // InProjectOnly    :       : Dirty FileSystem  : need one-more step? like confirm
    }

    // for InFileSytemOnly
    {
        ObjectItems targets;
        getItems(&targets, InFileSystemOnly);
        deleteFromFileSystem(&targets);             // InFileSystemOnly :       : Dirty FileSystem  : need one-more step? like confirm
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Different);
            copyFromTempDirToFileSystem(&targets);  // InBoth_Different :       : Dirty FileSystem  : need one-more step? like confirm
        }
    }

    return true;
}


bool ObjectModel::executeImport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute

    // TODO: targets must be selected only

    // for InProjectOnly
    {
        ObjectItems targets;
        getItems(&targets, InProjectOnly);
        deleteFromProject(&targets);                // InProjectOnly    : BLOCK : Dirty Project : need one-more step? like confirm
    }

    // for InFileSytemOnly
    {
        ObjectItems targets;
        getItems(&targets, InFileSystemOnly);
        copyFromFileSystemToTempDir(&targets);      // InFileSytemOnly  :       :               :
        desanitizeTempDir(&targets);                // InFileSytemOnly  :       :               :
        importFromTempDirToProject(&targets);       // InFileSystemOnly : BLOCK : Dirty Project : need one-more step? like confirm
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Different);
            copyFromFileSystemToTempDir(&targets);  // InBoth_Different :       :               :
            desanitizeTempDir(&targets);            // InBoth_Different :       :               :
            importFromTempDirToProject(&targets);   // InBoth_Different : BLOCK : Dirty Project : need one-more step? like confirm
        }
    }

    return true;
}

























void ObjectModel::getItems(ObjectItems *pItems, ObjectModel::ItemsType itemsType) const
{
    foreach ( ObjectItem *item, m_items )
    {
        ObjectItem *toBeInserted = NULL;
        switch (itemsType)
        {
            case AllItems:
            {
                toBeInserted = item;
                break;
            }
            case InBoth:
            {
                if ( item->inProject()    == Model::Present &&
                     item->inFileSystem() == Model::Present )
                    toBeInserted = item;
                break;
            }
            case InBoth_Different:
            {
                if ( item->inProject()    == Model::Present &&
                     item->inFileSystem() == Model::Present &&
                     item->isDifferent()  == Model::DifferentContents )
                    toBeInserted = item;
                break;
            }
            case InProjectOnly:
            {
                if ( item->inProject()    == Model::Present &&
                     item->inFileSystem() == Model::Absent )
                    toBeInserted = item;
                break;
            }
            case InFileSystemOnly:
            {
                if ( item->inProject()    == Model::Absent &&
                     item->inFileSystem() == Model::Present )
                    toBeInserted = item;
                break;
            }
        }

        if (toBeInserted)
        {
            (*pItems)[ toBeInserted->objectType() ][ toBeInserted->name() ] = toBeInserted;
        }
    }
}


void ObjectModel::loadItemFromProject()
{
    ProgressNotifier mainProg(LoadItemFromProjectProcess, this);

    QList<ObjectItem*> items;

    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);


    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::TableDef
                    << Model::Query
                    << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                       // TODO: TableData
                       // TODO: Relation
                       ;

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            os = setting[ objectType ];

            if (!os->prepareItemCollection())
                continue;

            ProgressNotifier subProg(LoadItemFromProjectProcess, os->itemCount(), this);
            for (int i = 0 ; i < os->itemCount() ; ++i )
            {
                subProg.next();
                ComPtr<QAxObject> object = os->itemUnsafePtr(i);
                if ( os->isTargetObject( object.ptr() ) )
                {
                    items << os->createItemFromProject(object.ptr(), this);
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------
    // reset items
    beginRemoveRows( QModelIndex(), 0, m_items.count() - 1 );
    {
        ProgressNotifier subProg(LoadItemFromProjectProcess, m_mapItems.keys().length(), this);
        qDeleteAll(m_items);
        m_items.clear();

        foreach (Model::ObjectType t, m_mapItems.keys() )
        {
            subProg.next();
            m_mapItems[t].clear();
        }
    }
    endRemoveRows();

    beginInsertRows( QModelIndex(), 0, items.count() - 1);
    {
        ProgressNotifier subProg(LoadItemFromProjectProcess, items.length(), this);
        foreach( ObjectItem *item, items)
        {
            subProg.next();
            addItem( item );
        }
    }
    endInsertRows();
}

void ObjectModel::loadItemFromFileSystem()
{
    // load items from local file system.

    ProgressNotifier mainProg(LoadItemFromFileSystemProcess, this);

    beginResetModel();
    {
        ProgressNotifier subProg(LoadItemFromFileSystemProcess, m_items.length(), this);
        foreach ( ObjectItem *item, m_items )
        {
            subProg.next();
            item->setInFileSystem( Model::Absent );
        }
    }
    endResetModel();

    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (!currentProject.is())
        return;

    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    QDir sourceDir( setting.sourcePath() );
    if (!sourceDir.exists())
        return;

    QList<ObjectItem *> items;

    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::TableDef
                    << Model::Query
                    << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                       // TODO: TableData
                       // TODO: Relation
                       ;

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            os = setting[ objectType ];

            QDir objectDir( os->sourceObjectPath() );
            if (objectDir.exists())
            {
                objectDir.setNameFilters( (QStringList() << ("*." + os->existCheckExtension() ) ) );
                QFileInfoList fileInfos = objectDir.entryInfoList( QDir::Files );
                ProgressNotifier subProg(LoadItemFromFileSystemProcess, fileInfos.length(), this);
                foreach ( QFileInfo fileInfo, fileInfos )
                {
                    subProg.next();
                    items << os->createItemFromFileSystem(fileInfo, this);
                }
            }
        }
    }

    // update / insert items
    beginResetModel();
    {
        ProgressNotifier subProg(LoadItemFromFileSystemProcess, items.length(), this);
        foreach ( ObjectItem *item, items )
        {
            subProg.next();
            addItem( item );
        }
    }
    endResetModel();
}


void ObjectModel::exportFromProjectToTempDir(ObjectItems *allTargets)
{
    // export to temp directory
    //      for objects existing in both Project and FileSystem
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(ExportFromProjectToTempDirProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    ComPtr<Access::CurrentProject> currentProject  = m_application->CurrentProject();
    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
    if ( !currentProject.is() )
    {
        qDebug() << "currentProject is null";
        return;
    }
    if ( setting.isMDB() && !currentDb.is() )
    {
        qDebug() << "currentProject is null";
        return;
    }


    if ( setting.isMDB() || setting.isADP() )
    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::TableDef
                    << Model::Query
                    << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                       // TODO: TableData
                       // TODO: Relation
                       ;

        foreach ( Model::ObjectType objectType, objectTypes )
        {
            time.start();

            os = setting[ objectType ];
            os->mkdirTempObjectPath();
            if (!os->prepareItemCollection())
                continue;

            QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
            QStringList objectNames = targets.keys();

            int nCount = objectNames.count();
            ProgressNotifier subProg(ExportFromProjectToTempDirProcess, nCount, this);

            foreach ( QString objectName, objectNames )
            {
                subProg.next();

                ComPtr<QAxObject> object = os->itemUnsafePtr( objectName );
                os->exportFromProjectToTempDir(object.ptr(), objectName);
            }
            qDebug() << os->objectPathName() << " : " << nCount << " : " << time.elapsed();
        }
    }

    qDebug() << "DONE : " << timeTotal.elapsed() ;
}

void ObjectModel::importFromTempDirToProject(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(ImportFromTempDirToProjectProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);


    {
        QList<Model::ObjectType> objectTypes;
        objectTypes << Model::Query
                    << Model::Form
                    << Model::Report
                    << Model::Macro
                    << Model::Module
                    << Model::Reference
                       // TODO: TableDef
                       // TODO: TableData
                       // TODO: Relation
                    ;

        foreach (Model::ObjectType objectType, objectTypes)
        {
            os = setting[ objectType ];
            if (!os->prepareItemCollection())
                continue;

            QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
            QStringList objectNames = targets.keys();

            int nCount = objectNames.count();
            ProgressNotifier subProg(ImportFromTempDirToProjectProcess, nCount, this);

            foreach (QString objectName, objectNames)
            {
                subProg.next();
                ObjectItem *item = targets[ objectName ];
                if (item->inProject() == Model::Present)
                {
                    // FIXME: form/report/macro/module : makes error?
                    ComPtr<QAxObject> object = os->itemUnsafePtr( objectName );
                    os->importFromTempDirToProject(object.ptr(), objectName);
                }
                else
                    os->importFromTempDirToProject(NULL, objectName);
            }
        }
    }
}

void ObjectModel::copyFromTempDirToFileSystem(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(CopyFromTempDirToFileSystemProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];
        os->mkdirSourceObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(CopyFromTempDirToFileSystemProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            os->copyFromTempDirToFileSystem(objectName);
        }
    }
}

void ObjectModel::copyFromFileSystemToTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(CopyFromFileSystemToTempDirProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(CopyFromFileSystemToTempDirProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            os->copyFromFileSystemToTempDir(objectName);
        }
    }

}

void ObjectModel::sanitizeTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(SanitizeTempDirProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes
                << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(SanitizeTempDirProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            os->sanitizeTempDir(NULL, objectName);
        }
    }
}

void ObjectModel::desanitizeTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(DesanitizeTempDirProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(DesanitizeTempDirProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            os->desanitizeTempDir(NULL, objectName);
        }
    }

}

void ObjectModel::compareTempDir(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(CompareTempDirProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(CompareTempDirProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            bool isDifferent = false;
            os->compareTempDir(objectName, &isDifferent);

            ObjectItem *item = targets[ objectName ];
            if (isDifferent == true && item->isDifferent() != Model::DifferentContents )
                item->setDifferent( Model::DifferentContents );
            else if (isDifferent == false && item->isDifferent() != Model::SameContents )
                item->setDifferent( Model::SameContents );

        }
    }


    // update items
    emit dataChanged( createIndex(0, DifferentColumn), createIndex( m_items.count()-1, DifferentColumn ) );

}

void ObjectModel::deleteFromFileSystem(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(DeleteFromFileSystemProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
                << Model::Reference
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(DeleteFromFileSystemProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            os->deleteFromFileSystem(objectName);
        }
    }
}

void ObjectModel::deleteFromProject(ObjectItems *allTargets)
{
    QTime time;
    QTime timeTotal;
    timeTotal.start();
    time.start();
    ProgressNotifier mainProg(DeleteFromProjectProcess, this);

    ProjectSetting setting(this);
    ObjectSetting *os;

    setting.initialize(m_application);

    QList<Model::ObjectType> objectTypes;
    objectTypes << Model::TableDef
                << Model::Query
                << Model::Form
                << Model::Report
                << Model::Macro
                << Model::Module
              //<< Model::Reference  // no need to delete
                   // TODO: TableData
                   // TODO: Relation
                ;

    foreach (Model::ObjectType objectType, objectTypes)
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        int nCount = objectNames.count();
        ProgressNotifier subProg(DeleteFromProjectProcess, nCount, this);

        foreach (QString objectName, objectNames)
        {
            subProg.next();
            os->deleteFromProject(objectName);
        }
    }
}


















void ObjectModel::addItem(ObjectItem *item)
{

    if ( m_mapItems[ item->objectType() ].contains( item->name() ) )
    {
        // already exists.
        // so we must merge.
        ObjectItem *itemInList = m_mapItems[ item->objectType() ].value( item->name() );

        //TODO: object existent merge logic must be function.

        itemInList->setInProject(
                    (itemInList->inProject() == Model::Present || item->inProject() == Model::Present) ? Model::Present :
                    (itemInList->inProject() == Model::Absent  || item->inProject() == Model::Absent ) ? Model::Absent  :
                                                                                                         Model::OE_Unchecked );
        itemInList->setInFileSystem(
                    (itemInList->inFileSystem() == Model::Present || item->inFileSystem() == Model::Present) ? Model::Present :
                    (itemInList->inFileSystem() == Model::Absent  || item->inFileSystem() == Model::Absent ) ? Model::Absent  :
                                                                                                               Model::OE_Unchecked );

        itemInList->setCreateDate(   item      ->createDate().isValid() ? item      ->createDate()
                                 : ( itemInList->createDate().isValid() ? itemInList->createDate()
                                 : QDateTime() ) );
        itemInList->setUpdateDate(   item      ->updateDate().isValid() ? item      ->updateDate()
                                 : ( itemInList->updateDate().isValid() ? itemInList->updateDate()
                                 : QDateTime() ) );
        // and delete 'item' object.
        delete item;
    }
    else
    {
        m_items << item;
        m_mapItems[ item->objectType() ].insert( item->name(), item );
    }
}

void ObjectModel::clearTempDir()
{
    ProjectSetting setting(this);
    setting.initialize(m_application);

    QDir tempDir( setting.tempPath() );
    if (tempDir.exists())
    {
        tempDir.removeRecursively();
        tempDir.mkpath(".");
    }
}

