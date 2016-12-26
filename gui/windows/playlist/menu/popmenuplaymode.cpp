#include "popmenuplaymode.h"

TPopMenuPlayMode::TPopMenuPlayMode(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionSingleOnce = addAction(QString(), this, SIGNAL(onActionSingleOnceTriggered()));
    mActionSignleRecycle = addAction(QString(), this, SIGNAL(onActionSignleRecycleTriggered()));
    mActionAllOnce = addAction(QString(), this, SIGNAL(onActionAllOnceTriggered()));
    mActionAllRecycle = addAction(QString(), this, SIGNAL(onActionAllRecycleTriggered()));
    mActionRandom = addAction(QString(), this, SIGNAL(onActionRandomTriggered()));
    mActionAutoSwitch = addAction(QString(), this, SIGNAL(onActionAutoSwitchTriggered()));
    retranslateUi();
}

TPopMenuPlayMode::~TPopMenuPlayMode()
{

}

void TPopMenuPlayMode::retranslateUi()
{
    setTitle(tr("Play Mode"));
    mActionSingleOnce->setText(tr("Single once"));
    mActionSignleRecycle->setText(tr("Signle recycle"));
    mActionAllOnce->setText(tr("All once"));
    mActionAllRecycle->setText(tr("All recycle"));
    mActionRandom->setText(tr("Random"));
    mActionAutoSwitch->setText(tr("Auto switch"));
}
