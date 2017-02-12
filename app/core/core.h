#ifndef TCORE_H
#define TCORE_H

#include "playercore.h"
#include "playlistcore.h"

enum PlayMode
{
    SINGLE_TRACK,
    SINGLE_TRACK_RECYCLE,
    SINGLE_MUSIC,
    SINGLE_MUSIC_RECYCLE,
    SINGLE_PLAY_LIST,
    SINGLE_PLAY_LIST_RECYCLE,
    ORDER_TRACK_LIST,
    ORDER_TRACK_LIST_RECYCLE,
    ORDER_MUSIC_LIST,
    ORDER_MUSIC_LIST_RECYCLE,
    ORDER_PLAY_LIST,
    ORDER_PLAY_LIST_RECYCLE,
    RANDOM_TRACK_LIST,
    RANDOM_MUSIC_LIST,
    RANDOM_PLAY_LIST
};

typedef QList<int> intn;

class TCore
{
public:
    TCore();
    ~TCore();

    TPlayerCore *player();
    TPlaylistCore *playlist();

    QStringList playlists();

    void open();
    void close();

private:
    TPlayerCore *mPlayer;
    TPlaylistCore *mPlaylist;
    TBackendPluginManager *mPlugins;
};

#endif // TCORE_H
