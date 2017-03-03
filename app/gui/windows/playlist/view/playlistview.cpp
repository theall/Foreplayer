#include "playlistview.h"

TPlaylistView::TPlaylistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setEditTriggers(QAbstractItemView::SelectedClicked);
}

void TPlaylistView::editCurrentName()
{
    edit(currentIndex());
}

void TPlaylistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    horizontalHeader()->setStretchLastSection(true);
}

void TPlaylistView::updateColumnsWidth()
{

}
