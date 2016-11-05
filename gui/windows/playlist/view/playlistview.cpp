#include "playlistview.h"

TPlaylistView::TPlaylistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setEditTriggers(QAbstractItemView::SelectedClicked);
}

void TPlaylistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    horizontalHeader()->setStretchLastSection(true);
}

void TPlaylistView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    emit onCurrentRowSelected(index.row());
    TAbstractTableView::mousePressEvent(event);
}

void TPlaylistView::updateColumnsWidth()
{

}
