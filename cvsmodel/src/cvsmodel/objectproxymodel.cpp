#include "objectproxymodel.h"

#include "objectmodel.h"

ObjectProxyModel::ObjectProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_showSelectedOnly(false)
{
    m_showObjectTypes[ Model::TableDef ] = true;
    m_showObjectTypes[ Model::Query ] = true;
    m_showObjectTypes[ Model::Form ] = true;
    m_showObjectTypes[ Model::Report ] = true;
    m_showObjectTypes[ Model::Macro ] = true;
    m_showObjectTypes[ Model::Module ] = true;
    m_showObjectTypes[ Model::Reference ] = true;
    m_showObjectTypes[ Model::ProjectFile ] = true;
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

bool ObjectProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index_type = sourceModel()->index( source_row, ObjectModel::ObjectTypeColumn, source_parent );
    QVariant value_type = sourceModel()->data(index_type);

    if ( !m_showObjectTypes[ value_type.toInt() ] )
        return false;

    if ( m_showSelectedOnly )
    {
        const QModelIndex index_name = sourceModel()->index( source_row, ObjectModel::NameColumn, source_parent );

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
}

void ObjectProxyModel::setFilterShowObjectType(const int objectTypes)
{
    m_showObjectTypes[ Model::TableDef  ] = ( objectTypes & ObjectModel::TableObjectType );
    m_showObjectTypes[ Model::Query     ] = ( objectTypes & ObjectModel::QueryObjectType );
    m_showObjectTypes[ Model::Form      ] = ( objectTypes & ObjectModel::FormObjectType ) ;
    m_showObjectTypes[ Model::Report    ] = ( objectTypes & ObjectModel::ReportObjectType ) ;
    m_showObjectTypes[ Model::Macro     ] = ( objectTypes & ObjectModel::MacroObjectType );
    m_showObjectTypes[ Model::Module    ] = ( objectTypes & ObjectModel::ModuleObjectType );
    m_showObjectTypes[ Model::Reference ] = ( objectTypes & ObjectModel::ReferenceObjectType ) ;
    m_showObjectTypes[ Model::ProjectFile ] = ( objectTypes & ObjectModel::ProjectFileType ) ;
    invalidateFilter();
}

