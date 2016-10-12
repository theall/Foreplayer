#ifndef TPLAYMODEMENU_H
#define TPLAYMODEMENU_H

#include "tabstractmenu.h"

class TPlaymodeMenu : public TAbstractMenu
{
    Q_OBJECT
public:
    TPlaymodeMenu(QWidget *parent = 0);

signals:
    void onSingleOnceTriggered();
    void onSingleRecycleTriggered();
    void onTracklistRecycleTriggered();
    void onTracklistShuffleTriggered();
    void onShuffleAllTriggered();
    void onRecycleAllTriggered();
    void onTracklistOnceTriggered();
    void onInorderOnceTriggered();

private slots:
    void slotActionTriggered(bool);

private:
    QAction *mLastActivedAction;
    QAction *mActionSingleOnce;
    QAction *mActionSingleRecycle;
    QAction *mActionTracklistRecycle;
    QAction *mActionTracklistShuffle;
    QAction *mActionShuffleAll;
    QAction *mActionRecycleAll;
    QAction *mActionTracklistOnce;
    QAction *mActionInorderOnce;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPLAYMODEMENU_H
