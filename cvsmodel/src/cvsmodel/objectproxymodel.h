#ifndef OBJECTPROXYMODEL_H
#define OBJECTPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class ObjectProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ObjectProxyModel(QObject *parent = 0);

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    void setFilterShowSelectedOnly(const bool selectedOnly);
    void setFilterShowObjectType(const int objectTypes);

signals:

public slots:

private:
    QMap<int, bool> m_showObjectTypes;
    bool m_showSelectedOnly;

};

#endif // OBJECTPROXYMODEL_H
