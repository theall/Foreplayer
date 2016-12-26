#ifndef TMUSICLISTVIEW_H
#define TMUSICLISTVIEW_H

#include "abstracttableview.h"

class TMusiclistView : public TAbstractTableView
{
public:
    TMusiclistView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void updateColumnsWidth() Q_DECL_OVERRIDE;
};

#endif // TMUSICLISTVIEW_H