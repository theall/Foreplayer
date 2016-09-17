#include "ttransparentmenu.h"

TTransparentMenu::TTransparentMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{
    mActionDisable = new QAction(this);
    mActionTransparent1 = new QAction(this);
    mActionTransparent2 = new QAction(this);
    mActionTransparent3 = new QAction(this);
    mActionTransparent4 = new QAction(this);
    mActionTransparent5 = new QAction(this);
    mActionTransparent6 = new QAction(this);
    mActionTransparent7 = new QAction(this);
    mActionTransparent8 = new QAction(this);
    mActionTransparent9 = new QAction(this);
    mActionTransparent10 = new QAction(this);
    mActionDisableWhileActived = new QAction(this);

    connect(mActionDisable, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent1, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent2, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent3, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent4, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent5, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent6, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent7, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent8, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent9, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionTransparent10, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));
    connect(mActionDisableWhileActived, SIGNAL(triggered(bool)), this, SLOT(slotActionTriggered(bool)));

    addAction(mActionDisable);
    addAction(mActionTransparent1);
    addAction(mActionTransparent2);
    addAction(mActionTransparent3);
    addAction(mActionTransparent4);
    addAction(mActionTransparent5);
    addAction(mActionTransparent6);
    addAction(mActionTransparent7);
    addAction(mActionTransparent8);
    addAction(mActionTransparent9);
    addAction(mActionTransparent10);
    addSeparator();
    addAction(mActionDisableWhileActived);

    retranslateUi();
}

void TTransparentMenu::slotActionTriggered(bool)
{
    QAction *action = static_cast<QAction*>(sender());
    if(action)
    {
        if(action != mActionDisableWhileActived && mLastActivedAction)
        {
            mLastActivedAction->setChecked(false);
        }

        if(action == mActionDisable)
        {
            emit transparentValueChanged(0);
        }
        else if(action == mActionTransparent1)
        {
            emit transparentValueChanged(1);
        }
        else if(action == mActionTransparent2)
        {
            emit transparentValueChanged(2);
        }
        else if(action == mActionTransparent3)
        {
            emit transparentValueChanged(3);
        }
        else if(action == mActionTransparent4)
        {
            emit transparentValueChanged(4);
        }
        else if(action == mActionTransparent5)
        {
            emit transparentValueChanged(5);
        }
        else if(action == mActionTransparent6)
        {
            emit transparentValueChanged(6);
        }
        else if(action == mActionTransparent7)
        {
            emit transparentValueChanged(7);
        }
        else if(action == mActionTransparent8)
        {
            emit transparentValueChanged(8);
        }
        else if(action == mActionTransparent9)
        {
            emit transparentValueChanged(9);
        }
        else if(action == mActionTransparent10)
        {
            emit transparentValueChanged(10);
        }
        else if(action == mActionDisableWhileActived)
        {
            emit diableWhileActivedToggled(action->isChecked());
        }
    }
    mLastActivedAction = action;
}

void TTransparentMenu::retranslateUi()
{
}
