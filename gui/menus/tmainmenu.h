#ifndef TMAINMENU_H
#define TMAINMENU_H

#include "pch.h"
#include "tabstractmenu.h"
#include "tplaycontrolmenu.h"
#include "tvolumecontrolmenu.h"
#include "tplaymodemenu.h"
#include "tskinmenu.h"
#include "ttransparentmenu.h"

class TMainMenu : public TAbstractMenu
{
public:
    TMainMenu(QWidget *parent = 0);
    ~TMainMenu();

    TPlayControlMenu *playControlMenu();
    TVolumeControlMenu *volumeControlMenu();
    TPlaymodeMenu *playmodeMenu();
    TSkinMenu *skinMenu();
    TTransparentMenu *transparentMenu();

signals:
    void onVolumeUpTriggered();
    void onVolumeDownTriggered();
    void onVolumeMuteTriggered();
    void onViewLyricTriggered();
    void onViewPlaylistTriggered();
    void onViewEqualizerTriggered();
    void onViewMinimodeTriggered();
    void onRearrangeWindowsTriggered();
    void onAlwaysOnTopTriggered();
    void onFullLyricTriggered();
    void onFullSpectrumTriggered();
    void onFullBothTriggered();
    void onOptionsTriggered();
    void onLinksWebsiteTriggered();
    void onLinksHelpTriggered();
    void onAboutTriggered();
    void onAboutQtTriggered();
    void onExitTriggered();

private:
    TPlayControlMenu *mPlayingMenu;
    TVolumeControlMenu *mVolumeMenu;
    TPlaymodeMenu *mPlaymodeMenu;
    TSkinMenu *mSkinMenu;
    TTransparentMenu *mTransparentMenu;

    QMenu *mViewMenu;
    QMenu *mFullScreenMenu;
    QMenu *mAboutMenu;

    QAction *mActionAbout;
    QAction *mActionAboutQt;
    QAction *mActionOptions;
    QAction *mActionExit;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TMAINMENU_H
