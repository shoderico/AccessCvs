#include "objectmodel.h"

#include <QIcon>
#include <QDir>

//#include <QApplication> // requred for QApplication::processEvents();
#include <QDebug>

#include <QtConcurrent>

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/progressnotifier.h"
#include "util/datachangedhelper.h"
#include "util/concurrentmaphelper.h"

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
            case InSourceDirColumn: return tr("InRepository");
            case CreateDateColumn: return tr("CreateDate");
            case UpdateDateColumn: return tr("UpdateDate");
            case ExportDateColumn: return tr("ExportDate");
            case ObjectTypeColumn: return tr("ObjectType");
            case DifferentColumn: return tr("Different");
            case HasDataColumn: return tr("HasData");
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
            case InSourceDirColumn:  return (int)item->inSourceDir();
            case CreateDateColumn:    return item->createDate();
            case UpdateDateColumn:    return item->updateDate();
            case ExportDateColumn:    return item->exportDate();
            case ObjectTypeColumn:    return item->objectType();
            case DifferentColumn:    return item->isDifferent();
            case HasDataColumn:       return item->hasData();
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
            case Model::TableDef:   return QIcon( ":/images/table.png" );
            case Model::Query:      return QIcon( ":/images/table_multiple.png" );
            case Model::Form:       return QIcon( ":/images/application_form.png" );
            case Model::Report:     return QIcon( ":/images/report.png" );
            case Model::Macro:      return QIcon( ":/images/script.png" );
            case Model::Module:     return QIcon( ":/images/page.png" );
            case Model::Reference:  return QIcon( ":/images/page_link.png" );
            case Model::ProjectFile: return QIcon( ":/images/page_link.png" );

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
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
        return false;

    if (role == Qt::CheckStateRole)
    {
        ObjectItem *item = m_items.at(index.row());
        switch (index.column()) {
            case NameColumn:
            {
                item->setSelected( value.toBool() );
                emit dataChanged(index, index);
                return true;
            }
                break;
            case HasDataColumn:
            {
                item->setHasData( value.toBool() );
                emit dataChanged(index, index);

                // reset exportDate, difference
                item->setDifferent( Model::Unchecked_OD );
                item->setExportDate( QDateTime() );
                emit dataChanged( createIndex(index.row(), DifferentColumn),  createIndex(index.row(), DifferentColumn) );
                emit dataChanged( createIndex(index.row(), ExportDateColumn), createIndex(index.row(), ExportDateColumn) );

                // Export dataFile into TempDir with hasData = true,
                // and then change hasData to false and do Refresh,
                // the item is always DifferentContents because *.dat file in TempDir but not in SourceDir.
                // so, we have to clear TempDir if hasData is changed.
                ObjectItems target;
                target[ item->objectType() ].insert( item->name(), item );
                deleteFromTempDir( &target );

                // save settings
                saveSettigs();
                return true;
            }
                break;
            default:
                return false;
        }
    }
    return false;
}

void ObjectModel::saveSettigs()
{
    // save settings
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    {
        QStringList tableDataTargets;
        QList<ObjectItem*> items = m_mapItems[ Model::TableDef ].values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it  )
        {
            if ( (*it)->hasData() )
                tableDataTargets.append( (*it)->name() );
        }

        os = setting[ Model::TableDef ];
        TableDefSetting *tableDataSetting = static_cast<TableDefSetting*>(os);
        tableDataSetting->setTableDataTargets( &tableDataTargets );
    }

    setting.saveSettings();
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
    //:confirm to reflect from Project to SourceDir


    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to SourceDir
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

    // smart-refresh : post-process
    updateItemsExportDate(&targets, QDateTime(), AllDifferenceTypes);
    updateItemsDifference(&targets, Model::Unchecked_OD);

    return true;
}

bool ObjectModel::refreshItems()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:determine target objects to reflect from Project to FileSytem

    // refresh model items
    reloadAndMergeItems();                              //                  : BLOCK :                   :

    // for InProjectOnly
    {
        // .done
    }

    // for InFileSytemOnly
    {
        // .done
    }

    // for InBoth
    {

        ObjectItems targetsAll;
        getItems(&targetsAll, InBoth, false/*selectedOnly*/, false/*modifiedOnly*/);

        // smart-refresh : pre-process
//        updateItemsDifferenceByFileTime(&targetsAll);                                       // for smart refresh, we assume the contents are the same if item's updateDate <= filetime of TempFile
        updateItemsDifferenceAsIs(&targetsAll);                                             // for smart refresh, we set item to be SameContents if TempDir and SourceDir are exactly the same

        ObjectItems targets;
        getItems(&targets, InBoth_NotSame, false/*selectedOnly*/, false/*modifiedOnly*/);

        exportFromProjectToTempDir(&targets);   // InBoth           : BLOCK :                   :
        sanitizeTempDir(&targets);              // InBoth           :       :                   :
        compareTempDir(&targets);               // InBoth           :       :                   :

        // smart-refresh : post-process
        updateFileTimeInTempDirByExportDate(&targets, DifferentContentsTypes);              // for smart refresh, we must rollback filetime of TempFile if different.
        updateItemsExportDate(&targets, QDateTime::currentDateTime(), SameContentsType);    // for smart-refresh, update exportDate

        //.done
    }

    return true;
}





