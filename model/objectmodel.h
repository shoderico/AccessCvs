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
        InProjectColumn,
        InFileSystemColumn,
        DifferentColumn,
        CreateDateColumn,
        UpdateDateColumn,
        ExportDateColumn,
        ObjectTypeColumn,
        ColumnCount
    };

    // TODO: split Model and Manager

    //----------------------------------------------------------------------------------------------------------------------
    // interface procedures

    void setApplication(Access::Application *application);

    void prepareInit();
    void prepareClone();
    void prepareCommit();
    void prepareMerge();


    bool refreshItems();

    bool executeExport();
    bool executeImport();




    // items getter
    enum ItemsType
    {
        NoItems          = 0,
        InBoth           = 1,
        InBoth_Different = 2,
        InBoth_Same      = 4,
        InProjectOnly    = 8,
        InFileSystemOnly = 16,
        AllItems         = 31,
    };
    Q_DECLARE_FLAGS(ItemsTypes, ItemsType)
    // TODO: define select-state ( selected, not-select, both)
    // TODO: extend getItems() to specify select state
    // TODO: can be non-public ?
    void getItems(ObjectItems *pItems, ItemsTypes itemsType, bool selectedOnly = true, bool modifiedOnly = false) const;

    void selectItemsForProcess(bool resetSelection = true);
    void selectItems(ItemsTypes itemsType, bool resetSelection = true);

    void assumeItemsTheSameByFileTime();



    //----------------------------------------------------------------------------------------------------------------------
    // internal procedures

    // load and build-up model-item from object/file
    void loadItemFromProject();                                 //                                                      : BLOCK
    void loadItemFromFileSystem();                              //                                                      :

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
        DeleteFromProjectProcess
    };
signals:
    void progressStart(int type, int count);
    void progressEnd(int type);
    void progressChange(int type, int position);


private:
    QList<ObjectItem*> m_items;
    ObjectItems m_mapItems;
    Access::Application *m_application;

    void addItem( ObjectItem* item);
    void clearTempDir();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectModel::ItemsTypes)

#endif // OBJECTMODEL_H
