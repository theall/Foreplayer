#include "equalizermenu.h"

TEqualizerMenu::TEqualizerMenu(QWidget *parent) :
    TAbstractMenu(parent)
  , mLastActivedAction(NULL)
{
    mActionRock = new QAction(this);
    mActionMetal = new QAction(this);
    mActionElectric = new QAction(this);
    mActionPop = new QAction(this);
    mActionJazz = new QAction(this);
    mActionClassic = new QAction(this);
    mActionCustomize = new QAction(this);

    mActionRock->setCheckable(true);
    mActionMetal->setCheckable(true);
    mActionElectric->setCheckable(true);
    mActionPop->setCheckable(true);
    mActionJazz->setCheckable(true);
    mActionClassic->setCheckable(true);
    mActionCustomize->setCheckable(true);

    addAction(mActionRock);
    addAction(mActionMetal);
    addAction(mActionElectric);
    addAction(mActionPop);
    addAction(mActionJazz);
    addAction(mActionClassic);
    addSeparator();
    addAction(mActionCustomize);
}

void TEqualizerMenu::slotActionTriggered(bool)
{
    QAction *action = static_cast<QAction*>(sender());

    if(mLastActivedAction)
        mLastActivedAction->setChecked(false);

    mLastActivedAction = action;

    if(action==mActionRock) {
       emit onRockSelected();
    }
    else if(action==mActionMetal) {
       emit onMetalSelected();
    }
    else if(action==mActionElectric) {
       emit onElectricSelected();
    }
    else if(action==mActionPop) {
       emit onPopSelected();
    }
    else if(action==mActionJazz) {
       emit onJazzSelected();
    }
    else if(action==mActionClassic) {
       emit onClassicSelected();
    }
    else if(action==mActionCustomize) {
       emit onCustomizeSelected();
    }
}

void TEqualizerMenu::retranslateUi()
{
    mActionRock->setText(tr("Rock"));
    mActionMetal->setText(tr("Metal"));
    mActionElectric->setText(tr("Electric"));
    mActionPop->setText(tr("Pop"));
    mActionJazz->setText(tr("Jazz"));
    mActionClassic->setText(tr("Classic"));
    mActionCustomize->setText(tr("Customize"));
}
