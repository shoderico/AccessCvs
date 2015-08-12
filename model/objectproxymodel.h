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

signals:

public slots:
};

#endif // OBJECTPROXYMODEL_H
