#ifndef CHECKBOXITEMDELEGETE_H
#define CHECKBOXITEMDELEGETE_H

#include <QStyledItemDelegate>

class CheckBoxItemDelegete : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxItemDelegete(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CHECKBOXITEMDELEGETE_H
