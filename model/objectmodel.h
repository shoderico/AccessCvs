#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractItemModel>

#include "objectitem.h"
#include "objectitems.h"

namespace Access {
class Application;
}


class ObjectModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ObjectModel(QObject * parent = 0);

    //----------------------------------------------------------------------------------------------------------------------
    // QAbstractItemModel overrides

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    enum Column
    {
        NameColumn = 0,
        HasDataColumn,
        InProjectColumn,
        InFileSystemColumn,
        DifferentColumn,
        CreateDateColumn,
        UpdateDateColumn,
        ExportDateColumn,
        ObjectTypeColumn,
        ColumnCount
    };

    void saveSettigs();

    // TODO: split Model and Manager

    //----------------------------------------------------------------------------------------------------------------------
    // interface procedures

    void setApplication(Access::Application *application);

    void prepareInit();
    void prepareClone();
    void prepareCommit();
    void prepareMerge();

    bool clearItemsCache();

    bool refreshItems();

    bool executeExport();
    bool executeImport();

    bool checkProjectState();




    // items getter
    enum ItemsType
    {
      //NoItems          = 0,
        InBoth           = 1,
        InBoth_Different = 2,
        InBoth_Same      = 4,
        InBoth_NotSame   = 8,
        InProjectOnly    = 16,
        InFileSystemOnly = 32,
        AllItems         = 63,
    };
    Q_DECLARE_FLAGS(ItemsTypes, ItemsType)

    enum SelectObjectType
    {
      //NoObjectTypes = 0,
        TableObjectType = 1,
        QueryObjectType = 2,
        FormObjectType = 4,
        ReportObjectType = 8,
        MacroObjectType = 16,
        ModuleObjectType = 32,
        ReferenceObjectType = 64,
        AllObjectTypes      = 127,
    };
    Q_DECLARE_FLAGS(SelectObjectTypes, SelectObjectType)

    enum ObjectDifferenceType
    {
        SameContentsType = 1,
        DifferentContentsTypes = 2,
        AllDifferenceTypes = 3,
    };
    Q_DECLARE_FLAGS(ObjectDifferenceTypes, ObjectDifferenceType)

    void getItems(ObjectItems *pItems, ItemsTypes itemsType, bool selectedOnly /*= true*/, bool modifiedOnly /*= false*/) const;
    void getItems(ObjectItems *pItems, ItemsTypes itemsType, SelectObjectTypes objectTypes, bool selectedOnly /*= true*/, bool modifiedOnly /*= false*/) const;

    void selectItemsForProcess(bool selected, bool resetSelection /*= true*/);
    void selectItems(ItemsTypes itemsType, bool selected, bool resetSelection /*= true*/);
    void selectItemsByObjectType(SelectObjectTypes objectTypes, bool selected, bool resetSelection /*= true*/);

    void updateItemsExportDate(ObjectItems *allTargets, const QDateTime &exportDate, const ObjectDifferenceTypes differenceTypes);
    void updateItemsInProject(ObjectItems *allTargets, Model::ObjectExistence existence);
    void updateItemsInFileSystem(ObjectItems *allTargets, Model::ObjectExistence existence);
    void updateItemsDifference(ObjectItems *allTargets, Model::ObjectDifference difference);
    void updateItemsDifferenceByFileTime(ObjectItems *allTargets);
    void updateItemsDifferenceAsIs(ObjectItems *allTargets);
    void updateItemsCreateUpdateDateFromProject(ObjectItems *allTargets);


    void updateFileTimeInTempDirByExportDate(ObjectItems *allTargets, const ObjectDifferenceTypes differenceTypes);
    void updateFileTimeInTempDir(ObjectItems *allTargets, const QDateTime &fileTime, const ObjectDifferenceTypes differenceTypes);

    void deleteItems(ObjectItems *allTargets);



    //----------------------------------------------------------------------------------------------------------------------
    // internal procedures

    // load and build-up model-item from object/file
    void loadItemFromProject(QList<ObjectItem*> *items);        //                                                      : BLOCK
    void loadItemFromFileSystem(QList<ObjectItem*> *items);     //                                                      :
    void reloadAndMergeItems();

    // import/export object
    void exportFromProjectToTempDir(ObjectItems *allTargets);   // InBoth           , InProjectOnly ,                   : BLOCK
    void importFromTempDirToProject(ObjectItems *allTargets);   // InBoth_DiffOnly  ,               , InFileSystemOnly  : BLOCK : Dirty Project

    // copy files between directories
    void copyFromTempDirToFileSystem(ObjectItems *allTargets);  // InBoth:DiffOnly  , InProjectOnly ,                   :       : Dirty FileSystem
    void copyFromFileSystemToTempDir(ObjectItems *allTargets);  // InBoth_DiffOnly  ,               , InFileSystemOnly  :

    // sanitize/de-sanitize files
    void sanitizeTempDir(ObjectItems *allTargets);              // InBoth           , InProjectOnly ,                   :
    void desanitizeTempDir(ObjectItems *allTargets);            // InBoth_DiffOnly  ,               , InFileSystemOnly  :

    // compare files and update model-item status
    void compareTempDir(ObjectItems *allTargets);               // InBoth           ,               ,                   :

    // delete object/file
    void deleteFromFileSystem(ObjectItems *allTargets);         //                  ,               , InFileSystemOnly  :       : Dirty FileSytem
    void deleteFromProject(ObjectItems *allTargets);            //                  , InProjectOnly ,                   : BLOCK : Dirty Project

    void deleteFromTempDir(ObjectItems *allTargets);

    //----------------------------------------------------------------------------------------------------------------------
    // progress notification
public:
    enum ProcessType
    {
        RefreshProcess,
        ExportProcess,
        ImportProcess,

        LoadItemFromProjectProcess,
        LoadItemFromFileSystemProcess,

        ExportFromProjectToTempDirProcess,
        ImportFromTempDirToProjectProcess,

        CopyFromTempDirToFileSystemProcess,
        CopyFromFileSystemToTempDirProcess,

        SanitizeTempDirProcess,
        DesanitizeTempDirProcess,
        CompareTempDirProcess,

        DeleteFromFileSystemProcess,
        DeleteFromProjectProcess,

        DeleteFromTempDirProcess,

        UpdateItemsDifferenceByFileTimeProcess,
        UpdateFileTimeInTempDirByExportDateProcess,
        UpdateItemsExportDateProcess,
        UpdateFileTimeInTempDirProcess,

        UpdateItemsInProjectProcess,
        UpdateItemsInFileSystemProcess,
        UpdateItemsDifferenceProcess,
        UpdateItemsDifferenceAsIsProcess,
        DeleteItemsProcess,
        UpdateItemsCreateUpdateDateFromProjectProcess,
    };
signals:
    void progressStart(int type, int count);
    void progressEnd(int type);
    void progressChange(int type, int position);


private:
    QList<ObjectItem*> m_items;
    ObjectItems m_mapItems;
    Access::Application *m_application;

    void mergeItemProperties(ObjectItem* itemSrc, ObjectItem* itemDst);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectModel::ItemsTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectModel::SelectObjectTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectModel::ObjectDifferenceTypes)

#endif // OBJECTMODEL_H
