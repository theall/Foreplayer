#ifndef TPLAYLISTVIEW_H
#define TPLAYLISTVIEW_H

#include "abstracttableview.h"

enum TPlaylistSortMode
{
    SM_NAME,
    SM_MUSICS
};

class TPlaylistView : public TAbstractTableView
{
public:
    TPlaylistView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:
    void requestAddPlaylist(QString &name);
    void requestRemovePlaylist(int index);
    void requestRenamePlaylist(int index);
    void requestSort(TPlaylistSortMode mode);
    void requestCopyTo();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // TAbstractTableView interface
protected:
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTVIEW_H
