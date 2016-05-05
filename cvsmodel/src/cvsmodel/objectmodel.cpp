#include "objectmodel.h"

#include <QIcon>
#include <QDir>

//#include <QApplication> // requred for QApplication::processEvents();
#include <QDebug>

#include <QtConcurrent>

#include "accesslib/accesslib.h"
#include "util/comptr.h"
#include "util/progressnotifier.h"
#include "util/datachangedhelper.h"
#include "util/concurrentmaphelper.h"

#include "projectcontainer.h"
#include "processor/objectprocessor.h"
#include "processor/tabledefprocessor.h"
#include "objectitem.h"

#include "command/exportfromprojecttotempdircommand.h"
#include "command/sanitizetempdircommand.h"
#include "command/comparetempdircommand.h"
#include "command/updateitemsdifferenceasiscommand.h"
#include "command/updatefiletimeintempdirbyexportdatecommand.h"
#include "command/updateitemsexportdatecommand.h"
#include "command/deletefromtempdircommand.h"
#include "command/updateitemsdifferencecommand.h"
#include "command/copyfromtempdirtosourcedircommand.h"
#include "command/updateitemsinsourcedircommand.h"
#include "command/deletefromsourcedircommand.h"
#include "command/updatefiletimeintempdircommand.h"
#include "command/deletefromprojectcommand.h"
#include "command/copyfromsourcedirtotempdircommand.h"
#include "command/desanitizetempdircommand.h"
#include "command/importfromtempdirtoprojectcommand.h"
#include "command/updateitemsinprojectcommand.h"
#include "command/updateitemscreateupdatedatefromprojectcommand.h"
#include "command/loaditemsfromprojectcommand.h"
#include "command/loaditemsfromsourcedircommand.h"


ObjectModel::ObjectModel(QObject * parent)
    : QAbstractItemModel(parent)
    , m_application(0)
    , m_mapItems(0)
{
}

void ObjectModel::init(const QList<Model::ObjectType> &objectTypes)
{
    m_objectTypes = objectTypes;
    m_mapItems = new ObjectItemMap(objectTypes);
}

int ObjectModel::columnCount(const QModelIndex &parent) const
{
    // name,objectType
    // inProject,inRepository
    // createDate,updateDate
    if (parent.isValid())
        return 0;
    return Model::ColumnCount;
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
            case Model::NameColumn: return tr("Name");
            case Model::InProjectColumn: return tr("InProject");
            case Model::InSourceDirColumn: return tr("InRepository");
            case Model::CreateDateColumn: return tr("CreateDate");
            case Model::UpdateDateColumn: return tr("UpdateDate");
            case Model::ExportDateColumn: return tr("ExportDate");
            case Model::ObjectTypeColumn: return tr("ObjectType");
            case Model::DifferentColumn: return tr("Different");
            case Model::HasDataColumn: return tr("HasData");
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
            index.column() < 0 || index.column() >= Model::ColumnCount)
        return QVariant();

    const ObjectItem *item = m_items.at(index.row());

    if (role == Qt::SizeHintRole)
    {
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column()) {
            case Model::NameColumn:          return item->name();
            case Model::InProjectColumn:     return (int)item->inProject();
            case Model::InSourceDirColumn:  return (int)item->inSourceDir();
            case Model::CreateDateColumn:    return item->createDate();
            case Model::UpdateDateColumn:    return item->updateDate();
            case Model::ExportDateColumn:    return item->exportDate();
            case Model::ObjectTypeColumn:    return item->objectType();
            case Model::DifferentColumn:    return item->isDifferent();
            case Model::HasDataColumn:       return item->hasData();
        default:
            break;
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case Model::NameColumn:  return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
        default: return static_cast<int>(Qt::AlignVCenter | Qt::AlignCenter);
        }
    }

    if (role == Qt::DecorationRole && index.column() == Model::NameColumn)
    {
        return QIcon( item->iconPath() );
    }

    if (role == Qt::CheckStateRole)
    {
        switch (index.column())
        {
        case Model::NameColumn: return static_cast<int>(item->isSelected() ? Qt::Checked : Qt::Unchecked);
        //case InProject: return static_cast<int>(item->inProject() ? Qt::Checked : Qt::Unchecked);
        //case InRepository: return static_cast<int>(item->inRepository() ? Qt::Checked : Qt::Unchecked);
        }
    }

    return QVariant();
}

