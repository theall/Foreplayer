#include "playlistcontroller.h"

TPlaylistController::TPlaylistController(QObject *parent) :
    TAbstractController(parent)
{

}



void TPlaylistController::joint(TGuiManager *manager, TCore *core)
{
    TAbstractController::joint(manager, core);
}

void TPlaylistController::slotTimerEvent()
{
}
