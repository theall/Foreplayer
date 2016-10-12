#include "core.h"

TCore::TCore()
{
    mPlayer = new TPlayerCore();
    mPlaylist = new TPlaylistCore();
}

TCore::~TCore()
{
    if(mPlayer)
    {
        delete mPlayer;
        mPlayer = NULL;
    }
    if(mPlaylist)
    {
        delete mPlaylist;
        mPlaylist = NULL;
    }
}

TPlayerCore *TCore::player()
{
    return mPlayer;
}

TPlaylistCore *TCore::playlist()
{
    return mPlaylist;
}

