#include "maincontroller.h"

TMainController::TMainController(QObject *parent) :
    TAbstractController(parent)
  , mPlayerController(new TPlayerController(this))
  , mPlaylistController(new TPlaylistController(this))
  , mEqualizerController(new TEqualizerController(this))

{
    connect(mPlaylistController,
            SIGNAL(requestPlay(int,int,int)),
            mPlayerController,
            SLOT(slotRequestPlay(int,int,int)));

    connect(mPlayerController,
            SIGNAL(requestUpdateModelsPlayingIndex(int,int,int)),
            mPlaylistController,
            SLOT(slotRequestUpdateModelsPlayingIndex(int,int,int)));

    connect(mPlayerController,
            SIGNAL(requestCurrentIndex(int*,int*,int*)),
            mPlaylistController,
            SLOT(slotRequestCurrentIndex(int*,int*,int*)));

    connect(mPlayerController,
            SIGNAL(requestFixDuration(int)),
            mPlaylistController,
            SLOT(slotRequestFixDuration(int)));
}

TMainController::~TMainController()
{

}

bool TMainController::joint(TGuiManager *manager, TCore *core)
{
    Q_ASSERT(manager);
    Q_ASSERT(core);

    connect(manager, SIGNAL(requestShutdown()), this, SLOT(slotQuitApp()));

    if(!manager->loadSkin("skins/winamp.zip"))
        return false;

    mPlayerController->joint(manager, core);
    mPlaylistController->joint(manager, core);
    mEqualizerController->joint(manager, core);

    manager->open();

    return TAbstractController::joint(manager, core);
}

void TMainController::slotQuitApp()
{
    qApp->quit();
}

void TMainController::slotTimerEvent()
{
}
