#include "checkboxitemdelegate.h"

#include <QPainter>
#include <QApplication>

//#include "cvsmodel/objectitem.h"

CheckBoxItemDelegate::CheckBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void CheckBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

    if (data == m_State_NoChange) checkboxstyle.state = QStyle::State_NoChange | QStyle::State_Enabled;
    else if (data == m_State_On)  checkboxstyle.state = QStyle::State_On       | QStyle::State_Enabled;
    else if (data == m_State_Off) checkboxstyle.state = QStyle::State_Off      | QStyle::State_Enabled;

    style->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
}

void CheckBoxItemDelegate::setDataValues(const int stateNoChange, const int stateOn, const int stateOff)
{
    m_State_NoChange = stateNoChange;
    m_State_On = stateOn;
    m_State_Off = stateOff;
}