bool ObjectModel::executeExport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to SourceDir

    if (!checkProjectState())
        return false;

    // first of all, we have to retreive target items for all states
    // because each process changes the states of items
    bool selectedOnly = true;

    ObjectItems targetsInProjectOnly;
    ObjectItems targetsInSourceDirOnly;
    ObjectItems targetsInBoth_Different;
    ObjectItems targetsInBoth_Same;

    getItems(&targetsInProjectOnly,    InProjectOnly,    selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInSourceDirOnly, InSourceDirOnly, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Different, InBoth_Different, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Same,      InBoth_Same,      selectedOnly, false/*modifiedOnly*/);

    // for InProjectOnly
    {
        {
            ObjectItems *targets = &targetsInProjectOnly;

            exportFromProjectToTempDir(targets);       // InProjectOnly    : BLOCK :                   :
            sanitizeTempDir(targets);                  // InProjectOnly    :       :                   :
            copyFromTempDirToSourceDir(targets);      // InProjectOnly    :       : Dirty SourceDir  : need one-more step? like confirm

            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            updateItemsExportDate(targets, currentTime, AllDifferenceTypes);   // set exportDate
            updateItemsInSourceDir(targets, Model::Present);                  // set inSourceDir flag to Present
            updateItemsDifference(targets, Model::SameContents);               // set isDifferent  flag to SameContents
        }
    }

    // for InFileSytemOnly
    {
        {
            ObjectItems *targets = &targetsInSourceDirOnly;

            deleteFromSourceDir(targets);                                             // InSourceDirOnly :       : Dirty SourceDir  : need one-more step? like confirm

            // smart-refresh : post-process
            deleteItems(targets); // delete item from model
        }
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems *targets = &targetsInBoth_Different;

            copyFromTempDirToSourceDir(targets);                                  // InBoth_Different :       : Dirty SourceDir  : need one-more step? like confirm

            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(targets, currentTime, AllDifferenceTypes);     // for smart-refresh, we need to update filetime which rollbacked in smart-refresh process
            updateItemsExportDate  (targets, currentTime, AllDifferenceTypes);     // for smart-refresh, update exportDate too.
            updateItemsDifference(targets, Model::SameContents);                   // set isDifferent flag to SameContents
        }

        // for InBoth_Same
        {
            ObjectItems *targets = &targetsInBoth_Same;

            copyFromTempDirToSourceDir(targets);                                  // InBoth_Same      :       : Dirty SourceDir  : need one-more step? like confirm

            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(targets, currentTime, AllDifferenceTypes);     // we need to update filetime which rollbacked in smart-refresh process
            updateItemsExportDate  (targets, currentTime, AllDifferenceTypes);     // update exportDate too.
        }
    }

    return true;
}


