#include "playlistcontroller.h"

TPlaylistController::TPlaylistController(QObject *parent) : QObject(parent)
{

}

void TPlaylistController::joint(TPlaylistWindow *window, TPlaylistCore *core)
{

    mWindow = window;
    mCore = core;
}

