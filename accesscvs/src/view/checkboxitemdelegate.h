#ifndef CHECKBOXITEMDELEGATE_H
#define CHECKBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

class CheckBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CHECKBOXITEMDELEGATE_H