QModelIndex ObjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= Model::ColumnCount || row >= m_items.count() ||
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
            case Model::NameColumn:
            {
                item->setSelected( value.toBool() );
                emit dataChanged(index, index);
                emitSelectionChanged();
                return true;
            }
                break;
            case Model::HasDataColumn:
            {
                item->setHasData( value.toBool() );
                emit dataChanged(index, index);

                // reset exportDate, difference
                item->setDifferent( Model::Unchecked_OD );
                item->setExportDate( QDateTime() );
                emit dataChanged( createIndex(index.row(), Model::DifferentColumn),  createIndex(index.row(), Model::DifferentColumn) );
                emit dataChanged( createIndex(index.row(), Model::ExportDateColumn), createIndex(index.row(), Model::ExportDateColumn) );

                // no need to do this because Refresh() does clear temp dir
                /*
                // Export dataFile into TempDir with hasData = true,
                // and then change hasData to false and do Refresh,
                // the item is always DifferentContents because *.dat file in TempDir but not in SourceDir.
                // so, we have to clear TempDir if hasData is changed.
                if ( item->hasData() )
                {
                    ObjectItemMap target;
                    target[ item->objectType() ].insert( item->name(), item );
                    DeleteFromTempDirCommand deleteFromTempDir(m_application, this);
                    deleteFromTempDir.execute( &target );
                }
                */

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
    ProjectContainer setting(this);
    ObjectProcessor *processor;
    setting.initialize(m_application);

    {
        QStringList tableDataTargets;
        QList<ObjectItem*> items = m_mapItems->value( Model::TableDef ).values();
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it  )
        {
            if ( (*it)->hasData() )
                tableDataTargets.append( (*it)->name() );
        }

        processor = setting[ Model::TableDef ];
        TableDefProcessor *tableDataProcessor = static_cast<TableDefProcessor*>(processor);
        tableDataProcessor->setTableDataTargets( &tableDataTargets );
    }

    setting.saveSettings();
}

void ObjectModel::setApplication(QAxObject *application)
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
    ObjectItemMap targets(m_objectTypes);
    getItems(&targets, Model::AllItems, true /*selectedOnly*/, false /* modifiedOnly */);

    DeleteFromTempDirCommand deleteFromTempDir(m_application, this);
    deleteFromTempDir.execute(&targets);

    // smart-refresh : post-process
    UpdateItemsExportDateCommand updateItemsExportDate(QDateTime(), Model::AllDifferenceTypes,  m_application, &m_items, this);
    UpdateItemsDifferenceCommand updateItemsDifference(Model::Unchecked_OD,                     m_application, &m_items, this);
    updateItemsExportDate.execute(&targets);
    updateItemsDifference.execute(&targets);

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

        ObjectItemMap targetsAll(m_objectTypes);
        getItems(&targetsAll, Model::InBoth, false/*selectedOnly*/, false/*modifiedOnly*/);


        // smart-refresh : pre-process
        UpdateItemsDifferenceAsIsCommand updateItemsDifferenceAsIs(m_application, &m_items, this);
