#ifndef OBJECTPROXYMODEL_H
#define OBJECTPROXYMODEL_H

#include "cvsmodel_global.h"
#include "cvsmodel_const.h"

#include <QObject>
#include <QSortFilterProxyModel>

class ObjectModel;

class CVSMODEL_SHARED_EXPORT ObjectProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ObjectProxyModel(QObject *parent = 0);

    void setSourceModel(QAbstractItemModel *sourceModel);

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    void setFilterShowSelectedOnly(const bool selectedOnly);
    void setFilterShowObjectType(const int objectTypes);

    bool isShowSelectedOnly() const;
    int showObjectType() const;

signals:
    void showSelectedOnlyChanged(bool selectedOnly);
    void showObjectTypeChanged(int objectTypes);

public slots:

private:
    QMap<int, bool> m_showObjectTypes;
    bool m_showSelectedOnly;
    QList<Model::ObjectType> m_objectTypes;
    ObjectModel *m_model;

};

#endif // OBJECTPROXYMODEL_H
