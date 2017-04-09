#ifndef TMUSICLISTVIEW_H
#define TMUSICLISTVIEW_H

#include "abstracttableview.h"

class TMusiclistView : public TAbstractTableView
{
public:
    TMusiclistView(QWidget *parent = 0);

    void editCurrentName();
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TMUSICLISTVIEW_H
