#include "popmenumusiclistitem.h"

TPopMenuMusiclistItem::TPopMenuMusiclistItem(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionPlay = addAction(QString(), this, SIGNAL(onActionPlayTriggered()));
    addSeparator();
    mActionDetail = addAction(QString(), this, SIGNAL(onActionDetailTriggered()));
    addSeparator();
    mActionCopy = addAction(QString(), this, SIGNAL(onActionCopyTriggered()));
    mActionPaste = addAction(QString(), this, SIGNAL(onActionPasteTriggered()));
    mActionRemove = addAction(QString(), this, SIGNAL(onActionRemoveTriggered()));
    addSeparator();
    mActionCopyToUsb = addAction(QString(), this, SIGNAL(onActionCopyToUsbTriggered()));
    retranslateUi();
}

TPopMenuMusiclistItem::~TPopMenuMusiclistItem()
{

}

void TPopMenuMusiclistItem::retranslateUi()
{
    setTitle(tr("Edit"));
    mActionPlay->setText(tr("Play"));
    mActionDetail->setText(tr("Detail"));
    mActionRemove->setText(tr("Remove"));
    mActionCopy->setText(tr("Copy"));
    mActionPaste->setText(tr("Paste"));
    mActionCopyToUsb->setText(tr("Copy to storage"));
}