//        updateItemsDifferenceByFileTime(&targetsAll); // for smart refresh, we assume the contents are the same if item's updateDate <= filetime of TempFile
        updateItemsDifferenceAsIs.execute(&targetsAll); // for smart refresh, we set item to be SameContents if TempDir and SourceDir are exactly the same


        ObjectItemMap targets(m_objectTypes);
        getItems(&targets, Model::InBoth_NotSame, false/*selectedOnly*/, false/*modifiedOnly*/);

        ExportFromProjectToTempDirCommand   exportFromProjectToTempDir  (m_application, this);
        SanitizeTempDirCommand              sanitizeTempDir             (m_application, this);
        CompareTempDirCommand               compareTempDir              (m_application, &m_items, this);
        exportFromProjectToTempDir  .execute(&targets); // InBoth           : BLOCK :                   :
        sanitizeTempDir             .execute(&targets); // InBoth           :       :                   :
        compareTempDir              .execute(&targets); // InBoth           :       :                   :


        // smart-refresh : post-process
        UpdateFileTimeInTempDirByExportDateCommand  updateFileTimeInTempDirByExportDate (Model::DifferentContentsTypes,                         m_application, &m_items, this);
        UpdateItemsExportDateCommand                updateItemsExportDate               (QDateTime::currentDateTime(), Model::SameContentsType, m_application, &m_items, this);
        updateFileTimeInTempDirByExportDate .execute(&targets); // for smart refresh, we must rollback filetime of TempFile if different.
        updateItemsExportDate               .execute(&targets); // for smart-refresh, update exportDate

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

    ObjectItemMap targetsInProjectOnly(m_objectTypes);
    ObjectItemMap targetsInSourceDirOnly(m_objectTypes);
    ObjectItemMap targetsInBoth_Different(m_objectTypes);
    ObjectItemMap targetsInBoth_Same(m_objectTypes);

    getItems(&targetsInProjectOnly,    Model::InProjectOnly,    selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInSourceDirOnly,  Model::InSourceDirOnly,  selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Different, Model::InBoth_Different, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Same,      Model::InBoth_Same,      selectedOnly, false/*modifiedOnly*/);

    // for InProjectOnly
    {
        {
            ObjectItemMap *targets = &targetsInProjectOnly;

            ExportFromProjectToTempDirCommand   exportFromProjectToTempDir  (m_application, this);
            SanitizeTempDirCommand              sanitizeTempDir             (m_application, this);
            CopyFromTempDirToSourceDirCommand   copyFromTempDirToSourceDir  (m_application, this);
            exportFromProjectToTempDir  .execute(targets);  // InProjectOnly    : BLOCK :                   :
            sanitizeTempDir             .execute(targets);  // InProjectOnly    :       :                   :
            copyFromTempDirToSourceDir  .execute(targets);  // InProjectOnly    :       : Dirty SourceDir  : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_application, &m_items, this);
            UpdateItemsInSourceDirCommand   updateItemsInSourceDir  (Model::Present,                            m_application, &m_items, this);
            UpdateItemsDifferenceCommand    updateItemsDifference   (Model::SameContents,                       m_application, &m_items, this);
            updateItemsExportDate   .execute(targets);  // set exportDate
            updateItemsInSourceDir  .execute(targets);  // set inSourceDir flag to Present
            updateItemsDifference   .execute(targets);  // set isDifferent  flag to SameContents

        }
    }

    // for InFileSytemOnly
    {
        {
            ObjectItemMap *targets = &targetsInSourceDirOnly;

            DeleteFromSourceDirCommand deleteFromSourceDir(m_application, this);
            deleteFromSourceDir.execute(targets);                                             // InSourceDirOnly :       : Dirty SourceDir  : need one-more step? like confirm

            // smart-refresh : post-process
            deleteItems(targets); // delete item from model
        }
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItemMap *targets = &targetsInBoth_Different;

            CopyFromTempDirToSourceDirCommand copyFromTempDirToSourceDir(m_application, this);
            copyFromTempDirToSourceDir.execute(targets);                                  // InBoth_Different :       : Dirty SourceDir  : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand  updateFileTimeInTempDir (currentTime, Model::AllDifferenceTypes,    m_application, this);
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_application, &m_items, this);
            UpdateItemsDifferenceCommand    updateItemsDifference   (Model::SameContents,                       m_application, &m_items, this);
            updateFileTimeInTempDir .execute(targets);     // for smart-refresh, we need to update filetime which rollbacked in smart-refresh process
            updateItemsExportDate   .execute(targets);     // for smart-refresh, update exportDate too.
            updateItemsDifference   .execute(targets);     // set isDifferent flag to SameContents

        }

        // for InBoth_Same
        {
            ObjectItemMap *targets = &targetsInBoth_Same;

            CopyFromTempDirToSourceDirCommand copyFromTempDirToSourceDir(m_application, this);
            copyFromTempDirToSourceDir.execute(targets);    // InBoth_Same      :       : Dirty SourceDir  : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand  updateFileTimeInTempDir (currentTime, Model::AllDifferenceTypes,    m_application, this);
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_application, &m_items, this);
            updateFileTimeInTempDir .execute(targets);  // we need to update filetime which rollbacked in smart-refresh process
            updateItemsExportDate   .execute(targets);  // update exportDate too.

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

    ObjectItemMap targetsInProjectOnly(m_objectTypes);
    ObjectItemMap targetsInSourceDirOnly(m_objectTypes);
    ObjectItemMap targetsInBoth_Different(m_objectTypes);
    ObjectItemMap targetsInBoth_Same(m_objectTypes);

    getItems(&targetsInProjectOnly,    Model::InProjectOnly,    selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInSourceDirOnly,  Model::InSourceDirOnly,  selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Different, Model::InBoth_Different, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Same,      Model::InBoth_Same,      selectedOnly, false/*modifiedOnly*/);


    // for InProjectOnly
    {
        {
            ObjectItemMap *targets = &targetsInProjectOnly;

            DeleteFromProjectCommand deleteFromProject(m_application, this);
            deleteFromProject.execute(targets); // InProjectOnly    : BLOCK : Dirty Project : need one-more step? like confirm

            // smart-refresh : post-process
            deleteItems(targets);   // delete item from model
        }
    }

    // for InFileSytemOnly
    {
        {
            ObjectItemMap *targets = &targetsInSourceDirOnly;

            CopyFromSourceDirToTempDirCommand   copyFromSourceDirToTempDir  (m_application, this);
            DesanitizeTempDirCommand            desanitizeTempDir           (m_application, this);
            ImportFromTempDirToProjectCommand   importFromTempDirToProject  (m_application, this);
            copyFromSourceDirToTempDir  .execute(targets);  // InFileSytemOnly  :       :               :
            desanitizeTempDir           .execute(targets);  // InFileSytemOnly  :       :               :
            importFromTempDirToProject  .execute(targets);  // InSourceDirOnly : BLOCK : Dirty Project : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand                  updateFileTimeInTempDir                 (currentTime, Model::AllDifferenceTypes,    m_application, this);
            UpdateItemsExportDateCommand                    updateItemsExportDate                   (currentTime, Model::AllDifferenceTypes,    m_application, &m_items, this);
            UpdateItemsInProjectCommand                     updateItemsInProject                    (Model::Present,                            m_application, &m_items, this);
            UpdateItemsDifferenceCommand                    updateItemsDifference                   (Model::SameContents,                       m_application, &m_items, this);
            UpdateItemsCreateUpdateDateFromProjectCommand   updateItemsCreateUpdateDateFromProject  (                                           m_application, &m_items, this);
            updateFileTimeInTempDir                 .execute(targets);  // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate                   .execute(targets);  // for smart-refresh, update exportDate too.
            updateItemsInProject                    .execute(targets);  // set inProject   flag to Present
            updateItemsDifference                   .execute(targets);  // set isDifferent flag to SameContents
            updateItemsCreateUpdateDateFromProject  .execute(targets);  // set create/updateDate from Access Object

        }
    }

    // for InBoth
    {
        // for InBoth_Different
        {
            ObjectItemMap *targets = &targetsInBoth_Different;

            CopyFromSourceDirToTempDirCommand   copyFromSourceDirToTempDir  (m_application, this);
            DesanitizeTempDirCommand            desanitizeTempDir           (m_application, this);
            ImportFromTempDirToProjectCommand   importFromTempDirToProject  (m_application, this);
            copyFromSourceDirToTempDir  .execute(targets);  // InBoth_Different :       :               :
            desanitizeTempDir           .execute(targets);  // InBoth_Different :       :               :
            importFromTempDirToProject  .execute(targets);  // InBoth_Different : BLOCK : Dirty Project : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand                  updateFileTimeInTempDir                 (currentTime, Model::AllDifferenceTypes,    m_application, this);
            UpdateItemsExportDateCommand                    updateItemsExportDate                   (currentTime, Model::AllDifferenceTypes,    m_application, &m_items, this);
            UpdateItemsDifferenceCommand                    updateItemsDifference                   (Model::SameContents,                       m_application, &m_items, this);
            UpdateItemsCreateUpdateDateFromProjectCommand   updateItemsCreateUpdateDateFromProject  (                                           m_application, &m_items, this);
            updateFileTimeInTempDir                 .execute(targets);  // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate                   .execute(targets);  // for smart-refresh, update exportDate too.
            updateItemsDifference                   .execute(targets);  // set isDifferent flag to SameContents
            updateItemsCreateUpdateDateFromProject  .execute(targets);  // set create/updateDate from Access Object

        }
        // for InBoth_Same
        {
            ObjectItemMap *targets = &targetsInBoth_Same;

            CopyFromSourceDirToTempDirCommand   copyFromSourceDirToTempDir  (m_application, this);
            DesanitizeTempDirCommand            desanitizeTempDir           (m_application, this);
            ImportFromTempDirToProjectCommand   importFromTempDirToProject  (m_application, this);
            copyFromSourceDirToTempDir  .execute(targets);  // InBoth_Same      :       :               :
            desanitizeTempDir           .execute(targets);  // InBoth_Same      :       :               :
            importFromTempDirToProject  .execute(targets);  // InBoth_Same      : BLOCK : Dirty Project : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand                  updateFileTimeInTempDir                 (currentTime, Model::AllDifferenceTypes,    m_application, this);
            UpdateItemsExportDateCommand                    updateItemsExportDate                   (currentTime, Model::AllDifferenceTypes,    m_application, &m_items, this);
            UpdateItemsCreateUpdateDateFromProjectCommand   updateItemsCreateUpdateDateFromProject  (                                           m_application, &m_items, this);
            updateFileTimeInTempDir                 .execute(targets);  // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate                   .execute(targets);  // for smart-refresh, update exportDate too.
            updateItemsCreateUpdateDateFromProject  .execute(targets);  // set create/updateDate from Access Object

        }
    }

    return true;
}