bool ObjectModel::executeImport()
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute

    if (!checkProjectState())
        return false;

    // first of all, we have to retreive target items for all states
    // because each process changes the states of items
    bool selectedOnly = true;

    ObjectItems targetsInProjectOnly;
    ObjectItems targetsInSourceDirOnly;
    ObjectItems targetsInBoth_Different;
    ObjectItems targetsInBoth_Same;

    getItems(&targetsInProjectOnly,    InProjectOnly,    selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInSourceDirOnly, InSourceDirOnly, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Different, InBoth_Different, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Same,      InBoth_Same,      selectedOnly, false/*modifiedOnly*/);


    // for InProjectOnly
    {
        {
            ObjectItems *targets = &targetsInProjectOnly;

            deleteFromProject(targets);                // InProjectOnly    : BLOCK : Dirty Project : need one-more step? like confirm

            // smart-refresh : post-process
            deleteItems(targets);                                                  // delete item from model
        }
    }

    // for InFileSytemOnly
    {
        {
            ObjectItems *targets = &targetsInSourceDirOnly;

            copyFromSourceDirToTempDir(targets);      // InFileSytemOnly  :       :               :
            desanitizeTempDir(targets);                // InFileSytemOnly  :       :               :
            importFromTempDirToProject(targets);       // InSourceDirOnly : BLOCK : Dirty Project : need one-more step? like confirm

            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(targets, currentTime, AllDifferenceTypes);     // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate  (targets, currentTime, AllDifferenceTypes);     // for smart-refresh, update exportDate too.
            updateItemsInProject(targets, Model::Present);                         // set inProject   flag to Present
            updateItemsDifference(targets, Model::SameContents);                   // set isDifferent flag to SameContents
            updateItemsCreateUpdateDateFromProject(targets);                       // set create/updateDate from Access Object
        }
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItems *targets = &targetsInBoth_Different;

            copyFromSourceDirToTempDir(targets);  // InBoth_Different :       :               :
            desanitizeTempDir(targets);            // InBoth_Different :       :               :
            importFromTempDirToProject(targets);   // InBoth_Different : BLOCK : Dirty Project : need one-more step? like confirm

            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(targets, currentTime, AllDifferenceTypes);     // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate  (targets, currentTime, AllDifferenceTypes);     // for smart-refresh, update exportDate too.
            updateItemsDifference(targets, Model::SameContents);                   // set isDifferent flag to SameContents
            updateItemsCreateUpdateDateFromProject(targets);                       // set create/updateDate from Access Object
        }
        // for InBoth_Same
        {
            ObjectItems *targets = &targetsInBoth_Same;

            copyFromSourceDirToTempDir(targets);  // InBoth_Same      :       :               :
            desanitizeTempDir(targets);            // InBoth_Same      :       :               :
            importFromTempDirToProject(targets);   // InBoth_Same      : BLOCK : Dirty Project : need one-more step? like confirm

            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            updateFileTimeInTempDir(targets, currentTime, AllDifferenceTypes);     // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate  (targets, currentTime, AllDifferenceTypes);     // for smart-refresh, update exportDate too.
            updateItemsCreateUpdateDateFromProject(targets);                       // set create/updateDate from Access Object
        }
    }

    m_application->RefreshDatabaseWindow();

    return true;
}

bool ObjectModel::checkProjectState()
{
    ComPtr<Access::CurrentProject> currentProject  = m_application->CurrentProject();
    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
    if ( !currentProject.is() )
    {
        qDebug() << "currentProject is null";
        return false;
    }
    int projectType = currentProject->ProjectType();
    if ( projectType == Access::acMDB && !currentDb.is() )
    {
        qDebug() << "currentDb is null";
        return false;
    }

    if ( (projectType != Access::acMDB) && (projectType != Access::acADP) )
    {
        qDebug() << "unknown project type";
        return false;
    }
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
                !item->isModified() // not-modified
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
            case Model::ProjectFile:  if ( !(objectTypes & ObjectModel::ProjectFileType) ) continue; break;
            default: break;
        }

        if (!toBeInserted && itemsType & InBoth)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InBoth_Different)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present &&
                 item->isDifferent()  == Model::DifferentContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InBoth_Same)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present &&
                 item->isDifferent()  == Model::SameContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InBoth_NotSame)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present &&
                 item->isDifferent()  != Model::SameContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InProjectOnly)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Absent )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & InSourceDirOnly)
        {
            if ( item->inProject()    == Model::Absent &&
                 item->inSourceDir() == Model::Present )
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
    selectItems( InProjectOnly | InSourceDirOnly | InBoth_Different, selected, resetSelection );
}

void ObjectModel::selectItems(ObjectModel::ItemsTypes itemsType, bool selected, bool resetSelection)
{
    DataChangedHelper helper( m_items.count() );
    if (resetSelection)
    {
        for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
            (*it)->setSelected( false );

        helper.changedAll();
    }

    ObjectItems targets;
    getItems(&targets, itemsType, false/*selectedOnly*/, false/*modifiedOnly*/);
    foreach (const Model::ObjectType &objectType, targets.keys() )
    {
        QList<ObjectItem*> items = targets[ objectType ].values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            (*it)->setSelected( selected );

            helper.changed( m_items.indexOf( (*it) ) );
        }
    }

    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), NameColumn), createIndex(helper.last(), NameColumn) );
}

void ObjectModel::selectItemsByObjectType(SelectObjectTypes objectTypes, bool selected, bool resetSelection)
{
    DataChangedHelper helper( m_items.count() );
    if (resetSelection)
    {
        for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
            (*it)->setSelected( false );

        helper.changedAll();
    }

    ObjectItems targets;
    getItems(&targets, ObjectModel::AllItems, objectTypes, false/*selectedOnly*/, false/*modifiedOnly*/);
    foreach (const Model::ObjectType &objectType, targets.keys() )
    {
        QList<ObjectItem*> items = targets[ objectType ].values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            (*it)->setSelected( selected );

            helper.changed( m_items.indexOf( (*it) ) );
        }
    }

    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), NameColumn), createIndex(helper.last(), NameColumn) );
}









