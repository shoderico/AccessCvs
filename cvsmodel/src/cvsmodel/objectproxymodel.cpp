#include "objectproxymodel.h"

#include "objectmodel.h"
#include "projectcontainer.h"

ObjectProxyModel::ObjectProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_showSelectedOnly(false)
{
}

void ObjectProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);

    m_model = static_cast<ObjectModel *>(sourceModel);

    m_objectTypes = m_model->projectContainer()->objectTypes();

    foreach ( Model::ObjectType objectType, m_objectTypes )
        m_showObjectTypes[ objectType ] = true;
}

bool ObjectProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // Returns true if the value of the item referred to by the given index source_left
    // is less than the value of the item referred to by the given index source_right,
    // otherwise returns false.

    // left is less than right : return true
    // left < right : return true

    switch (source_left.column())
    {
        case Model::NameColumn:
        {
            int row_left = source_left.row();
            int row_right = source_right.row();

            const QModelIndex index_name_left  = sourceModel()->index(row_left,  Model::NameColumn, QModelIndex());
            const QModelIndex index_name_right = sourceModel()->index(row_right, Model::NameColumn, QModelIndex());
            const QModelIndex index_type_left  = sourceModel()->index(row_left,  Model::ObjectTypeColumn, QModelIndex());
            const QModelIndex index_type_right = sourceModel()->index(row_right, Model::ObjectTypeColumn, QModelIndex());

            QVariant value_name_left  = sourceModel()->data(index_name_left);
            QVariant value_name_right = sourceModel()->data(index_name_right);
            QVariant value_type_left  = sourceModel()->data(index_type_left);
            QVariant value_type_right = sourceModel()->data(index_type_right);
            int order_left  = m_objectTypes.indexOf( (Model::ObjectType)value_type_left.toInt() );
            int order_right = m_objectTypes.indexOf( (Model::ObjectType)value_type_right.toInt() );

            if (order_left != order_right)
                return ( order_left < order_right );

            return QString::localeAwareCompare(value_name_left.toString(), value_name_right.toString()) < 0;

        }
            break;

        default:
            return QSortFilterProxyModel::lessThan(source_left, source_right);
    }

    return false;
}

bool ObjectProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index_type = sourceModel()->index( source_row, Model::ObjectTypeColumn, source_parent );
    QVariant value_type = sourceModel()->data(index_type);

    if ( !m_showObjectTypes[ value_type.toInt() ] )
        return false;

    if ( m_showSelectedOnly )
    {
        const QModelIndex index_name = sourceModel()->index( source_row, Model::NameColumn, source_parent );

        QVariant value_selected = sourceModel()->data( index_name, Qt::CheckStateRole );
        if ( value_selected.toInt() != Qt::Checked )
            return false;
    }

    return true;
}

void ObjectProxyModel::setFilterShowSelectedOnly(const bool selectedOnly)
{
    m_showSelectedOnly = selectedOnly;
    invalidateFilter();
    emit showSelectedOnlyChanged(selectedOnly);
}

void ObjectProxyModel::setFilterShowObjectType(const int objectTypes)
{
    foreach(int objectType, m_showObjectTypes.keys() )
    {
        int selectObjectType = objectType; // SelectObjectType is the same as ObjectType
        m_showObjectTypes[ objectType ] = ( objectTypes & selectObjectType ) ;
    }

    invalidateFilter();
    emit showObjectTypeChanged( showObjectType() );
}

bool ObjectProxyModel::isShowSelectedOnly() const
{
    return m_showSelectedOnly;
}

int ObjectProxyModel::showObjectType() const
{
    int objectTypes = 0;

    foreach(int objectType, m_showObjectTypes.keys() )
    {
        int selectObjectType = objectType; // SelectObjectType is the same as ObjectType
        objectTypes |= m_showObjectTypes[ objectType ] ? selectObjectType : 0;
    }

    return objectTypes;
}