bool ObjectModel::checkProjectState()
{
    return true;
}

int ObjectModel::selectedRowCount() const
{
    int count  = 0;
    for ( QList<ObjectItem*>::const_iterator it = m_items.begin() ; it != m_items.end() ; ++it )
    {
        if ((*it)->isSelected())
            ++count;
    }
    return count;
}

























void ObjectModel::getItems(ObjectItemMap *pItems, Model::ItemsTypes itemsType, bool selectedOnly, bool modifiedOnly) const
{
    getItems( pItems, itemsType, Model::AllObjectTypes, selectedOnly, modifiedOnly);
}

void ObjectModel::getItems(ObjectItemMap *pItems, Model::ItemsTypes itemsType, Model::SelectObjectTypes objectTypes, bool selectedOnly, bool modifiedOnly) const
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
        if ( !(objectTypes & item->selectObjectType()) )
            continue;



        if (!toBeInserted && itemsType & Model::InBoth)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & Model::InBoth_Different)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present &&
                 item->isDifferent()  == Model::DifferentContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & Model::InBoth_Same)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present &&
                 item->isDifferent()  == Model::SameContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & Model::InBoth_NotSame)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Present &&
                 item->isDifferent()  != Model::SameContents )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & Model::InProjectOnly)
        {
            if ( item->inProject()    == Model::Present &&
                 item->inSourceDir() == Model::Absent )
                toBeInserted = item;
        }
        if (!toBeInserted && itemsType & Model::InSourceDirOnly)
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
    selectItems( Model::InProjectOnly | Model::InSourceDirOnly | Model::InBoth_Different, selected, resetSelection );
}