struct BaseFunctionObject
{
    BaseFunctionObject(DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : m_dataChangedHelper(dataChangedHelper)
        , m_items(items)
    {
    }

    typedef void result_type;

    void registerChanged(ObjectItem* item)
    {
        if (m_dataChangedHelper && m_items)
            m_dataChangedHelper->changed( m_items->indexOf(item) );
    }

    DataChangedHelper *m_dataChangedHelper;
    QList<ObjectItem*> *m_items;
};














struct UpdateItemsExportDateFunctionObject : public BaseFunctionObject
{
    UpdateItemsExportDateFunctionObject(QDateTime exportDate, ObjectModel::ObjectDifferenceTypes differenceTypes, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_exportDate(exportDate)
        , m_differenceTypes(differenceTypes)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        switch( item->isDifferent() )
        {
            case Model::SameContents:       if (!(m_differenceTypes & ObjectModel::SameContentsType      )) return;
            case Model::DifferentContents:  if (!(m_differenceTypes & ObjectModel::DifferentContentsTypes)) return;
            default: break;
        }

        item->setExportDate(m_exportDate);
        registerChanged( item );
    }

    QDateTime m_exportDate;
    ObjectModel::ObjectDifferenceTypes m_differenceTypes;
};

void ObjectModel::updateItemsExportDate(ObjectItems *allTargets, const QDateTime &exportDate, const ObjectDifferenceTypes differenceTypes)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsExportDateProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsExportDateProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            switch( (*it)->isDifferent() )
            {
                case Model::SameContents:       if (!(differenceTypes & SameContentsType      )) continue; break;
                case Model::DifferentContents:  if (!(differenceTypes & DifferentContentsTypes)) continue; break;
                default: break;
            }

            (*it)->setExportDate(exportDate);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        // */


        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsExportDateFunctionObject(exportDate, differenceTypes, &helper, &m_items) ) );
        }
        // */
    }
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), ExportDateColumn), createIndex(helper.last(), ExportDateColumn) );
}


struct UpdateItemsInProjectFunctionObject : public BaseFunctionObject
{
    UpdateItemsInProjectFunctionObject(Model::ObjectExistence existence, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_existence(existence)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        item->setInProject(m_existence);
        registerChanged( item );
    }

    Model::ObjectExistence m_existence;
};

void ObjectModel::updateItemsInProject(ObjectItems *allTargets, Model::ObjectExistence existence)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsInProjectProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsInProjectProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            (*it)->setInProject(existence);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsInProjectFunctionObject(existence, &helper, &m_items) ) );
        }
        // */
    }
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), InProjectColumn), createIndex(helper.last(), InProjectColumn) );
}



struct UpdateItemsInSourceDirFunctionObject : public BaseFunctionObject
{
    UpdateItemsInSourceDirFunctionObject(Model::ObjectExistence existence, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_existence(existence)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        item->setInSourceDir(m_existence);
        registerChanged( item );
    }

    Model::ObjectExistence m_existence;
};

void ObjectModel::updateItemsInSourceDir(ObjectItems *allTargets, Model::ObjectExistence existence)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsInSourceDirProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsInSourceDirProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            (*it)->setInSourceDir(existence);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsInSourceDirFunctionObject(existence, &helper, &m_items) ) );
        }
        // */
    }
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), InSourceDirColumn), createIndex(helper.last(), InSourceDirColumn) );
}



struct UpdateItemsDifferenceFunctionObject : public BaseFunctionObject
{
    UpdateItemsDifferenceFunctionObject(Model::ObjectDifference difference, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_difference(difference)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        item->setDifferent(m_difference);
        registerChanged( item );
    }

    Model::ObjectDifference m_difference;
};

void ObjectModel::updateItemsDifference(ObjectItems *allTargets, Model::ObjectDifference difference)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsDifferenceProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsDifferenceProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            (*it)->setDifferent(difference);
            helper.changed( m_items.indexOf( (*it) ) );
        }
        // */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsDifferenceFunctionObject(difference, &helper, &m_items) ) );
        }
        // */

    }
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), DifferentColumn), createIndex(helper.last(), DifferentColumn) );
}


struct UpdateItemsDifferenceByFileTimeFunctionObject : public BaseFunctionObject
{
    UpdateItemsDifferenceByFileTimeFunctionObject(DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        if ( !item->isModified() )
        {
            item->setDifferent( Model::SameContents );
            registerChanged( item );
        }
    }
};

