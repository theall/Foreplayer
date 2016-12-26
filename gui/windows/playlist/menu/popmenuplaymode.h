#ifndef TPOPMENUPLAYMODE_H
#define TPOPMENUPLAYMODE_H

#include "abstractpopmenu.h"

class TPopMenuPlayMode : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuPlayMode(QWidget *parent = 0);
    ~TPopMenuPlayMode();

signals:
    void onActionSingleOnceTriggered();
    void onActionSignleRecycleTriggered();
    void onActionAllOnceTriggered();
    void onActionAllRecycleTriggered();
    void onActionRandomTriggered();
    void onActionAutoSwitchTriggered();

private:
    QAction *mActionSingleOnce;
    QAction *mActionSignleRecycle;
    QAction *mActionAllOnce;
    QAction *mActionAllRecycle;
    QAction *mActionRandom;
    QAction *mActionAutoSwitch;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUPLAYMODE_H
