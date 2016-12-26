#ifndef TPOPMENUPLAYLIST_H
#define TPOPMENUPLAYLIST_H

#include "abstractpopmenu.h"

class TPopMenuPlayList : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuPlayList(QWidget *parent = 0);
    ~TPopMenuPlayList();

    void pop(const QPoint &pos, bool enableAllActions = true, QAction *at = 0);

signals:
    void onActionAddTriggered();
    void onActionRemoveTriggered();
    void onActionRenameTriggered();
    void onActionSortTriggered();
    void onActionSendTriggered();

private:
    QAction *mActionAdd;
    QAction *mActionRemove;
    QAction *mActionRename;
    QAction *mActionSort;
    QAction *mActionSend;

    // QWidget interface
protected:
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUPLAYLIST_H
