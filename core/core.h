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

    /**
     * Play functions.
     */
    void play(int trackIndex, int musicIndex=-1, int playlist=-1);
    void resume();
    void pause();
    void jump(int n=1);
    void jumpM(int n=1);
    void jumpP(int n=1);
    int progress(int *total=NULL);
    void track(int seconds);
    QByteArray sample();

    void setPlayMode(PlayMode mode);

    /**
     * @brief Set volume.
     * @param value 0.0~1.0
     */
    float volume();
    void setVolume(float value);
    void toggleVolume(bool enable);

    int effect();
    void set3DEffect(int value);

    void lrBallance();
    void setLRBallance(int value);

    int eqValue();
    void setEqValue();

    intn eqValues();
    void setEqValues(intn values);

    /**
     * Play list functions.
     */
    QStringList playLists();
    void plInsert(QString name, int index = -1);
    void plDelete(int index);
    void plDelete(intn index);
    void plRename(int index, QString newName);
    void plSort(SortMode = TITLE_ASC);
    void plSave(int index);
    void plSaveAll();
    int plCurrentIndex();
    void plsetCurrentIndex();

    /**
     * Music list functions.
     */
    TMusicInfoList musicList(int plIndex = -1);
    void mlInsert(QStringList fileNames, int index = -1);
    void mlInsert(int newIndex, int oldIndex);
    void mlDelete(int index);
    void mlDelete(intn indexes);
    void mlDeleteReluctant();
    void mlDeleteErrorFiles();
    void mlClear();
    void mlSort(SortMode mode);

    /**
     * Track list functions.
     */
    TTrackInfoList trackList(int mlIndex = -1);
    void reload(int mlIndex=-1);

private:
    TPlayerCore *mPlayer;
    TPlaylistCore *mPlaylist;
};

#endif // TCORE_H
