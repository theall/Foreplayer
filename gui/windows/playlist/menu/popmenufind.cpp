#include "popmenufind.h"

TPopMenuFind::TPopMenuFind(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionMark = addAction(QString(), this, SIGNAL(onActionMarkTriggered()));
    mActionFind = addAction(QString(), this, SIGNAL(onActionFindTriggered()));
    mActionFindNext = addAction(QString(), this, SIGNAL(onActionFindTriggered()));
    retranslateUi();
}

TPopMenuFind::~TPopMenuFind()
{

}

void TPopMenuFind::retranslateUi()
{
    setTitle(tr("Find"));

    mActionMark->setText(tr("Mark"));
    mActionFind->setText(tr("Find"));
    mActionFindNext->setText(tr("Find next"));
}
