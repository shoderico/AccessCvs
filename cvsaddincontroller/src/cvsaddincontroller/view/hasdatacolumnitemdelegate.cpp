#include "hasdatacolumnitemdelegate.h"

#include "cvsmodel/objectmodel.h"

#include "pch.hpp"

HasDataColumnItemDelegate::HasDataColumnItemDelegate(QObject *parent) : BoolCheckBoxItemDelegate(parent)
{

}

bool HasDataColumnItemDelegate::paintRequired(const QModelIndex &index) const
{
    // table only (TableDef or OdbcTable)
    const QModelIndex index_type = index.model()->index( index.row(), Model::ObjectTypeColumn );
    int objectType = index.model()->data(index_type).toInt();
    if (objectType != Model::TableDef && objectType != Model::OdbcTable)
        return false;
    return true;
}

bool HasDataColumnItemDelegate::editAvaiable(QAbstractItemModel *model, const QModelIndex &index)
{
    // table only (TableDef or OdbcTable)
    QModelIndex index_type = model->index( index.row(), Model::ObjectTypeColumn );
    int objectType = model->data(index_type).toInt();
    if (objectType != Model::TableDef && objectType != Model::OdbcTable)
        return false;
    return true;
}

