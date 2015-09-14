#ifndef CHECKBOXITEMDELEGATE_H
#define CHECKBOXITEMDELEGATE_H

#include "view_global.h"

#include <QStyledItemDelegate>

class VIEWSHARED_EXPORT CheckBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxItemDelegate(QObject *parent = 0);
    explicit CheckBoxItemDelegate(const int stateNoChange, const int stateOn, const int stateOff, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setDataValues(const int stateNoChange, const int stateOn, const int stateOff);

private:
    int m_State_NoChange;
    int m_State_On;
    int m_State_Off;
};

#endif // CHECKBOXITEMDELEGATE_H
