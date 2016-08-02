#ifndef HASDATACOLUMNITEMDELEGATE_H
#define HASDATACOLUMNITEMDELEGATE_H

#include "view/boolcheckboxitemdelegate.h"

class HasDataColumnItemDelegate : public BoolCheckBoxItemDelegate
{
    Q_OBJECT
public:
    explicit HasDataColumnItemDelegate(QObject *parent = 0);

    virtual bool paintRequired(const QModelIndex &index) const;
    virtual bool editAvaiable(QAbstractItemModel *model, const QModelIndex &index);
};

#endif // HASDATACOLUMNITEMDELEGATE_H
