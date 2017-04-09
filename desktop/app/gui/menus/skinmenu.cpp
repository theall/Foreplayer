#include "skinmenu.h"
#include "../share/skin.h"

#define BUF_SIZE 256

TSkinMenu::TSkinMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{

}

void TSkinMenu::slotSkinTriggered(bool checked)
{
    QAction *action = qobject_cast<QAction*>(sender());

    if(checked)
    {
        if(mLastActivedAction)
        {
            mLastActivedAction->blockSignals(true);
            mLastActivedAction->setChecked(false);
            mLastActivedAction->blockSignals(false);
        }
        mLastActivedAction = action;

        if(action)
            emit requestLoadSkin(action->data().toInt());
    }
}

void TSkinMenu::retranslateUi()
{
}

void TSkinMenu::showEvent(QShowEvent *event)
{
    clear();
    QStringList skinNames;
    emit requestSkinNames(skinNames);
    for(int i=0;i<skinNames.count();i++)
    {
        QAction *action = addAction(skinNames[i], this, SLOT(slotSkinTriggered(bool)));
        action->setCheckable(true);
        action->setData(i);
    }
    TAbstractMenu::showEvent(event);
}
