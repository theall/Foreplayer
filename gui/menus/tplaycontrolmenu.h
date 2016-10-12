#ifndef TPLAYCONTROLMENU_H
#define TPLAYCONTROLMENU_H

#include "tabstractmenu.h"

class TPlayControlMenu : public TAbstractMenu
{
    Q_OBJECT

public:
    TPlayControlMenu(QWidget *parent = 0);

signals:
    void onPlayTriggered();
    void onPauseTriggered();
    void onStopTriggered();
    void onPrevTrackTriggered();
    void onNextTrackTriggered();
    void onPrevMusicTriggered();
    void onNextMusicTriggered();
    void onOpenFileTriggered();

private:
    QAction *mActionPlay;
    QAction *mActionPause;
    QAction *mActionStop;
    QAction *mActionPrevTrack;
    QAction *mActionNextTrack;
    QAction *mActionPrevMusic;
    QAction *mActionNextMusic;
    QAction *mActionOpenFile;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPLAYCONTROLMENU_H
