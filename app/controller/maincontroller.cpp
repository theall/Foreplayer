#include "maincontroller.h"

TMainController::TMainController(QObject *parent) :
    TAbstractController(parent),
    mPlayerController(new TPlayerController(this)),
    mPlaylistController(new TPlaylistController(this))
{
    connect(mPlaylistController,
            SIGNAL(requestPlay(int,int,int)),
            mPlayerController,
            SLOT(slotRequestPlay(int,int,int)));
    connect(mPlayerController,
            SIGNAL(requestUpdateModelsPlayingIndex(int,int,int)),
            mPlaylistController,
            SLOT(slotRequestUpdateModelsPlayingIndex(int,int,int)));
}

TMainController::~TMainController()
{

}

void TMainController::joint(TGuiManager *manager, TCore *core)
{
    Q_ASSERT(manager);
    Q_ASSERT(core);

    connect(manager, SIGNAL(requestShutdown()), this, SLOT(slotQuitApp()));

    if(manager->loadSkin("z:/skins/fulkfour.zip"))
    {
        mPlayerController->joint(manager, core);
        mPlaylistController->joint(manager, core);

        manager->showGui();
    } else {

    }

    TAbstractController::joint(manager, core);
}

void TMainController::slotQuitApp()
{
    qApp->quit();
}

void TMainController::slotTimerEvent()
{
}
