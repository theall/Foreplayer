#include "tmainmenu.h"

TMainMenu::TMainMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mPlayingMenu(new TPlayControlMenu(this)),
    mVolumeMenu(new TVolumeControlMenu(this)),
    mPlaymodeMenu(new TPlaymodeMenu(this)),
    mSkinMenu(new TSkinMenu(this)),
    mTransparentMenu(new TTransparentMenu(this)),
    mViewMenu(new QMenu(this)),
    mFullScreenMenu(new QMenu(this)),
    mAboutMenu(new QMenu(this)),
    mActionAbout(new QAction(this)),
    mActionAboutQt(new QAction(this)),
    mActionOptions(new QAction(this)),
    mActionExit(new QAction(this))
{
    connect(mActionAbout, SIGNAL(triggered()), this, SIGNAL(onAboutTriggered()));
    connect(mActionAboutQt, SIGNAL(triggered()), this, SIGNAL(onAboutQtTriggered()));
    connect(mActionOptions, SIGNAL(triggered()), this, SIGNAL(onOptionsTriggered()));
    connect(mActionExit, SIGNAL(triggered()), this, SIGNAL(onExitTriggered()));

    addMenu(mPlayingMenu);
    addMenu(mVolumeMenu);
    addMenu(mPlaymodeMenu);
    addSeparator();

    addMenu(mViewMenu);
    addSeparator();
    addMenu(mSkinMenu);
    addMenu(mTransparentMenu);
    addMenu(mFullScreenMenu);
    addSeparator();
    addAction(mActionOptions);
    addSeparator();
    addMenu(mAboutMenu);
    addSeparator();
    addAction(mActionExit);

    retranslateUi();
}

TMainMenu::~TMainMenu()
{

}

TPlayControlMenu *TMainMenu::playControlMenu()
{
    return mPlayingMenu;
}

TVolumeControlMenu *TMainMenu::volumeControlMenu()
{
    return mVolumeMenu;
}

TPlaymodeMenu *TMainMenu::playmodeMenu()
{
    return mPlaymodeMenu;
}

TSkinMenu *TMainMenu::skinMenu()
{
    return mSkinMenu;
}

TTransparentMenu *TMainMenu::transparentMenu()
{
    return mTransparentMenu;
}

void TMainMenu::retranslateUi()
{
    mPlayingMenu->setTitle(tr("Play Control"));
    mVolumeMenu->setTitle(tr("Volume Control"));
    mPlaymodeMenu->setTitle(tr("Play Mode"));
    mViewMenu->setTitle(tr("View"));
    mSkinMenu->setTitle(tr("Skins"));
    mTransparentMenu->setTitle(tr("Transparent"));
    mFullScreenMenu->setTitle(tr("Fullscreen"));
    mAboutMenu->setTitle(tr("About"));
    mActionOptions->setText(tr("Options"));
    mActionExit->setText(tr("Exit"));
}
