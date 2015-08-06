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
            case ExportDateColumn: return tr("ExportDate");
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
            case ExportDateColumn:    return item->exportDate();
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

bool ObjectModel::clearItemsCache()
{
    ObjectItems targets;
    getItems(&targets, AllItems, true /*selectedOnly*/, false /* modifiedOnly */);
    deleteFromTempDir(&targets);
    updateExportDate(&targets, QDateTime());
    return true;
}

bool ObjectModel::refreshItems()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to reflect from Project to FileSytem

    // refresh model items
    reloadAndMergeItems();                          //
//    loadItemFromProject();                          //                  : BLOCK :                   :
//    loadItemFromFileSystem();                       //                  :       :                   :

//    clearTempDir(); // keep old files

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
        // for smart refresh, we assume the contents is the same if item's updateDate <= filetime of TempFile
        assumeItemsTheSameByFileTime();

        ObjectItems targets;
        getItems(&targets, InBoth, false, true /* modifiedOnly */);

        exportFromProjectToTempDir(&targets);   // InBoth           : BLOCK :                   :
        sanitizeTempDir(&targets);              // InBoth           :       :                   :
        compareTempDir(&targets);               // InBoth           :       :                   :


        rollbackFileTimeIfDifferent(&targets);                          // for smart refresh, we must rollback filetime of TempFile if different.
        updateExportDateIfSame(&targets, QDateTime::currentDateTime()); // for smart-refresh, update exportDate

        //.
    }

    return true;
}





bool ObjectModel::executeExport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to FileSystem

    bool selectedOnly = true;

    // for InProjectOnly
    {
        ObjectItems targets;
        getItems(&targets, InProjectOnly, selectedOnly, false/*modifiedOnly*/);
        exportFromProjectToTempDir(&targets);       // InProjectOnly    : BLOCK :                   :
        sanitizeTempDir(&targets);                  // InProjectOnly    :       :                   :
        copyFromTempDirToFileSystem(&targets);      // InProjectOnly    :       : Dirty FileSystem  : need one-more step? like confirm

        QDateTime currentTime = QDateTime::currentDateTime();
        updateExportDate(&targets, currentTime);    // for smart-refresh, update exportDate
    }

    // for InFileSytemOnly
    {
        ObjectItems targets;
        getItems(&targets, InFileSystemOnly, selectedOnly, false/*modifiedOnly*/);
        deleteFromFileSystem(&targets);             // InFileSystemOnly :       : Dirty FileSystem  : need one-more step? like confirm
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Different, selectedOnly, false/*modifiedOnly*/);
            copyFromTempDirToFileSystem(&targets);  // InBoth_Different :       : Dirty FileSystem  : need one-more step? like confirm

            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(&targets, currentTime); // for smart-refresh, we need to update filetime which rollbacked in smart-refresh process
            updateExportDate(&targets, currentTime);        // for smart-refresh, update exportDate too.
        }
        // for InBoth_Same
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Same, selectedOnly, false/*modifiedOnly*/);
            copyFromTempDirToFileSystem(&targets);  // InBoth_Same      :       : Dirty FileSystem  : need one-more step? like confirm

            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(&targets, currentTime); // for smart-refresh, we need to update filetime which rollbacked in smart-refresh process
            updateExportDate(&targets, currentTime);        // for smart-refresh, update exportDate too.
        }
    }

    return true;
}


