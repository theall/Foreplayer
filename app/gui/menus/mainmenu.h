#ifndef TMAINMENU_H
#define TMAINMENU_H

#include "pch.h"
#include "abstractmenu.h"
#include "playcontrolmenu.h"
#include "volumecontrolmenu.h"
#include "playmodemenu.h"
#include "skinmenu.h"
#include "transparentmenu.h"

class TMainMenu : public TAbstractMenu
{
    Q_OBJECT

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
