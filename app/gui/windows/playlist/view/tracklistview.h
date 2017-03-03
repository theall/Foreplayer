#ifndef TTRACKLISTVIEW_H
#define TTRACKLISTVIEW_H

#include "abstracttableview.h"

class TTracklistView : public TAbstractTableView
{
public:
    TTracklistView(QWidget *parent = 0);

    void editCurrent();
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

private:
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TTRACKLISTVIEW_H
