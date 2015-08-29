#ifndef BOOLCHECKBOXITEMDELEGATE_H
#define BOOLCHECKBOXITEMDELEGATE_H

#include "view_global.h"

#include <QStyledItemDelegate>

class VIEWSHARED_EXPORT BoolCheckBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    BoolCheckBoxItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    virtual bool paintRequired(const QModelIndex &index) const;
    virtual bool editAvaiable(QAbstractItemModel *model, const QModelIndex &index);
};

#endif // BOOLCHECKBOXITEMDELEGATE_H
