#ifndef TPLAYLISTCORE_H
#define TPLAYLISTCORE_H

#include "playlist/playlistitem.h"
#include "pluginmanager/backendpluginmanager.h"

typedef QList<int> intn;

class TPlaylistCore
{
    Q_DECLARE_TR_FUNCTIONS(TPlaylistCore)

public:
    explicit TPlaylistCore();
    ~TPlaylistCore();

    /**
     * Playlist functions
     */
    QStringList names();
    int size();
    TPlaylistItem *currentPlaylistItem();
    TPlaylistItem *playlistItem(int plIndex = -1);
    TPlaylistItem *takeAt(int plIndex);
    void insert(int pos, TPlaylistItem *item);
    void insert(QString name, int index = -1);
    bool remove(int index);
    void rename(int index, QString newName);
    void sort(SortMethod mode = SM_TITLE_ASC);
    int indexOf(TPlaylistItem *item);

    int playingPlaylistIndex();
    int playingMusicIndex();
    int playingTrackIndex();
    void playingIndex(int *pIndex, int *mIndex, int *tIndex);
    void setPlayingIndex(int pIndex, int mIndex, int tIndex);

    void exportAs(int index, QString fileName);
    void exportAll(QString path);

    /**
     * Music list functions.
     */
    TMusicItem *currentMusicItem();
    void insertMusic(QStringList fileNames, int playlistIndex=-1, int index=-1);
    void insertMusic(int playlistIndex, int newIndex, intn oldIndex);
    void removeMusic(int playlistIndex, intn index);
    void removeRedundant(int playlistIndex = -1);
    void removeErrorFiles(int playlistIndex = -1);
    void clearMusics(int playlistIndex = -1);
    void sortMusics(SortMethod mode);

    /**
     * Track list functions.
     */
    TTrackItem *currentTrackItem();
    TTrackItems trackList(int mlIndex = -1);
    void reload(int mlIndex=-1);

    /**
     * Parse track list from music.
     */
    TMusicItem *parse(QString file);

    void save();

private:
    TPlaylistItems mPlaylist;

    // Current playing index
    int mPlaylistIndex;
    int mMusiclistIndex;
    int mTracklistIndex;
    QDir mCurrentDir;
    QDir mPlaylistDir;
    bool mFileSaving;
    TBackendPluginManager *mBackendPluginManager;

    void findPlaylist();
    void loadPlaylist(QString fileName);
    QString getFileName();
};

#endif // TPLAYLISTCORE_H
