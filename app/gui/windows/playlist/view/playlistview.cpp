#include "playlistview.h"

TPlaylistView::TPlaylistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setEditTriggers(QAbstractItemView::SelectedClicked);
}

void TPlaylistView::editCurrent()
{
    edit(currentIndex());
}

void TPlaylistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    horizontalHeader()->setStretchLastSection(true);
}

void TPlaylistView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    int row = index.row();
    if(row >= 0)
        emit onCurrentRowSelected(row);
    TAbstractTableView::mousePressEvent(event);
}

void TPlaylistView::updateColumnsWidth()
{

}
