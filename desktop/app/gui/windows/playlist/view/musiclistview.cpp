#include "musiclistview.h"

#define COLUMN_ICON             0
#define COLUMN_INDEX            1
#define COLUMN_TITLE            2
#define COLUMN_DURATION         3
#define COLUMN_ICON_WIDTH       6

TMusiclistView::TMusiclistView(QWidget *parent) :
    TAbstractTableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TMusiclistView::editCurrentName()
{
    edit(currentIndex());
}

void TMusiclistView::setModel(QAbstractItemModel *model)
{
    TAbstractTableView::setModel(model);

    updateColumnsWidth();
}

void TMusiclistView::updateColumnsWidth()
{
    setColumnWidth(COLUMN_ICON, COLUMN_ICON_WIDTH);
    resizeColumnToContents(COLUMN_INDEX);
    resizeColumnToContents(COLUMN_DURATION);

    int w = rect().width()-columnWidth(COLUMN_INDEX)-columnWidth(COLUMN_DURATION)-COLUMN_ICON_WIDTH;
    QScrollBar *bar = verticalScrollBar();
    if(bar->isVisible())
        w -= bar->width()+3;

    setColumnWidth(COLUMN_TITLE, w);
}