bool ObjectModel::executeImport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute

    bool selectedOnly = true;

    // for InProjectOnly
    {
        ObjectItems targets;
        getItems(&targets, InProjectOnly, selectedOnly, false/*modifiedOnly*/);
        deleteFromProject(&targets);                // InProjectOnly    : BLOCK : Dirty Project : need one-more step? like confirm
    }

    // for InFileSytemOnly
    {
        ObjectItems targets;
        getItems(&targets, InFileSystemOnly, selectedOnly, false/*modifiedOnly*/);
        copyFromFileSystemToTempDir(&targets);      // InFileSytemOnly  :       :               :
        desanitizeTempDir(&targets);                // InFileSytemOnly  :       :               :
        importFromTempDirToProject(&targets);       // InFileSystemOnly : BLOCK : Dirty Project : need one-more step? like confirm

        QDateTime currentTime = QDateTime::currentDateTime();
        updateFileTimeInTempDir(&targets, currentTime); // for smart refresh, update filetime of TempFile if imported
        updateExportDate(&targets, currentTime);        // for smart-refresh, update exportDate too.
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Different, selectedOnly, false/*modifiedOnly*/);
            copyFromFileSystemToTempDir(&targets);  // InBoth_Different :       :               :
            desanitizeTempDir(&targets);            // InBoth_Different :       :               :
            importFromTempDirToProject(&targets);   // InBoth_Different : BLOCK : Dirty Project : need one-more step? like confirm

            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(&targets, currentTime); // for smart refresh, update filetime of TempFile if imported
            updateExportDate(&targets, currentTime);        // for smart-refresh, update exportDate too.
        }
        // for InBoth_Different
        {
            ObjectItems targets;
            getItems(&targets, InBoth_Same, selectedOnly, false/*modifiedOnly*/);
            copyFromFileSystemToTempDir(&targets);  // InBoth_Same      :       :               :
            desanitizeTempDir(&targets);            // InBoth_Same      :       :               :
            importFromTempDirToProject(&targets);   // InBoth_Same      : BLOCK : Dirty Project : need one-more step? like confirm

            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(&targets, currentTime); // for smart refresh, update filetime of TempFile if imported
            updateExportDate(&targets, currentTime);        // for smart-refresh, update exportDate too.
        }
    }

    m_application->RefreshDatabaseWindow();

    return true;
}

























void ObjectModel::getItems(ObjectItems *pItems, ObjectModel::ItemsTypes itemsType, bool selectedOnly, bool modifiedOnly) const
{
    getItems( pItems, itemsType, ObjectModel::AllObjectTypes, selectedOnly, modifiedOnly);
}

void ObjectModel::getItems(ObjectItems *pItems, ItemsTypes itemsType, SelectObjectTypes objectTypes, bool selectedOnly, bool modifiedOnly) const
{
    for ( QList<ObjectItem*>::const_iterator it = m_items.begin() ; it != m_items.end() ; ++it )
    {
        ObjectItem *item = (*it);
        ObjectItem *toBeInserted = NULL;

        // skip non-selected item
        if (selectedOnly &&
                !item->isSelected() // not-selected
                )
            continue;

        // skip non-modified item
        if (modifiedOnly &&
                item->updateDate().isValid() && item->exportDate().isValid() &&
                item->updateDate() <= item->exportDate() // not-modified
                )
            continue;

        // skip non-target object type
        switch (item->objectType())
        {
            case Model::TableDef:   if ( !(objectTypes & ObjectModel::TableObjectType    ) ) continue; break;
            case Model::Query:      if ( !(objectTypes & ObjectModel::QueryObjectType    ) ) continue; break;
            case Model::Form:       if ( !(objectTypes & ObjectModel::FormObjectType     ) ) continue; break;
            case Model::Report:     if ( !(objectTypes & ObjectModel::ReportObjectType   ) ) continue; break;
            case Model::Macro:      if ( !(objectTypes & ObjectModel::MacroObjectType    ) ) continue; break;
            case Model::Module:     if ( !(objectTypes & ObjectModel::ModuleObjectType   ) ) continue; break;
            case Model::Reference:  if ( !(objectTypes & ObjectModel::ReferenceObjectType) ) continue; break;
            default: break;
        }
        /*
        if ( ( (objectTypes & ObjectModel::TableObjectType)     && item->objectType() == Model::TableDef  ) ||
             ( (objectTypes & ObjectModel::QueryObjectType)     && item->objectType() == Model::Query     ) ||
             ( (objectTypes & ObjectModel::FormObjectType)      && item->objectType() == Model::Form      ) ||
             ( (objectTypes & ObjectModel::ReportObjectType)    && item->objectType() == Model::Report    ) ||
             ( (objectTypes & ObjectModel::MacroObjectType)     && item->objectType() == Model::Macro     ) ||
             ( (objectTypes & ObjectModel::ModuleObjectType)    && item->objectType() == Model::Module    ) ||
             ( (objectTypes & ObjectModel::ReferenceObjectType) && item->objectType() == Model::Reference ) )
        {
        }
        */

        if (!toBeInserted && itemsType & InBoth)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inFileSystem() == Model::Present )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InBoth_Different)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inFileSystem() == Model::Present &&
                 item->isDifferent()  == Model::DifferentContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InBoth_Same)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inFileSystem() == Model::Present &&
                 item->isDifferent()  == Model::SameContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InProjectOnly)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inFileSystem() == Model::Absent )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InFileSystemOnly)
        {
            if ( item->inProject()    == Model::Absent &&
                 item->inFileSystem() == Model::Present )
                toBeInserted = item;
        }

        if (toBeInserted && ( (selectedOnly && item->isSelected()) || !selectedOnly ) )
        {
            (*pItems)[ toBeInserted->objectType() ][ toBeInserted->name() ] = toBeInserted;
        }
    }
}

