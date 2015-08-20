#ifndef BOOLCHECKBOXITEMDELEGATE_H
#define BOOLCHECKBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

class BoolCheckBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    BoolCheckBoxItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // BOOLCHECKBOXITEMDELEGATE_H