void ObjectModel::selectItems(Model::ItemsTypes itemsType, bool selected, bool resetSelection)
{
    DataChangedHelper helper( m_items.count() );
    if (resetSelection)
    {
        for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
            (*it)->setSelected( false );

        helper.changedAll();
    }

    ObjectItemMap targets(m_objectTypes);
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
        emit dataChanged( createIndex(helper.first(), Model::NameColumn), createIndex(helper.last(), Model::NameColumn) );

    emitSelectionChanged();
}

void ObjectModel::selectItemsByObjectType(Model::SelectObjectTypes objectTypes, bool selected, bool resetSelection)
{
    DataChangedHelper helper( m_items.count() );
    if (resetSelection)
    {
        for (QList<ObjectItem*>::iterator it = m_items.begin() ; it != m_items.end() ; ++it )
            (*it)->setSelected( false );

        helper.changedAll();
    }

    ObjectItemMap targets(m_objectTypes);
    getItems(&targets, Model::AllItems, objectTypes, false/*selectedOnly*/, false/*modifiedOnly*/);
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
        emit dataChanged( createIndex(helper.first(), Model::NameColumn), createIndex(helper.last(), Model::NameColumn) );

    emitSelectionChanged();
}

void ObjectModel::emitSelectionChanged()
{
    int objectTypes = 0;
    foreach( const Model::ObjectType &objectType, m_mapItems->keys() )
    {
       foreach( const ObjectItem *item, m_mapItems->value( objectType ).values() )
       {
           if (item->isSelected())
           {
               objectTypes |= item->selectObjectType();
               break;
           }
       }
    }
    emit selectionChanged( objectTypes );
}






