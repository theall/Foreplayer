#include "core.h"

TCore::TCore()
{
    mPlayer = new TPlayerCore();
    mPlaylist = new TPlaylistCore();
    mPlugins = TBackendPluginManager::instance();
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
    if(mPlugins)
    {
        TBackendPluginManager::deleteInstance();
        mPlugins = NULL;
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

QStringList TCore::playlists()
{
    return mPlaylist->names();
}

void TCore::open()
{

}

void TCore::close()
{

}