void ObjectModel::updateItemsDifferenceByFileTime(ObjectItems *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsDifferenceByFileTimeProcess, this);
    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsDifferenceByFileTimeProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            if ( !(*it)->isModified() )
            {
                (*it)->setDifferent( Model::SameContents );
                helper.changed( m_items.indexOf( (*it) ) );
            }
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsDifferenceByFileTimeFunctionObject( &helper, &m_items ) ) );
        }
        // */

    }
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), DifferentColumn), createIndex(helper.last(), DifferentColumn) );
}



struct UpdateItemsDifferenceAsIsFunctionObject : public BaseFunctionObject
{
    UpdateItemsDifferenceAsIsFunctionObject(ObjectSetting *os, Model::ObjectDifference difference, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_os(os)
        , m_difference(difference)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        if ( item->isModified() )
            return;

        if ( item->objectType() == Model::TableDef )
            return;

        bool isDifferent;
        m_os->compareTempDir( item->name(), &isDifferent );
        if ( isDifferent )
            return;

        item->setDifferent(m_difference);
        registerChanged( item );
    }

    ObjectSetting *m_os;
    Model::ObjectDifference m_difference;
};


void ObjectModel::updateItemsDifferenceAsIs(ObjectItems *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsDifferenceAsIsProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        os = setting[ objectType ];

        QList<ObjectItem*> items = allTargets->value( objectType ).values();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateItemsDifferenceAsIsProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();

            if ( (*it)->isModified() )
                continue;

            bool isDifferent;
            os->compareTempDir( (*it)->name(), &isDifferent );
            if ( !isDifferent )
            {
                (*it)->setDifferent( Model::SameContents );
                helper.changed( m_items.indexOf( (*it) ) );
            }
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateItemsDifferenceAsIsFunctionObject(os, Model::SameContents, NULL, NULL) ) );
        }
        // */

    }
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), DifferentColumn), createIndex(helper.last(), DifferentColumn) );
}

void ObjectModel::updateItemsCreateUpdateDateFromProject(ObjectItems *allTargets)
{
    // BLOCKING, cannot be asynch
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateItemsCreateUpdateDateFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        os = setting[ objectType ];
        if (!os->prepareItemCollection())
            continue;

        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        ProgressNotifier subProg(mainProg.type(), items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();

            ComPtr<QAxObject> object = os->itemUnsafePtr( (*it)->name() );
            ObjectItem *item = os->createItemFromProject( object.ptr(), 0 );
            (*it)->setCreateDate( item->createDate() );
            (*it)->setUpdateDate( item->updateDate() );
            delete item;
            helper.changed( m_items.indexOf( (*it) ) );
        }
    }
    if (helper.isChanged())
    {
        emit dataChanged( createIndex(helper.first(), CreateDateColumn), createIndex(helper.last(), CreateDateColumn) );
        emit dataChanged( createIndex(helper.first(), UpdateDateColumn), createIndex(helper.last(), UpdateDateColumn) );
    }
}



struct UpdateFileTimeInTempDirFunctionObject : public BaseFunctionObject
{
    UpdateFileTimeInTempDirFunctionObject(ObjectSetting *os, const QDateTime &fileTime, const ObjectModel::ObjectDifferenceTypes differenceTypes, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_os(os)
        , m_fileTime(fileTime)
        , m_differenceTypes(differenceTypes)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        switch( item->isDifferent() )
        {
            case Model::SameContents:       if (!(m_differenceTypes & ObjectModel::SameContentsType      )) return;
            case Model::DifferentContents:  if (!(m_differenceTypes & ObjectModel::DifferentContentsTypes)) return;
            default: break;
        }

        m_os->updateFileTimeInTempDir( item->name(), m_fileTime );
        registerChanged( item );
    }

    ObjectSetting *m_os;
    QDateTime m_fileTime;
    ObjectModel::ObjectDifferenceTypes m_differenceTypes;
};


void ObjectModel::updateFileTimeInTempDir(ObjectItems *allTargets, const QDateTime &fileTime, const ObjectDifferenceTypes differenceTypes)
{
    // non-blocking
    ProgressNotifier mainProg(UpdateFileTimeInTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        os = setting[ objectType ];
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateFileTimeInTempDirProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            switch( (*it)->isDifferent() )
            {
                case Model::SameContents:       if (!(differenceTypes & SameContentsType      )) continue; break;
                case Model::DifferentContents:  if (!(differenceTypes & DifferentContentsTypes)) continue; break;
                default: break;
            }

            os->updateFileTimeInTempDir( (*it)->name(), fileTime );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        // concurrent-map
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateFileTimeInTempDirFunctionObject(os, fileTime, differenceTypes, NULL, NULL) ) );
        }
        // */

    }
}

