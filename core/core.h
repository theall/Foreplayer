#ifndef TCORE_H
#define TCORE_H

#include "playercore.h"
#include "playlistcore.h"

class TCore
{
public:
    TCore();
    ~TCore();

    TPlayerCore *player();
    TPlaylistCore *playlist();

private:
    TPlayerCore *mPlayer;
    TPlaylistCore *mPlaylist;
};

#endif // TCORE_H
