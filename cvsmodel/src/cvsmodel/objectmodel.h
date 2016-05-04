#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include "cvsmodel_global.h"
#include "cvsmodel_const.h"

#include <QAbstractItemModel>

#include "objectitemmap.h"

namespace Access {
class Application;
}


class CVSMODEL_SHARED_EXPORT ObjectModel : public QAbstractItemModel
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


    int selectedRowCount() const;

    void getItems(ObjectItemMap *pItems, Model::ItemsTypes itemsType, bool selectedOnly /*= true*/, bool modifiedOnly /*= false*/) const;
    void getItems(ObjectItemMap *pItems, Model::ItemsTypes itemsType, Model::SelectObjectTypes objectTypes, bool selectedOnly /*= true*/, bool modifiedOnly /*= false*/) const;

    void selectItemsForProcess(bool selected, bool resetSelection /*= true*/);
    void selectItems(Model::ItemsTypes itemsType, bool selected, bool resetSelection /*= true*/);
    void selectItemsByObjectType(Model::SelectObjectTypes objectTypes, bool selected, bool resetSelection /*= true*/);
    void emitSelectionChanged();


    void deleteItems(ObjectItemMap *allTargets);



    //----------------------------------------------------------------------------------------------------------------------
    // internal procedures

    // load and build-up model-item from object/file
    void reloadAndMergeItems();


public slots:
    void itemDataChanged(int rowStart, int rowEnd, int columnStart, int columnEnd);

    //----------------------------------------------------------------------------------------------------------------------
    // progress notification
signals:
    void progressStart(int type, int count);
    void progressEnd(int type);
    void progressChange(int type, int position);

    void selectionChanged(int objectTypes);

private:
    QList<ObjectItem*> m_items;
    ObjectItemMap m_mapItems;
    Access::Application *m_application;

    void mergeItemProperties(ObjectItem* itemSrc, ObjectItem* itemDst);
};

#endif // OBJECTMODEL_H
