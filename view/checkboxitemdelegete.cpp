#include "checkboxitemdelegete.h"

#include <QPainter>
#include <QApplication>

CheckBoxItemDelegete::CheckBoxItemDelegete(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void CheckBoxItemDelegete::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    bool data = index.model()->data(index, Qt::DisplayRole).toBool();
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()/2 - checkbox_rect.width()/2);
    if(data)
        checkboxstyle.state = QStyle::State_On | QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off | QStyle::State_Enabled;

    style->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
}