void ObjectModel::selectItemsForProcess(bool selected, bool resetSelection)
{
    selectItems( InProjectOnly | InFileSystemOnly | InBoth_Different, selected, resetSelection );
}

void ObjectModel::selectItems(ObjectModel::ItemsTypes itemsType, bool selected, bool resetSelection)
{
    int first = m_items.count();
    int last = 0;
    if (resetSelection)
    {
        for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
            (*it)->setSelected( false );

        first = 0;
        last = m_items.length() - 1;
    }

    ObjectItems targets;
    getItems(&targets, itemsType, false/*selectedOnly*/, false/*modifiedOnly*/);
    foreach (const Model::ObjectType &objectType, targets.keys() )
    {
        QList<ObjectItem*> items = targets[ objectType ].values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            (*it)->setSelected( selected );

            int row = m_items.indexOf( (*it) );
            if (first > row) first = row;
            if (last  < row) last  = row;
        }
    }

    emit dataChanged( createIndex(first, 0), createIndex(last, ColumnCount) );
}

void ObjectModel::selectItemsByObjectType(SelectObjectTypes objectTypes, bool selected, bool resetSelection)
{
    int first = m_items.count();
    int last = 0;
    if (resetSelection)
    {
        for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
            (*it)->setSelected( false );

        first = 0;
        last = m_items.length() - 1;
    }

    ObjectItems targets;
    getItems(&targets, ObjectModel::AllItems, objectTypes, false/*selectedOnly*/, false/*modifiedOnly*/);
    foreach (const Model::ObjectType &objectType, targets.keys() )
    {
        QList<ObjectItem*> items = targets[ objectType ].values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            (*it)->setSelected( selected );

            int row = m_items.indexOf( (*it) );
            if (first > row) first = row;
            if (last  < row) last  = row;
        }
    }

    emit dataChanged( createIndex(first, 0), createIndex(last, ColumnCount) );
}

void ObjectModel::assumeItemsTheSameByFileTime()
{
    for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
    {
        if ( (*it)->inProject() && (*it)->inFileSystem() &&
             (*it)->updateDate().isValid() && (*it)->exportDate().isValid() &&
             (*it)->updateDate() <= (*it)->exportDate() )
        {
            (*it)->setDifferent( Model::SameContents );
        }
    }
}

void ObjectModel::rollbackFileTimeIfDifferent(ObjectItems *allTargets)
{
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        os = setting[ objectType ];
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            if ( (*it)->isDifferent() == Model::DifferentContents )
            {
                os->rollbackFileTimeTempDir( (*it)->name(), (*it)->exportDate() );
            }
        }
    }
}

void ObjectModel::updateExportDateIfSame(ObjectItems *allTargets, const QDateTime &exportDate)
{
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            if ( (*it)->isDifferent() == Model::SameContents )
            {
                (*it)->setExportDate(exportDate);
            }
        }
    }
}

void ObjectModel::updateExportDate(ObjectItems *allTargets, const QDateTime &exportDate)
{
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            (*it)->setExportDate(exportDate);
        }
    }
}

void ObjectModel::updateFileTimeInTempDir(ObjectItems *allTargets, const QDateTime &fileTime)
{
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        os = setting[ objectType ];
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            os->rollbackFileTimeTempDir( (*it)->name(), fileTime );
        }
    }
}


void ObjectModel::loadItemFromProject(QList<ObjectItem*> *items)
{
    ProgressNotifier mainProg(LoadItemFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes() )
    {
        os = setting[ objectType ];

        if ( !os->prepareItemCollection() )
            continue;

        ProgressNotifier subProg(LoadItemFromProjectProcess, os->itemCount(), this);
        for ( int i = 0 ; i < os->itemCount() ; ++i )
        {
            subProg.next();
            ComPtr<QAxObject> object = os->itemUnsafePtr(i);
            if ( os->isTargetObject( object.ptr() ) )
            {
                items->append( os->createItemFromProject(object.ptr(), this) );
            }
        }
    }

}


