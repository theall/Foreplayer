#ifndef TPLAYLISTVIEW_H
#define TPLAYLISTVIEW_H

#include "abstracttableview.h"

class TPlaylistView : public TAbstractTableView
{
public:
    TPlaylistView(QWidget *parent = 0);

    void editCurrentName();
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTVIEW_H
