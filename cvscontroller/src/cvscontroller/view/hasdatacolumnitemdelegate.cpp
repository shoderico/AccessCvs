#include "hasdatacolumnitemdelegate.h"

#include "cvsmodel/objectmodel.h"

HasDataColumnItemDelegate::HasDataColumnItemDelegate(QObject *parent) : BoolCheckBoxItemDelegate(parent)
{

}

bool HasDataColumnItemDelegate::paintRequired(const QModelIndex &index) const
{
    // table only
    const QModelIndex index_type = index.model()->index( index.row(), ObjectModel::ObjectTypeColumn );
    int objectType = index.model()->data(index_type).toInt();
    if (objectType != Model::TableDef)
        return false;
    return true;
}

bool HasDataColumnItemDelegate::editAvaiable(QAbstractItemModel *model, const QModelIndex &index)
{
    // table only
    QModelIndex index_type = model->index( index.row(), ObjectModel::ObjectTypeColumn );
    int objectType = model->data(index_type).toInt();
    if (objectType != Model::TableDef)
        return false;
    return true;
}

