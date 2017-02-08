#include "tracklistview.h"

TTracklistView::TTracklistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TTracklistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    updateColumnsWidth();
}

void TTracklistView::updateColumnsWidth()
{
    resizeColumnToContents(0);
    resizeColumnToContents(2);

    setColumnWidth(0, columnWidth(0)+5);
    int w = rect().width()-columnWidth(0)-columnWidth(2);
    QScrollBar *bar = verticalScrollBar();
    if(bar->isVisible())
        w -= bar->width()+3;
    setColumnWidth(1, w);
}
