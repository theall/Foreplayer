#ifndef TPLAYLISTVIEW_H
#define TPLAYLISTVIEW_H

#include "abstracttableview.h"

class TPlaylistView : public TAbstractTableView
{
public:
    TPlaylistView(QWidget *parent = 0);

    void editCurrent();
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // TAbstractTableView interface
protected:
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTVIEW_H