void ObjectModel::loadItemFromFileSystem(QList<ObjectItem*> *items)
{
    // load items from local file system.

    ProgressNotifier mainProg(LoadItemFromFileSystemProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes() )
    {
        os = setting[ objectType ];

        QDir objectDir( os->sourceObjectPath() );
        if (objectDir.exists())
        {
            objectDir.setNameFilters( (QStringList() << ("*." + os->existCheckExtension() ) ) );
            QFileInfoList fileInfos = objectDir.entryInfoList( QDir::Files );
            ProgressNotifier subProg(LoadItemFromFileSystemProcess, fileInfos.length(), this);

            for (QFileInfoList::iterator it = fileInfos.begin(); it != fileInfos.end(); ++it )
            {
                subProg.next();
                items->append( os->createItemFromFileSystem( (*it), this) );
            }
        }
    }
}

void ObjectModel::reloadAndMergeItems()
{
    // loading items more smart

    QList<ObjectItem*> itemsFromProject;
    QList<ObjectItem*> itemsFromFileSystem;

    loadItemFromProject( &itemsFromProject );
    loadItemFromFileSystem( &itemsFromFileSystem );


    // we need to do here for ..
    //  * delete item if not exist in both
    //  * add item if not exist in m_items/m_mapItems
    //  * update values


    // first of all, we merge both items into one
    QList<ObjectItem*> items;
    ObjectItems mapItems;

    //
    for ( QList<ObjectItem*>::iterator it = itemsFromProject.begin(); it != itemsFromProject.end(); ++it )
    {
        ObjectItem *item = (*it);
        items << item;
        mapItems[ item->objectType() ].insert( item->name(), item );
        item->setInFileSystem( Model::Absent );
    }
    for ( QList<ObjectItem*>::iterator it = itemsFromFileSystem.begin(); it != itemsFromFileSystem.end(); ++it )
    {
        if ( mapItems[ (*it)->objectType() ].contains( (*it)->name() ) )
        {
            // exist. merge property
            mergeItemProperties( (*it), mapItems[ (*it)->objectType() ].value( (*it)->name() ) );
        }
        else
        {
            items << (*it);
            mapItems[ (*it)->objectType() ].insert( (*it)->name(), (*it) );
        }
    }


    beginResetModel();

    // delete local-item from member  if not exist in new-items
    for ( QList<ObjectItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it )
    {
        if ( !mapItems[ (*it)->objectType() ].contains( (*it)->name() ) )
        {
            ObjectItem *item = (*it);
            m_mapItems[ (*it)->objectType() ].remove( (*it)->name() );
            it = m_items.erase( it );
            --it; // back one
            delete item;
        }
    }
    // okey until here

    // add new-item to member  if not exist in member
    for ( QList<ObjectItem*>::iterator it = items.begin(); it != items.end(); ++it )
    {
        if ( !m_mapItems[ (*it)->objectType() ].contains( (*it)->name() ) )
        {
            ObjectItem *item = new ObjectItem( (*it) , this);
            m_mapItems[ (*it)->objectType() ].insert( (*it)->name(), item );
            m_items.append( item );
        }
    }

    // and now, we merge  from new-item to local-item
    for ( QList<ObjectItem*>::iterator it = items.begin(); it != items.end(); ++it )
    {
        if ( m_mapItems[ (*it)->objectType() ].contains( (*it)->name() ) )
        {
            ObjectItem *item = m_mapItems[ (*it)->objectType() ].value( (*it)->name() );

            //  * update values
            //      * must keep
            //          * selected state
            //      * must update
            //          * createDate
            //          * updateDate
            //          * exportDate
            //          * inProject
            //          * inFileSystem
            //          * isDifferent ???
            //     * no need to update
            //          * name
            //          * objectType
            //          * gitStatusInIndex (for now)
            //          * gitStatusInWorkTree (for now)

            item->setCreateDate( (*it)->createDate() );
            item->setUpdateDate( (*it)->updateDate() );
            item->setExportDate( (*it)->exportDate() );
            item->setInProject( (*it)->inProject() );
            item->setInFileSystem( (*it)->inFileSystem() );
            item->setDifferent( (*it)->isDifferent() );
        }
    }

    endResetModel();

    // and delete local-items
    qDeleteAll( items.begin(), items.end() );
    items.clear();

}