void ObjectModel::deleteItems(ObjectItems *allTargets)
{
    // FIXME: non-blocking, can be async ? require removeRows emission ?
    ProgressNotifier mainProg(DeleteItemsProcess, this);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        ProgressNotifier subProg(mainProg.type(), items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();

            int row = m_items.indexOf( (*it) );
            beginRemoveRows( QModelIndex(), row, row );
            m_mapItems[ objectType ].remove( (*it)->name() );
            m_items.removeAll( (*it) );
            delete (*it);
            endRemoveRows();
        }
    }
}


struct UpdateFileTimeInTempDirByExportDateFunctionObject : public BaseFunctionObject
{
    UpdateFileTimeInTempDirByExportDateFunctionObject(ObjectSetting *os, const ObjectModel::ObjectDifferenceTypes differenceTypes, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_os(os)
        , m_differenceTypes(differenceTypes)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        switch( item->isDifferent() )
        {
            case Model::SameContents:       if (!(m_differenceTypes & ObjectModel::SameContentsType      )) return;
            case Model::DifferentContents:  if (!(m_differenceTypes & ObjectModel::DifferentContentsTypes)) return;
            default: break;
        }

        if ( item->exportDate().isValid() )
            m_os->updateFileTimeInTempDir( item->name(), item->exportDate() );
        else
        {
            // treat as 1 sec. old from udpateDate
            QDateTime exportDate = item->updateDate().addSecs(-1);
            m_os->updateFileTimeInTempDir( item->name(), exportDate );
            item->setExportDate( exportDate );
            registerChanged( item );
        }
    }

    ObjectSetting *m_os;
    ObjectModel::ObjectDifferenceTypes m_differenceTypes;
};

void ObjectModel::updateFileTimeInTempDirByExportDate(ObjectItems *allTargets, const ObjectDifferenceTypes differenceTypes)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(UpdateFileTimeInTempDirByExportDateProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        os = setting[ objectType ];
        QList<ObjectItem*> items = allTargets->value( objectType ).values();


        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(UpdateFileTimeInTempDirByExportDateProcess, items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProg.next();
            switch( (*it)->isDifferent() )
            {
                case Model::SameContents:       if (!(differenceTypes & SameContentsType      )) continue; break;
                case Model::DifferentContents:  if (!(differenceTypes & DifferentContentsTypes)) continue; break;
                default: break;
            }

            if ( (*it)->exportDate().isValid() )
                os->updateFileTimeInTempDir( (*it)->name(), (*it)->exportDate() );
            else
            {
                // treat as 1 sec. old from udpateDate
                QDateTime exportDate = (*it)->updateDate().addSecs(-1);
                os->updateFileTimeInTempDir( (*it)->name(), exportDate );
                (*it)->setExportDate( exportDate );

                int row = m_items.indexOf( (*it) );
                emit dataChanged( createIndex(row, ExportDateColumn), createIndex(row, ExportDateColumn) );

            }
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, UpdateFileTimeInTempDirByExportDateFunctionObject(os, differenceTypes, &helper, &m_items) ) );
        }
        // */
    }

    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), ExportDateColumn), createIndex(helper.last(), ExportDateColumn) );
}


void ObjectModel::loadItemsFromProject(QList<ObjectItem*> *items)
{
    // BLOCKING, cannot be async
    ProgressNotifier mainProg(LoadItemFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes() )
    {
        os = setting[ objectType ];

        if ( !os->prepareItemCollection() )
            continue;

        ProgressNotifier subProg(mainProg.type(), os->itemCount(), this);
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


void ObjectModel::loadItemsFromSourceDir(QList<ObjectItem*> *items)
{
    // FIXME: non-blocking, can be async ? require append ?
    ProgressNotifier mainProg(LoadItemFromSourceDirProcess, this);
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
            ProgressNotifier subProg(mainProg.type(), fileInfos.length(), this);

            for (QFileInfoList::iterator it = fileInfos.begin(); it != fileInfos.end(); ++it )
            {
                subProg.next();
                items->append( os->createItemFromSourceDir( (*it), this) );
            }
        }
    }
}

