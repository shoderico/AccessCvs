#include "objectproxymodel.h"

#include "objectmodel.h"

ObjectProxyModel::ObjectProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool ObjectProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // Returns true if the value of the item referred to by the given index source_left is less than the value of the item referred to by the given index source_right, otherwise returns false.

    // left is less than right : return true
    // left < right : return true

    switch (source_left.column())
    {
        case ObjectModel::NameColumn:
        {
            int row_left = source_left.row();
            int row_right = source_right.row();

            const QModelIndex index_name_left  = sourceModel()->index(row_left,  ObjectModel::NameColumn, QModelIndex());
            const QModelIndex index_name_right = sourceModel()->index(row_right, ObjectModel::NameColumn, QModelIndex());
            const QModelIndex index_type_left  = sourceModel()->index(row_left,  ObjectModel::ObjectTypeColumn, QModelIndex());
            const QModelIndex index_type_right = sourceModel()->index(row_right, ObjectModel::ObjectTypeColumn, QModelIndex());

            QVariant value_name_left  = sourceModel()->data(index_name_left);
            QVariant value_name_right = sourceModel()->data(index_name_right);
            QVariant value_type_left  = sourceModel()->data(index_type_left);
            QVariant value_type_right = sourceModel()->data(index_type_right);

            if (value_type_left.toInt() != value_type_right.toInt())
                return ( value_type_left.toInt() < value_type_right.toInt() );

            return QString::localeAwareCompare(value_name_left.toString(), value_name_right.toString()) < 0;

        }
            break;

        default:
            return QSortFilterProxyModel::lessThan(source_left, source_right);
    }

    return false;
}

