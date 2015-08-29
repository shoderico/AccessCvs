#include "boolcheckboxitemdelegate.h"

#include <QPainter>
#include <QApplication>
#include <QKeyEvent>

//#include "cvsmodel/objectmodel.h"

BoolCheckBoxItemDelegate::BoolCheckBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void BoolCheckBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    /*
    // table only
    const QModelIndex index_type = index.model()->index( index.row(), ObjectModel::ObjectTypeColumn );
    int objectType = index.model()->data(index_type).toInt();
    if (objectType != Model::TableDef)
        return;
    */
    if ( !paintRequired(index) )
        return;

    bool data = index.model()->data(index, Qt::DisplayRole).toBool();
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()/2 - checkbox_rect.width()/2);
    if (data)
        checkboxstyle.state = QStyle::State_On       | QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off      | QStyle::State_Enabled;

    style->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
}

bool BoolCheckBoxItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_ASSERT(event);
    Q_ASSERT(model);

    /*
    // table only
    QModelIndex index_type = model->index( index.row(), ObjectModel::ObjectTypeColumn );
    int objectType = model->data(index_type).toInt();
    if (objectType != Model::TableDef)
        return false;
    */
    if ( !editAvaiable(model, index) )
        return false;

    // make sure that the item is checkable
    Qt::ItemFlags flags = model->flags(index);
    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        return false;

    // make sure that we have a check state
    QVariant value = index.data(Qt::EditRole);
    if (!value.isValid())
        return false;

    // make sure that we have the right event type
    if (event->type() == QEvent::MouseButtonRelease)
    {
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect checkRect = QStyle::alignedRect(option.direction,
                                              Qt::AlignCenter,
                                              option.decorationSize,
                                                QRect(option.rect.x() + (2 * textMargin),
                                                      option.rect.y(),
                                                      option.rect.width() - (2 * textMargin),
                                                      option.rect.height())
                                              );
        if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
            return false;
    }
//      else if (event->type() == QEvent::KeyPress)
//      {
//          if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space && static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
//              return false;
//      }
    else
    {
        return false;
    }
    Qt::CheckState state = (value.toBool() ? Qt::Unchecked : Qt::Checked);
    return model->setData(index, state, Qt::CheckStateRole);
}

bool BoolCheckBoxItemDelegate::paintRequired(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return true;
}

bool BoolCheckBoxItemDelegate::editAvaiable(QAbstractItemModel *model, const QModelIndex &index)
{
    Q_UNUSED(model)
    Q_UNUSED(index)
    return true;
}
