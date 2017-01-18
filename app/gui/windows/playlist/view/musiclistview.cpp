#include "musiclistview.h"

TMusiclistView::TMusiclistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TMusiclistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    updateColumnsWidth();
}

void TMusiclistView::updateColumnsWidth()
{
    resizeColumnToContents(0);
    resizeColumnToContents(2);

    int w = rect().width()-columnWidth(0)-columnWidth(2);
    QScrollBar *bar = verticalScrollBar();
    if(bar->isVisible())
        w -= bar->width()+3;

    setColumnWidth(1, w);
}

void TMusiclistView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    emit onCurrentRowSelected(index.row());
    TAbstractTableView::mouseDoubleClickEvent(event);
}