void ObjectModel::reloadAndMergeItems()
{
    // FIXME: conjunction with BLOCKING and non-blocking, can be async ?

    // loading items more smart

    QList<ObjectItem*> itemsFromProject;
    QList<ObjectItem*> itemsFromSourceDir;

    loadItemsFromProject( &itemsFromProject );
    loadItemsFromSourceDir( &itemsFromSourceDir );


    // we need to do here for ..
    //  * delete item if not exist in both
    //  * add item if not exist in m_items/m_mapItems
    //  * update values


    // first of all, we merge both items into one
    QList<ObjectItem*> items;
    ObjectItems mapItems;

    // merged from Project
    for ( QList<ObjectItem*>::iterator it = itemsFromProject.begin(); it != itemsFromProject.end(); ++it )
    {
        // assume in project but NOT in fileSytem. merged later.
        (*it)->setInSourceDir( Model::Absent );

        items << (*it);
        mapItems[ (*it)->objectType() ].insert( (*it)->name(), (*it) );
    }
    // merged from SourceDir
    for ( QList<ObjectItem*>::iterator it = itemsFromSourceDir.begin(); it != itemsFromSourceDir.end(); ++it )
    {
        if ( mapItems[ (*it)->objectType() ].contains( (*it)->name() ) )
        {
            // merge properties
            mergeItemProperties( (*it), mapItems[ (*it)->objectType() ].value( (*it)->name() ) );
        }
        else
        {
            // insert new

            // in SourceDir but NOT in project
            (*it)->setInProject( Model::Absent );

            items << (*it);
            mapItems[ (*it)->objectType() ].insert( (*it)->name(), (*it) );
        }
    }


    // TODO: require updates more smartly
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
            //          * inSourceDir
            //          * isDifferent ???
            //          * hasData
            //     * no need to update
            //          * name
            //          * objectType
            //          * gitStatusInIndex (for now)
            //          * gitStatusInWorkTree (for now)

            item->setCreateDate( (*it)->createDate() );
            item->setUpdateDate( (*it)->updateDate() );
            item->setExportDate( (*it)->exportDate() );
            item->setInProject( (*it)->inProject() );
            item->setInSourceDir( (*it)->inSourceDir() );
            item->setDifferent( (*it)->isDifferent() );
            item->setHasData( (*it)->hasData() );
        }
    }

    endResetModel();

    // and delete local-items
    qDeleteAll( items.begin(), items.end() );
    items.clear();

}




void ObjectModel::exportFromProjectToTempDir(ObjectItems *allTargets)
{
    // BLOCKING, cannot be async

    // export to temp directory
    //      for objects existing in both Project and SourceDir
    //      for objects existing in ProjectOnly
    // without sanitizing and any extra processes.

    ProgressNotifier mainProg(ExportFromProjectToTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
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

void ObjectModel::importFromTempDirToProject(ObjectItems *allTargets)
{
    // BLOCKING, cannot be async

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
}



struct CopyFromTempDirToSourceDirFunctionObject
{
    CopyFromTempDirToSourceDirFunctionObject(ObjectSetting *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->copyFromTempDirToSourceDir(objectName);
    }

    ObjectSetting *m_os;
};


void ObjectModel::copyFromTempDirToSourceDir(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(CopyFromTempDirToSourceDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];
        os->mkdirSourceObjectPath();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(CopyFromTempDirToSourceDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->copyFromTempDirToSourceDir( (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, CopyFromTempDirToSourceDirFunctionObject(os) ) );
        }
        // */
    }
}



struct CopyFromSourceDirToTempDirFunctionObject
{
    CopyFromSourceDirToTempDirFunctionObject(ObjectSetting *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->copyFromSourceDirToTempDir(objectName);
    }

    ObjectSetting *m_os;
};

void ObjectModel::copyFromSourceDirToTempDir(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(CopyFromSourceDirToTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(CopyFromSourceDirToTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->copyFromSourceDirToTempDir( (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, CopyFromSourceDirToTempDirFunctionObject(os) ) );
        }
        // */
    }
}



struct SanitizeTempDirFunctionObject
{
    SanitizeTempDirFunctionObject(ObjectSetting *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->sanitizeTempDir(NULL, objectName);
    }

    ObjectSetting *m_os;
};

void ObjectModel::sanitizeTempDir(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(SanitizeTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];
        os->determineCodecForProject();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(SanitizeTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->sanitizeTempDir(NULL, (*it) );
        }
        // */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, SanitizeTempDirFunctionObject(os) ) );
        }
        // */
    }
}

struct DesanitizeTempDirFunctionObject
{
    DesanitizeTempDirFunctionObject(ObjectSetting *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->desanitizeTempDir(NULL, objectName);
    }

    ObjectSetting *m_os;
};

void ObjectModel::desanitizeTempDir(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(DesanitizeTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];
        os->determineCodecForProject();

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
        ProgressNotifier subProg(DesanitizeTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->desanitizeTempDir(NULL, (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, DesanitizeTempDirFunctionObject(os) ) );
        }
        // */
    }
}




struct CompareTempDirFunctionObject : public BaseFunctionObject
{
    CompareTempDirFunctionObject(ObjectSetting *os, DataChangedHelper *dataChangedHelper, QList<ObjectItem*> *items)
        : BaseFunctionObject(dataChangedHelper, items)
        , m_os(os)
    {
    }

    typedef void result_type;

