#include "objectmodel.h"

#include <QIcon>
#include <QDir>

//#include <QApplication> // requred for QApplication::processEvents();
#include <QDebug>

#include <QtConcurrent>

//#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/progressnotifier.h"
#include "util/datachangedhelper.h"
#include "util/concurrentmaphelper.h"

#include "projectcontainer.h"
#include "processor/objectprocessor.h"
//#include "processor/tabledefprocessor.h"
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

#include "pch.hpp"


ObjectModel::ObjectModel(QObject * parent)
    : QAbstractItemModel(parent)
    , m_application(0)
    , m_itemMap(0)
{
}

ObjectModel::~ObjectModel()
{
    if (m_itemMap)
        delete m_itemMap;
}

void ObjectModel::init(ProjectContainer *projectContainer)
{
    m_objectTypesForItemMap = projectContainer->objectTypesForItemMap();
    m_itemMap = new ObjectItemMap(m_objectTypesForItemMap);

    m_projectContainer = projectContainer;
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
    m_projectContainer->updateSetting( &m_items );
    m_projectContainer->saveSetting();
}

ProjectContainer *ObjectModel::projectContainer() const
{
    return m_projectContainer;
}

QString ObjectModel::objectTypeDisplayName(Model::ObjectType objectType) const
{
    return m_projectContainer ? m_projectContainer->objectTypeDisplayName(objectType) : QString();
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
    m_projectContainer->initialize(m_application);

    ObjectItemMap targets(m_objectTypesForItemMap);
    getItems(&targets, Model::AllItems, true /*selectedOnly*/, false /* modifiedOnly */);

    DeleteFromTempDirCommand deleteFromTempDir(m_projectContainer, m_application, this);
    deleteFromTempDir.execute(&targets);

    // smart-refresh : post-process
    UpdateItemsExportDateCommand updateItemsExportDate(QDateTime(), Model::AllDifferenceTypes,  m_projectContainer, m_application, &m_items, this);
    UpdateItemsDifferenceCommand updateItemsDifference(Model::Unchecked_OD,                     m_projectContainer, m_application, &m_items, this);
    updateItemsExportDate.execute(&targets);
    updateItemsDifference.execute(&targets);

    return true;
}

bool ObjectModel::refreshItems()
{
    m_projectContainer->initialize(m_application);

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

        ObjectItemMap targetsAll(m_objectTypesForItemMap);
        getItems(&targetsAll, Model::InBoth, false/*selectedOnly*/, false/*modifiedOnly*/);


        // smart-refresh : pre-process
        UpdateItemsDifferenceAsIsCommand updateItemsDifferenceAsIs(m_projectContainer, m_application, &m_items, this);
//        updateItemsDifferenceByFileTime(&targetsAll); // for smart refresh, we assume the contents are the same if item's updateDate <= filetime of TempFile
        updateItemsDifferenceAsIs.execute(&targetsAll); // for smart refresh, we set item to be SameContents if TempDir and SourceDir are exactly the same


        ObjectItemMap targets(m_objectTypesForItemMap);
        getItems(&targets, Model::InBoth_NotSame, false/*selectedOnly*/, false/*modifiedOnly*/);

        ExportFromProjectToTempDirCommand   exportFromProjectToTempDir  (m_projectContainer, m_application, this);
        SanitizeTempDirCommand              sanitizeTempDir             (m_projectContainer, m_application, this);
        CompareTempDirCommand               compareTempDir              (m_projectContainer, m_application, &m_items, this);
        exportFromProjectToTempDir  .execute(&targets); // InBoth           : BLOCK :                   :
        sanitizeTempDir             .execute(&targets); // InBoth           :       :                   :
        compareTempDir              .execute(&targets); // InBoth           :       :                   :


        // smart-refresh : post-process
        UpdateFileTimeInTempDirByExportDateCommand  updateFileTimeInTempDirByExportDate (Model::DifferentContentsTypes,                         m_projectContainer, m_application, &m_items, this);
        UpdateItemsExportDateCommand                updateItemsExportDate               (QDateTime::currentDateTime(), Model::SameContentsType, m_projectContainer, m_application, &m_items, this);
        updateFileTimeInTempDirByExportDate .execute(&targets); // for smart refresh, we must rollback filetime of TempFile if different.
        updateItemsExportDate               .execute(&targets); // for smart-refresh, update exportDate

        //.done
    }

    return true;
}





