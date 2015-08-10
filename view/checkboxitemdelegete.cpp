#include "checkboxitemdelegete.h"

#include <QPainter>
#include <QApplication>

#include "model/objectitem.h"

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

    int data = index.model()->data(index, Qt::DisplayRole).toInt();
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()/2 - checkbox_rect.width()/2);
    switch (data)
    {
        case Model::State_NoChange: checkboxstyle.state = QStyle::State_NoChange | QStyle::State_Enabled;   break;
        case Model::State_On:       checkboxstyle.state = QStyle::State_On       | QStyle::State_Enabled;   break;
        case Model::State_Off:      checkboxstyle.state = QStyle::State_Off      | QStyle::State_Enabled;   break;
    }

    style->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
}