    void operator()(ObjectItem *item)
    {
        bool isDifferent = false;
        m_os->compareTempDir( item->name() , &isDifferent);

        if (isDifferent == true && item->isDifferent() != Model::DifferentContents )
        {
            item->setDifferent( Model::DifferentContents );
            registerChanged( item );
        }
        else if (isDifferent == false && item->isDifferent() != Model::SameContents )
        {
            item->setDifferent( Model::SameContents );
            registerChanged( item );
        }
    }

    ObjectSetting *m_os;
};


void ObjectModel::compareTempDir(ObjectItems *allTargets)
{
    // non-blocking
    DataChangedHelper helper( m_items.count() );
    ProgressNotifier mainProg(CompareTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];


        //----------------------------------------------------------------------------------------
        // synchronous call
        /*
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
            helper.changed( m_items.indexOf( item ) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        //
        {
            ProgressNotifier subProg(mainProg.type(), items.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(items, CompareTempDirFunctionObject(os, &helper, &m_items) ) );
        }
        // */
    }

    // update items
    if (helper.isChanged())
        emit dataChanged( createIndex(helper.first(), DifferentColumn), createIndex( helper.last(), DifferentColumn ) );
}



struct DeleteFromSourceDirFunctionObject
{
    DeleteFromSourceDirFunctionObject(ObjectSetting *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->deleteFromSourceDir(objectName);
    }

    ObjectSetting *m_os;
};


void ObjectModel::deleteFromSourceDir(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(DeleteFromSourceDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // syncronous call
        /*
        ProgressNotifier subProg(DeleteFromSourceDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromSourceDir( (*it) );
        }
        */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, DeleteFromSourceDirFunctionObject(os) ) );
        }
        // */
    }
}

void ObjectModel::deleteFromProject(ObjectItems *allTargets)
{
    // BLOCKING, cannot be async

    ProgressNotifier mainProg(DeleteFromProjectProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
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



struct DeleteFromTempDirFunctionObject
{
    DeleteFromTempDirFunctionObject(ObjectSetting *os)
        : m_os(os)
    {
    }

    typedef void result_type;

    void operator()(const QString &objectName)
    {
        m_os->deleteFromTempDir(objectName);
    }

    ObjectSetting *m_os;
};

void ObjectModel::deleteFromTempDir(ObjectItems *allTargets)
{
    // non-blocking
    ProgressNotifier mainProg(DeleteFromTempDirProcess, this);
    ProjectSetting setting(this);
    ObjectSetting *os;
    setting.initialize(m_application);

    foreach (const Model::ObjectType &objectType, setting.objectTypes())
    {
        os = setting[ objectType ];

        QMap<QString, ObjectItem*> targets = allTargets->value( os->objectType() );
        QStringList objectNames = targets.keys();

        //----------------------------------------------------------------------------------------
        // syncronous call
        /*
        ProgressNotifier subProg(DeleteFromTempDirProcess, objectNames.count(), this);
        for (QStringList::iterator it = objectNames.begin(); it != objectNames.end(); ++it)
        {
            subProg.next();
            os->deleteFromTempDir( (*it) );
        }
        // */

        //----------------------------------------------------------------------------------------
        // asynchronous call
        //*
        {
            ProgressNotifier subProg(mainProg.type(), objectNames.count(), this);
            ConcurrentMapHelper<void> mapHelper( &subProg );
            mapHelper.run( QtConcurrent::map(objectNames, DeleteFromTempDirFunctionObject(os) ) );
        }
        // */
    }
}



void ObjectModel::mergeItemProperties(ObjectItem *itemSrc, ObjectItem *itemDst)
{
    // itemSrc : item created from SourceDir
    // itemDst : item created from Project

    itemDst->setInProject(
                (itemDst->inProject() == Model::Present || itemSrc->inProject() == Model::Present) ? Model::Present :
                (itemDst->inProject() == Model::Absent  || itemSrc->inProject() == Model::Absent ) ? Model::Absent  :
                                                                                                     Model::OE_Unchecked );

    itemDst->setInSourceDir(
                (itemDst->inSourceDir() == Model::Present || itemSrc->inSourceDir() == Model::Present) ? Model::Present :
                (itemDst->inSourceDir() == Model::Absent  || itemSrc->inSourceDir() == Model::Absent ) ? Model::Absent  :
                                                                                                           Model::OE_Unchecked );

    itemDst->setCreateDate(      itemSrc->createDate().isValid() ? itemSrc->createDate()
                             : ( itemDst->createDate().isValid() ? itemDst->createDate()
                             :   QDateTime() ) );

    itemDst->setUpdateDate(      itemSrc->updateDate().isValid() ? itemSrc->updateDate()
                             : ( itemDst->updateDate().isValid() ? itemDst->updateDate()
                             :   QDateTime() ) );

    itemDst->setHasData(    itemSrc->hasData() || itemDst->hasData() ? true : false );

}