void ObjectModel::exportFromProjectToTempDir(ObjectItems *allTargets)
{
    // export to temp directory
    //      for objects existing in both Project and FileSystem
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

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

    if ( !setting.isMDB() && !setting.isADP() )
    {
        qDebug() << "unknown project type";
        return;
    }

    foreach ( const Model::ObjectType &objectType, setting.objectTypes() )
    {
        os = setting[ objectType ];
        os->mkdirTempObjectPath();

        if (!os->prepareItemCollection())
            continue;

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(ExportFromProjectToTempDirProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            ComPtr<QAxObject> object = os->itemUnsafePtr( (*it) );
            os->exportFromProjectToTempDir(object.ptr(), (*it) );
        }
    }
}

void ObjectModel::importFromTempDirToProject(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(ImportFromTempDirToProjectProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        if (!os->prepareItemCollection())
            continue;

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(ImportFromTempDirToProjectProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            ObjectItem *item = targets[ (*it) ];
            if (item->inProject() == Model::Present)
            {
                // FIXME: form/report/macro/module : makes error?
                ComPtr<QAxObject> object = os->itemUnsafePtr( (*it) );
                os->importFromTempDirToProject(object.ptr(), (*it));
            }
            else
                os->importFromTempDirToProject( NULL, (*it) );
        }
    }
}

void ObjectModel::copyFromTempDirToFileSystem(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(CopyFromTempDirToFileSystemProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];
        os->mkdirSourceObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(CopyFromTempDirToFileSystemProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->copyFromTempDirToFileSystem( (*it) );
        }
    }
}

void ObjectModel::copyFromFileSystemToTempDir(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(CopyFromFileSystemToTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(CopyFromFileSystemToTempDirProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->copyFromFileSystemToTempDir( (*it) );
        }
    }

}

void ObjectModel::sanitizeTempDir(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(SanitizeTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(SanitizeTempDirProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->sanitizeTempDir(NULL, (*it) );
        }
    }
}

void ObjectModel::desanitizeTempDir(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(DesanitizeTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(DesanitizeTempDirProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->desanitizeTempDir(NULL, (*it) );
        }
    }

}

void ObjectModel::compareTempDir(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(CompareTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(CompareTempDirProcess, objectNames.count(), this);

        bool isDifferent = false;
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            isDifferent = false;
            os->compareTempDir( (*it) , &isDifferent);

            ObjectItem *item = targets[ (*it) ];
            if (isDifferent == true && item->isDifferent() != Model::DifferentContents )
            {
                item->setDifferent( Model::DifferentContents );
            }
            else if (isDifferent == false && item->isDifferent() != Model::SameContents )
            {
                item->setDifferent( Model::SameContents );
            }
        }
    }

    // update items
    emit dataChanged( createIndex(0, DifferentColumn), createIndex( m_items.count()-1, DifferentColumn ) );
}

void ObjectModel::deleteFromFileSystem(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(DeleteFromFileSystemProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(DeleteFromFileSystemProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromFileSystem( (*it) );
        }
    }
}

void ObjectModel::deleteFromProject(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(DeleteFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(DeleteFromProjectProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromProject( (*it) );
        }
    }
}

void ObjectModel::deleteFromTempDir(ObjectItems *allTargets)
{
    ProgressNotifier mainProg(DeleteFromTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();
        ProgressNotifier subProg(DeleteFromTempDirProcess, objectNames.count(), this);

        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteTempFileFromTempDir( (*it) );
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

void ObjectModel::mergeItemProperties(ObjectItem *itemSrc, ObjectItem *itemDst)
{
    itemDst->setInProject(
                (itemDst->inProject() == Model::Present || itemSrc->inProject() == Model::Present) ? Model::Present :
                (itemDst->inProject() == Model::Absent  || itemSrc->inProject() == Model::Absent ) ? Model::Absent  :
                                                                                                     Model::OE_Unchecked );

    itemDst->setInFileSystem(
                (itemDst->inFileSystem() == Model::Present || itemSrc->inFileSystem() == Model::Present) ? Model::Present :
                (itemDst->inFileSystem() == Model::Absent  || itemSrc->inFileSystem() == Model::Absent ) ? Model::Absent  :
                                                                                                           Model::OE_Unchecked );

    itemDst->setCreateDate(      itemSrc->createDate().isValid() ? itemSrc->createDate()
                             : ( itemDst->createDate().isValid() ? itemDst->createDate()
                             :   QDateTime() ) );

    itemDst->setUpdateDate(      itemSrc->updateDate().isValid() ? itemSrc->updateDate()
                             : ( itemDst->updateDate().isValid() ? itemDst->updateDate()
                             :   QDateTime() ) );

}