void ObjectModel::deleteItems(ObjectItemMap *allTargets)
{
    // FIXME: non-blocking, can be async ? require removeRows emission ?
    ProgressNotifier mainProgress(Model::DeleteItemsProcess, this);

    foreach (const Model::ObjectType &objectType, allTargets->keys() )
    {
        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProgress.next();

            int row = m_items.indexOf( (*it) );
            beginRemoveRows( QModelIndex(), row, row );
            m_mapItems->operator []( objectType ).remove( (*it)->name() );
            m_items.removeAll( (*it) );
            delete (*it);
            endRemoveRows();
        }
    }
}



void ObjectModel::reloadAndMergeItems()
{
    // FIXME: conjunction with BLOCKING and non-blocking, can be async ?

    // loading items more smart

    QList<ObjectItem*> itemsFromProject;
    QList<ObjectItem*> itemsFromSourceDir;

    LoadItemsFromProjectCommand     loadItemsFromProject    (m_application, &itemsFromProject, this);
    LoadItemsFromSourceDirCommand   loadItemsFromSourceDir  (m_application, &itemsFromSourceDir, this);
    loadItemsFromProject    .execute( NULL );
    loadItemsFromSourceDir  .execute( NULL );


    // we need to do here for ..
    //  * delete item if not exist in both
    //  * add item if not exist in m_items/m_mapItems
    //  * update values


    // first of all, we merge both items into one
    QList<ObjectItem*> items;
    ObjectItemMap mapItems(m_objectTypes);

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
            m_mapItems->operator []( (*it)->objectType() ).remove( (*it)->name() );
            it = m_items.erase( it );
            --it; // back one
            delete item;
        }
    }

    // add new-item to member  if not exist in member
    for ( QList<ObjectItem*>::iterator it = items.begin(); it != items.end(); ++it )
    {
        if ( !m_mapItems->value( (*it)->objectType() ).contains( (*it)->name() ) )
        {
            ObjectItem *item = new ObjectItem( (*it) , this);
            m_mapItems->operator []( (*it)->objectType() ).insert( (*it)->name(), item );
            m_items.append( item );
        }
    }

    // and now, we merge  from new-item to local-item
    for ( QList<ObjectItem*>::iterator it = items.begin(); it != items.end(); ++it )
    {
        if ( m_mapItems->value( (*it)->objectType() ).contains( (*it)->name() ) )
        {
            ObjectItem *item = m_mapItems->value( (*it)->objectType() ).value( (*it)->name() );

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




void ObjectModel::itemDataChanged(int rowStart, int rowEnd, int columnStart, int columnEnd)
{
    emit dataChanged( createIndex(rowStart, columnStart), createIndex(rowEnd, columnEnd) );
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

