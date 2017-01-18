#include "popmenuplaylist.h"

TPopMenuPlayList::TPopMenuPlayList(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionAdd = addAction(QString(), this, SIGNAL(onActionAddTriggered()));
    mActionRemove = addAction(QString(), this, SIGNAL(onActionRemoveTriggered()));
    mActionRename = addAction(QString(), this, SIGNAL(onActionRenameTriggered()));
    mActionSort = addAction(QString(), this, SIGNAL(onActionSortTriggered()));
    mActionSend = addAction(QString(), this, SIGNAL(onActionSendTriggered()));

    retranslateUi();
}

TPopMenuPlayList::~TPopMenuPlayList()
{

}

void TPopMenuPlayList::pop(const QPoint &pos, bool enableAllActions, QAction *at)
{
    if(!enableAllActions)
    {
        mActionRemove->setEnabled(false);
        mActionRename->setEnabled(false);
    }
    popup(pos, at);
}

void TPopMenuPlayList::showEvent(QShowEvent *ev)
{
    QMenu::showEvent(ev);
}

void TPopMenuPlayList::retranslateUi()
{
    setTitle(tr("Playlist"));

    mActionAdd->setText(tr("Add"));
    mActionRemove->setText(tr("Remove"));
    mActionRename->setText(tr("Rename"));
    mActionSort->setText(tr("Sort"));
    mActionSend->setText(tr("Send"));
}
