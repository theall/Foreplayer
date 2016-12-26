#ifndef TPLAYLISTCORE_H
#define TPLAYLISTCORE_H

#include "playlist/playlistitem.h"

typedef QMap<TPlaylistItem*, TMusicItems> TPlaylistMap;
typedef QMap<TMusicItem*, TTrackItems> TMusiclistMap;

enum IndexType
{
    PlayList,
    MusicList,
    TrackList
};

typedef QList<int> intn;

class TPlaylistCore
{
    Q_DECLARE_TR_FUNCTIONS(TPlaylistCore)

public:
    explicit TPlaylistCore();
    ~TPlaylistCore();

    QStringList names();
    TPlaylistItems *playlists();
    void insertPlaylist(QString name, int index = -1);
    void removePlaylist(int index);
    void renamePlaylist(int index, QString newName);
    void sortPlaylist(SortMode mode = TITLE_ASC);

    int currentIndex(IndexType type=MusicList);
    void setCurrentIndex(IndexType type, int index);

    void exportAs(int index, QString fileName);
    void exportAll(QString path);

    /**
     * Music list functions.
     */
    TMusicItems musicList(int plIndex = -1);
    void insertMusic(QStringList fileNames, int playlistIndex=-1, int index=-1);
    void insertMusic(int playlistIndex, int newIndex, intn oldIndex);
    void removeMusic(int playlistIndex, intn index);
    void removeRedundant(int playlistIndex = -1);
    void removeErrorFiles(int playlistIndex = -1);
    void clearMusics(int playlistIndex = -1);
    void sortMusics(SortMode mode);

    /**
     * Track list functions.
     */
    TTrackItems trackList(int mlIndex = -1);
    void reload(int mlIndex=-1);

private:
    int mCurrentPlaylistIndex;
    int mCurrentMusiclistIndex;
    int mCurrentTracklistIndex;

    TPlaylistItems mPlaylist;
    TPlaylistMap mPlaylistMap;
    TMusiclistMap mMusiclistMap;

    void findPlaylist(QString path);
    void loadPlaylist(QString fileName);
    void save();
    QString getFileName();
    QString getFullPath(QString name);
};

#endif // TPLAYLISTCORE_H