bool ObjectModel::executeExport()
{
    m_projectContainer->initialize(m_application);

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute to reflect from Project to SourceDir

    if (!checkProjectState())
        return false;

    // first of all, we have to retreive target items for all states
    // because each process changes the states of items
    bool selectedOnly = true;

    ObjectItemMap targetsInProjectOnly(m_objectTypesForItemMap);
    ObjectItemMap targetsInSourceDirOnly(m_objectTypesForItemMap);
    ObjectItemMap targetsInBoth_Different(m_objectTypesForItemMap);
    ObjectItemMap targetsInBoth_Same(m_objectTypesForItemMap);

    getItems(&targetsInProjectOnly,    Model::InProjectOnly,    selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInSourceDirOnly,  Model::InSourceDirOnly,  selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Different, Model::InBoth_Different, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Same,      Model::InBoth_Same,      selectedOnly, false/*modifiedOnly*/);

    // -------------------------------------------------------------------------
    // Post-open HasData toggle support (ensure .dat in TempDir at Export time)
    // -------------------------------------------------------------------------
    // User symptom: "UIを開く際に、TempDirに書き出して...UIを開いた後、HasDataチェックをオンにして、.datがTempDirに書き出されていないので、ExportしてもSourceDirに.datがコピーされない"
    // Root cause: refreshItems does ExportFromProjectToTempDirCommand (at that moment's m_tableDataTargets).
    //             setData(HasDataColumn) only does item->setHasData + dataChanged + setDifferent(Unchecked_OD) + saveSettigs (updates TableData list) — no TempDir I/O.
    //             executeExport's InBoth_Different/Same paths do *only* CopyFromTempDirToSourceDir (no re-ExportToTemp).
    //             Unchecked_OD (0) does not match InBoth_Different (==DifferentContents) or InBoth_Same (==SameContents); it matches InBoth_NotSame (!= SameContents).
    // User decisions (captured via askQuestions): B (defer actual data export to Export button click — toggle stays responsive), leave stale .dat/.dattmp on uncheck, yes force ensure for InBoth items too.
    // Approach (per approved follow-up plan): lightweight virtual ensureDataInTempDir on ObjectProcessor (no-op default).
    //             TableObjectProcessor impl: m_tableDataTargets guard + QFile.exists early return + exportTableData + sanitizeTableData only (no structure touch).
    //             We take a *broad* snapshot (InBoth_NotSame | InBoth_Same | InBoth | InProjectOnly) so toggled items are not missed.
    //             ensure is called before InBoth Copies (critical) and safely after InProjectOnly (exists() guard skips work).
    //             This is data-only; full ExportFromProjectToTempDirCommand is intentionally not reused here.
    ObjectItemMap ensureDataCandidates(m_objectTypesForItemMap);
    getItems(&ensureDataCandidates,
             Model::InBoth_NotSame | Model::InBoth_Same | Model::InBoth | Model::InProjectOnly,
             selectedOnly, false/*modifiedOnly*/);

    // for InProjectOnly
    {
        {
            ObjectItemMap *targets = &targetsInProjectOnly;

            // Note: ensureDataInTempDir (post-toggle) is invoked later for the broad ensureDataCandidates snapshot.
            // For items that were already InProjectOnly at refresh time, the initial ExportFromProjectToTempDirCommand
            // already exported data for then-current m_tableDataTargets. The later ensure pass will safely no-op
            // (via QFile.exists guard) for those that already have .dat/.dattmp. This also covers any InProjectOnly
            // items that were toggled on after open (they would have been Unchecked_OD and thus not in the initial export).
            ExportFromProjectToTempDirCommand   exportFromProjectToTempDir  (m_projectContainer, m_application, this);
            SanitizeTempDirCommand              sanitizeTempDir             (m_projectContainer, m_application, this);
            CopyFromTempDirToSourceDirCommand   copyFromTempDirToSourceDir  (m_projectContainer, m_application, this);
            exportFromProjectToTempDir  .execute(targets);  // InProjectOnly    : BLOCK :                   :
            sanitizeTempDir             .execute(targets);  // InProjectOnly    :       :                   :
            copyFromTempDirToSourceDir  .execute(targets);  // InProjectOnly    :       : Dirty SourceDir  : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsInSourceDirCommand   updateItemsInSourceDir  (Model::Present,                            m_projectContainer, m_application, &m_items, this);
            UpdateItemsDifferenceCommand    updateItemsDifference   (Model::SameContents,                       m_projectContainer, m_application, &m_items, this);
            updateItemsExportDate   .execute(targets);  // set exportDate
            updateItemsInSourceDir  .execute(targets);  // set inSourceDir flag to Present
            updateItemsDifference   .execute(targets);  // set isDifferent  flag to SameContents

        }
    }

    // -------------------------------------------------------------------------
    // ensureDataInTempDir for hasData items (post-UI-open HasData toggle support)
    // -------------------------------------------------------------------------
    // This pass covers items that became hasData targets *after* refreshItems (which did the initial
    // ExportFromProjectToTempDirCommand based on the m_tableDataTargets at that time).
    // We iterate the broad ensureDataCandidates snapshot (InBoth_NotSame captures Unchecked_OD items).
    // For table processors this will call exportTableData + sanitizeTableData (lightweight, data-only).
    // For other processors this is a no-op (virtual default).
    // Placement: after InProjectOnly (its full export already ran for then-current targets; exists() guards redundancy)
    // and before InBoth copies (the critical path for items that were InBoth at open time but toggled later).
    // User decisions honored: B (batch at Export click), leave stale files on uncheck, force for InBoth.
    {
        foreach (const Model::ObjectType &objectType, ensureDataCandidates.keys())
        {
            ObjectProcessor *processor = m_projectContainer->operator[](objectType);
            if (!processor)
                continue;
            const QMap<QString, ObjectItem*> &itemsForType = ensureDataCandidates[objectType];
            for (QMap<QString, ObjectItem*>::const_iterator it = itemsForType.constBegin();
                 it != itemsForType.constEnd(); ++it)
            {
                ObjectItem *item = it.value();
                if (item && item->hasData())
                {
                    processor->ensureDataInTempDir( item->name() );
                }
            }
        }
    }

    // -------------------------------------------------------------------------
    // Dedicated post-ensure data copy path for post-open HasData toggle (InBoth_NotSame items)
    // (implements user-selected option C from the bug-fix plan)
    // -------------------------------------------------------------------------
    // User symptom (exact): "UIを開いた後、HasDataチェックをオンにして、Export対象として選択して、Exportを行うと、TempDirには .dattmp, .dat が生成されているのに、SourceDir にはコピーされないんだよね。"
    //
    // Context: The ensure loop above (using broad ensureDataCandidates including InBoth_NotSame) successfully populates
    // .dattmp/.dat in TempDir for items toggled hasData *after* UI open (they are Unchecked_OD and thus classified InBoth_NotSame).
    // However, the earlier snapshots targetsInBoth_Different / targetsInBoth_Same only captured items with exact isDifferent() match at snapshot time.
    // InBoth copy blocks (later) only process those two maps. Hence no copy command ever sees these items.
    //
    // Fix: Build dataCopyTargets for selected + hasData + InBoth (Present in both) items from the ensure candidates,
    //      skipping any that are already in the strict _Different/_Same targets (to prevent duplicate Copy execution).
    //      Then run CopyFromTempDirToSourceDirCommand + post-process (UpdateFileTime..., UpdateItemsExportDate..., UpdateItemsDifference to SameContents)
    //      exactly like the InBoth_Same block. This makes the .dat reach SourceDir.
    //
    // Design notes:
    // - Reuses existing Copy command and post-process commands (no new logic duplication).
    // - Does not modify the original 4-bucket snapshots or getItems predicates.
    // - Placement here (after ensure, before InBoth copies and before InFileSystemOnly) is intentional.
    // - Honors all prior user decisions: B (defer to Export click), leave stale files, force ensure+copy for InBoth, lightweight data-only.
    // - For InProjectOnly toggled items: they are usually already handled by the full ExportToTemp+Copy in the InProjectOnly block; the exists() guard in ensure also helps.
    {
        ObjectItemMap dataCopyTargets(m_objectTypesForItemMap);
        foreach (const Model::ObjectType &objectType, ensureDataCandidates.keys())
        {
            const QMap<QString, ObjectItem*> &itemsForType = ensureDataCandidates[objectType];
            for (QMap<QString, ObjectItem*>::const_iterator it = itemsForType.constBegin();
                 it != itemsForType.constEnd(); ++it)
            {
                ObjectItem *item = it.value();
                if (!item) continue;
                if (!item->isSelected()) continue;
                if (!item->hasData()) continue;
                // Must be InBoth (present in project and in SourceDir) for this dedicated data-copy path
                if (item->inProject() != Model::Present || item->inSourceDir() != Model::Present) continue;
                // Skip items that are already covered by the strict InBoth_Different / InBoth_Same snapshots
                // (they will go through the normal InBoth copy blocks below; avoids duplicate work and post-process)
                bool alreadyInDifferent = targetsInBoth_Different.contains(objectType) &&
                                          targetsInBoth_Different[objectType].contains(item->name());
                bool alreadyInSame = targetsInBoth_Same.contains(objectType) &&
                                     targetsInBoth_Same[objectType].contains(item->name());
                if (alreadyInDifferent || alreadyInSame) continue;
                dataCopyTargets[objectType][item->name()] = item;
            }
        }
        if (!dataCopyTargets.isEmpty())
        {
            CopyFromTempDirToSourceDirCommand copyFromTempDirToSourceDir(m_projectContainer, m_application, this);
            copyFromTempDirToSourceDir.execute(&dataCopyTargets);   // note: pass address to match other call sites

            // smart-refresh : post-process (identical to the InBoth_Same block)
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand  updateFileTimeInTempDir (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsDifferenceCommand    updateItemsDifference   (Model::SameContents,                       m_projectContainer, m_application, &m_items, this);
            updateFileTimeInTempDir .execute(&dataCopyTargets);
            updateItemsExportDate   .execute(&dataCopyTargets);
            updateItemsDifference   .execute(&dataCopyTargets);
        }
    }

    // ------------------------------------------------------------------------- 
    // Dedicated data REMOVE path for post-open HasData toggle OFF (InBoth !hasData items)
    // (symmetric to the dataCopyTargets block above; implements the off half of the approved bug-fix plan)
    // ------------------------------------------------------------------------- 
    // User symptom (exact quote from report):
    //   「UIが表示されたときは、HasDataがチェックオンだったテーブルに対して、HasDataチェックをオフにして、Export対象のチェックをオンにして、Exportを実行しても、SourceDirから .dat が消えないんだよね。」
    //
    // Context / root cause (precise references from diagnosis reads + greps):
    // - At UI open, refreshItems runs ExportFromProjectToTempDirCommand + Copy for items that were hasData=true
    //   at that instant (m_tableDataTargets at refresh time). This places .dat in both Temp and Source for InBoth items.
    // - Later, user toggles the HasData checkbox OFF in the tree for such an InBoth item:
    //     setData(HasDataColumn) [~210-222] does: item->setHasData(false); item->setDifferent(Model::Unchecked_OD);
    //     ... saveSettigs();   <--- ONLY model + settings (TableData block rewritten via updateSetting). ZERO file I/O.
    //     (Decision B: keep the checkbox toggle fast; defer real work to Export click.)
    // - executeExport snapshots [463-469]: only the 4 strict buckets (InProjectOnly / InSourceDirOnly / InBoth_Different / InBoth_Same).
    //   Unchecked_OD (0) does NOT match InBoth_Different (==DifferentContents) or InBoth_Same (==SameContents).
    //   It falls into InBoth_NotSame (!= SameContents) which is captured only by the *broad* ensureDataCandidates [485-487].
    // - InSourceDirOnly Delete [~617]: DeleteFromSourceDirCommand is executed only against targetsInSourceDirOnly.
    //   InBoth items are never in that map.
    // - The later InBoth copy blocks [~650+] only iterate the pre-snapshotted targetsInBoth_Different / targetsInBoth_Same.
    //   Toggled-off items are absent from them.
    // - copyFromTempDirToSourceDir [objectprocessor.cpp:76] always does deleteAllFileFromSourceDir (which would remove DataFile)
    //   followed by copy of whatever exists in Temp. But because the off item never enters a Copy command, deleteAll is never
    //   invoked for it in this Export.
    // - ensure loop [532-542] and dataCopyTargets [573-608] both explicitly test "if (item && item->hasData())" / "if (!item->hasData()) continue;".
    // - Result: the .dat that was placed in SourceDir at open time stays there forever for this item, even after off+Export.
    //   (Next full refresh would eventually stop producing it because m_tableDataTargets no longer contains the name.)
    //
    // Fix (user-selected option C, confirmed via askQuestions; symmetric to dataCopyTargets on-path):
    // - Reuse the broad ensureDataCandidates snapshot (already includes InBoth_NotSame for the Unchecked items).
    // - Build a dedicated ObjectItemMap dataRemoveTargets containing exactly the items that are:
    //     selected && !hasData && InBoth (inProject==Present && inSourceDir==Present)
    //     (plus defensive skip of anything that somehow made it into the strict _Different/_Same maps).
    // - For each such item call the new processor->removeDataFromSourceDir(name). The base impl does a targeted
    //     deleteFile(SourceDir, DataFile, ...) only. No structure files, no full deleteAll, no Temp data removal.
    // - Then run the identical post-process triple used by dataCopyTargets (and InBoth_Same):
    //     UpdateFileTimeInTempDirCommand + UpdateItemsExportDateCommand + UpdateItemsDifferenceCommand(SameContents).
    //   This makes the smart-refresh / next UI open see the item as "in sync, data intentionally absent".
    //
    // Why only Source DataFile (not Temp):
    // - Per prior "leave stale" decision (honored for both on-fix and this off-fix): when unchecking we do not want
    //   immediate Temp cleanup at toggle time. At Export time for off we still leave the Temp .dat/.dattmp so that
    //   a subsequent "turn back on + Export" before a full refresh can re-use the last-known data file from Temp
    //   (ensure will see it and skip re-export; dataCopy will copy it). The verification steps in the plan explicitly
    //   expect "TempDir の .dat / .dattmp は残っていること (leave stale)".
    // - Re-copy protection for *this* Export is provided by the fact that toggled-off items are Unchecked_OD and
    //   therefore absent from targetsInBoth_Different/Same and from dataCopyTargets (the hasData filter).
    //   They also do not match InSourceDirOnly. So no later copyFromTemp... will be invoked for them in this run.
    //
    // Scope / decisions honored (all previous user choices carried forward):
    // - B (batch at Export click, not on toggle) – setData remains a pure model/settings operation.
    // - leave stale in TempDir for the off case.
    // - Yes, force the removal for InBoth items that were on at open and toggled off later.
    // - C (dedicated removeDataFromSourceDir + dataRemoveTargets map) rather than A (force through Copy) or B (extend Delete cmd).
    //   This keeps structure files completely untouched (the whole point of a pure data-off).
    // - No modifications to the 4-bucket snapshots, getItems predicates (~820), InBoth_NotSame block, or any command objects.
    // - TableDef and OdbcTable both supported automatically (the hasData flag + m_tableDataTargets already work for both;
    //   the new virtual is on the base ObjectProcessor).
    //
    // Placement: right after the dataCopyTargets block (and its post-process), before the InSourceDirOnly block
    // and before the two InBoth copy blocks. This is the natural "post-ensure data maintenance" window.
    //
    // The early ensure loop (hasData only) is left as-is; we do not inject !hasData calls there because that would
    // delete Source data "early" for candidates that may include non-InBoth, and the dedicated block below is the
    // documented place for the remove + post-process. The current ordering + filters are sufficient.
    {
        ObjectItemMap dataRemoveTargets(m_objectTypesForItemMap);
        foreach (const Model::ObjectType &objectType, ensureDataCandidates.keys())
        {
            const QMap<QString, ObjectItem*> &itemsForType = ensureDataCandidates[objectType];
            for (QMap<QString, ObjectItem*>::const_iterator it = itemsForType.constBegin();
                 it != itemsForType.constEnd(); ++it)
            {
                ObjectItem *item = it.value();
                if (!item) continue;
                if (!item->isSelected()) continue;
                if (item->hasData()) continue;   // the off (toggled after open) case
                // Must be InBoth (present in project and in SourceDir) – these are the ones that actually have
                // a .dat in SourceDir that needs to be removed.
                if (item->inProject() != Model::Present || item->inSourceDir() != Model::Present) continue;
                // Skip items that are already covered by the strict InBoth_Different / InBoth_Same snapshots
                // (defensive; a !hasData item should not be in them, but cheap to check).
                bool alreadyInDifferent = targetsInBoth_Different.contains(objectType) &&
                                          targetsInBoth_Different[objectType].contains(item->name());
                bool alreadyInSame = targetsInBoth_Same.contains(objectType) &&
                                     targetsInBoth_Same[objectType].contains(item->name());
                if (alreadyInDifferent || alreadyInSame) continue;
                dataRemoveTargets[objectType][item->name()] = item;
            }
        }
        if (!dataRemoveTargets.isEmpty())
        {
            // Manually drive the data-only removal (no RemoveXXXCommand exists for this narrow case).
            // removeDataFromSourceDir only deletes the DataFile from SourceDir; Temp data is left (leave stale).
            foreach (const Model::ObjectType &objectType, dataRemoveTargets.keys())
            {
                ObjectProcessor *processor = m_projectContainer->operator[](objectType);
                if (!processor)
                    continue;
                const QMap<QString, ObjectItem*> &itemsForType = dataRemoveTargets[objectType];
                for (QMap<QString, ObjectItem*>::const_iterator it = itemsForType.constBegin();
                     it != itemsForType.constEnd(); ++it)
                {
                    ObjectItem *item = it.value();
                    if (item)
                        processor->removeDataFromSourceDir( item->name() );
                }
            }

            // smart-refresh : post-process (identical to dataCopyTargets block and the InBoth_Same block)
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand  updateFileTimeInTempDir (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsDifferenceCommand    updateItemsDifference   (Model::SameContents,                       m_projectContainer, m_application, &m_items, this);
            updateFileTimeInTempDir .execute(&dataRemoveTargets);
            updateItemsExportDate   .execute(&dataRemoveTargets);
            updateItemsDifference   .execute(&dataRemoveTargets);
        }
    }

    // for InFileSytemOnly
    {
        {
            ObjectItemMap *targets = &targetsInSourceDirOnly;

            DeleteFromSourceDirCommand deleteFromSourceDir(m_projectContainer, m_application, this);
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

            CopyFromTempDirToSourceDirCommand copyFromTempDirToSourceDir(m_projectContainer, m_application, this);
            copyFromTempDirToSourceDir.execute(targets);                                  // InBoth_Different :       : Dirty SourceDir  : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand  updateFileTimeInTempDir (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsDifferenceCommand    updateItemsDifference   (Model::SameContents,                       m_projectContainer, m_application, &m_items, this);
            updateFileTimeInTempDir .execute(targets);     // for smart-refresh, we need to update filetime which rollbacked in smart-refresh process
            updateItemsExportDate   .execute(targets);     // for smart-refresh, update exportDate too.
            updateItemsDifference   .execute(targets);     // set isDifferent flag to SameContents

        }

        // for InBoth_Same
        {
            ObjectItemMap *targets = &targetsInBoth_Same;

            // Note: ensureDataInTempDir (for post-toggle hasData items) was already invoked above
            // for the broad ensureDataCandidates snapshot. This Copy will now see the .dat if the item
            // had hasData toggled on after UI open (InBoth case). User decision: force for InBoth.
            CopyFromTempDirToSourceDirCommand copyFromTempDirToSourceDir(m_projectContainer, m_application, this);
            copyFromTempDirToSourceDir.execute(targets);    // InBoth_Same      :       : Dirty SourceDir  : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand  updateFileTimeInTempDir (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand    updateItemsExportDate   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            updateFileTimeInTempDir .execute(targets);  // we need to update filetime which rollbacked in smart-refresh process
            updateItemsExportDate   .execute(targets);  // update exportDate too.

        }
    }

    return true;
}


bool ObjectModel::executeImport()
{
    m_projectContainer->initialize(m_application);

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //:execute

    if (!checkProjectState())
        return false;

    // first of all, we have to retreive target items for all states
    // because each process changes the states of items
    bool selectedOnly = true;

    ObjectItemMap targetsInProjectOnly(m_objectTypesForItemMap);
    ObjectItemMap targetsInSourceDirOnly(m_objectTypesForItemMap);
    ObjectItemMap targetsInBoth_Different(m_objectTypesForItemMap);
    ObjectItemMap targetsInBoth_Same(m_objectTypesForItemMap);

    getItems(&targetsInProjectOnly,    Model::InProjectOnly,    selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInSourceDirOnly,  Model::InSourceDirOnly,  selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Different, Model::InBoth_Different, selectedOnly, false/*modifiedOnly*/);
    getItems(&targetsInBoth_Same,      Model::InBoth_Same,      selectedOnly, false/*modifiedOnly*/);


    // for InProjectOnly
    {
        {
            ObjectItemMap *targets = &targetsInProjectOnly;

            DeleteFromProjectCommand deleteFromProject(m_projectContainer, m_application, this);
            deleteFromProject.execute(targets); // InProjectOnly    : BLOCK : Dirty Project : need one-more step? like confirm

            // smart-refresh : post-process
            deleteItems(targets);   // delete item from model
        }
    }

    // for InFileSytemOnly
    {
        {
            ObjectItemMap *targets = &targetsInSourceDirOnly;

            CopyFromSourceDirToTempDirCommand   copyFromSourceDirToTempDir  (m_projectContainer, m_application, this);
            DesanitizeTempDirCommand            desanitizeTempDir           (m_projectContainer, m_application, this);
            ImportFromTempDirToProjectCommand   importFromTempDirToProject  (m_projectContainer, m_application, this);
            copyFromSourceDirToTempDir  .execute(targets);  // InFileSytemOnly  :       :               :
            desanitizeTempDir           .execute(targets);  // InFileSytemOnly  :       :               :
            importFromTempDirToProject  .execute(targets);  // InSourceDirOnly : BLOCK : Dirty Project : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand                  updateFileTimeInTempDir                 (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand                    updateItemsExportDate                   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsInProjectCommand                     updateItemsInProject                    (Model::Present,                            m_projectContainer, m_application, &m_items, this);
            UpdateItemsDifferenceCommand                    updateItemsDifference                   (Model::SameContents,                       m_projectContainer, m_application, &m_items, this);
            UpdateItemsCreateUpdateDateFromProjectCommand   updateItemsCreateUpdateDateFromProject  (                                           m_projectContainer, m_application, &m_items, this);
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

            CopyFromSourceDirToTempDirCommand   copyFromSourceDirToTempDir  (m_projectContainer, m_application, this);
            DesanitizeTempDirCommand            desanitizeTempDir           (m_projectContainer, m_application, this);
            ImportFromTempDirToProjectCommand   importFromTempDirToProject  (m_projectContainer, m_application, this);
            copyFromSourceDirToTempDir  .execute(targets);  // InBoth_Different :       :               :
            desanitizeTempDir           .execute(targets);  // InBoth_Different :       :               :
            importFromTempDirToProject  .execute(targets);  // InBoth_Different : BLOCK : Dirty Project : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand                  updateFileTimeInTempDir                 (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand                    updateItemsExportDate                   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsDifferenceCommand                    updateItemsDifference                   (Model::SameContents,                       m_projectContainer, m_application, &m_items, this);
            UpdateItemsCreateUpdateDateFromProjectCommand   updateItemsCreateUpdateDateFromProject  (                                           m_projectContainer, m_application, &m_items, this);
            updateFileTimeInTempDir                 .execute(targets);  // for smart refresh, update filetime of TempFile if imported
            updateItemsExportDate                   .execute(targets);  // for smart-refresh, update exportDate too.
            updateItemsDifference                   .execute(targets);  // set isDifferent flag to SameContents
            updateItemsCreateUpdateDateFromProject  .execute(targets);  // set create/updateDate from Access Object

        }
        // for InBoth_Same
        {
            ObjectItemMap *targets = &targetsInBoth_Same;

            CopyFromSourceDirToTempDirCommand   copyFromSourceDirToTempDir  (m_projectContainer, m_application, this);
            DesanitizeTempDirCommand            desanitizeTempDir           (m_projectContainer, m_application, this);
            ImportFromTempDirToProjectCommand   importFromTempDirToProject  (m_projectContainer, m_application, this);
            copyFromSourceDirToTempDir  .execute(targets);  // InBoth_Same      :       :               :
            desanitizeTempDir           .execute(targets);  // InBoth_Same      :       :               :
            importFromTempDirToProject  .execute(targets);  // InBoth_Same      : BLOCK : Dirty Project : need one-more step? like confirm


            // smart-refresh : post-process
            QDateTime currentTime = QDateTime::currentDateTime();
            UpdateFileTimeInTempDirCommand                  updateFileTimeInTempDir                 (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, this);
            UpdateItemsExportDateCommand                    updateItemsExportDate                   (currentTime, Model::AllDifferenceTypes,    m_projectContainer, m_application, &m_items, this);
            UpdateItemsCreateUpdateDateFromProjectCommand   updateItemsCreateUpdateDateFromProject  (                                           m_projectContainer, m_application, &m_items, this);
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

    ObjectItemMap targets(m_objectTypesForItemMap);
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

    ObjectItemMap targets(m_objectTypesForItemMap);
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
    foreach( const Model::ObjectType &objectType, m_itemMap->keys() )
    {
       foreach( const ObjectItem *item, m_itemMap->value( objectType ).values() )
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
        emit currentObjectTypeChanged(objectType);

        QList<ObjectItem*> items = allTargets->value( objectType ).values();
        ProgressNotifier subProgress(mainProgress.type(), items.count(), this);
        for (QList<ObjectItem*>::iterator it = items.begin() ; it != items.end() ; ++it )
        {
            subProgress.next();

            int row = m_items.indexOf( (*it) );
            beginRemoveRows( QModelIndex(), row, row );
            m_itemMap->operator []( objectType ).remove( (*it)->name() );
            m_items.removeAll( (*it) );
            delete (*it);
            endRemoveRows();
        }
    }
}



void ObjectModel::reloadAndMergeItems()
{
    m_projectContainer->initialize(m_application);

    // FIXME: conjunction with BLOCKING and non-blocking, can be async ?

    // loading items more smart

    QList<ObjectItem*> itemsFromProject;
    QList<ObjectItem*> itemsFromSourceDir;

    LoadItemsFromProjectCommand     loadItemsFromProject    (m_projectContainer, m_application, &itemsFromProject, this);
    LoadItemsFromSourceDirCommand   loadItemsFromSourceDir  (m_projectContainer, m_application, &itemsFromSourceDir, this);
    loadItemsFromProject    .execute( NULL );
    loadItemsFromSourceDir  .execute( NULL );


    // we need to do here for ..
    //  * delete item if not exist in both
    //  * add item if not exist in m_items/m_mapItems
    //  * update values


    // first of all, we merge both items into one
    QList<ObjectItem*> items;
    ObjectItemMap mapItems(m_objectTypesForItemMap);

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
            m_itemMap->operator []( (*it)->objectType() ).remove( (*it)->name() );
            it = m_items.erase( it );
            --it; // back one
            delete item;
        }
    }

    // add new-item to member  if not exist in member
    for ( QList<ObjectItem*>::iterator it = items.begin(); it != items.end(); ++it )
    {
        if ( !m_itemMap->value( (*it)->objectType() ).contains( (*it)->name() ) )
        {
            ObjectItem *item = new ObjectItem( (*it) , this);
            m_itemMap->operator []( (*it)->objectType() ).insert( (*it)->name(), item );
            m_items.append( item );
        }
    }

    // and now, we merge  from new-item to local-item
    for ( QList<ObjectItem*>::iterator it = items.begin(); it != items.end(); ++it )
    {
        if ( m_itemMap->value( (*it)->objectType() ).contains( (*it)->name() ) )
        {
            ObjectItem *item = m_itemMap->value( (*it)->objectType() ).value( (*it)->name() );

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

