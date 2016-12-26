#include "popmenumusiclistedit.h"

TPopMenuMusiclistEdit::TPopMenuMusiclistEdit(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionCopy = addAction(QString(), this, SIGNAL(onActionCopyTriggered()));
    mActionPaste = addAction(QString(), this, SIGNAL(onActionPasteTriggered()));
    mActionRemove = addAction(QString(), this, SIGNAL(onActionRemoveTriggered()));
    addSeparator();
    mActionSelectAll = addAction(QString(), this, SIGNAL(onActionSelectAllTriggered()));
    mActionUnSelectAll = addAction(QString(), this, SIGNAL(onActionUnSelectAllTriggered()));
    mActionInverse = addAction(QString(), this, SIGNAL(onActionInverseTriggered()));
    retranslateUi();
}

TPopMenuMusiclistEdit::~TPopMenuMusiclistEdit()
{

}

void TPopMenuMusiclistEdit::retranslateUi()
{
    mActionCopy->setText(tr("Copy"));
    mActionPaste->setText(tr("Paste"));
    mActionRemove->setText(tr("Remove"));
    mActionSelectAll->setText(tr("SelectAll"));
    mActionUnSelectAll->setText(tr("UnSelect all"));
    mActionInverse->setText(tr("Inverse"));
}
