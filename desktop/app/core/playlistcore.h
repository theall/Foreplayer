#ifndef TPLAYLISTCORE_H
#define TPLAYLISTCORE_H

#include <QList>
#include <QStringList>

typedef QList<int> intn;

class TPlaylistCore
{
public:
    explicit TPlaylistCore();
    ~TPlaylistCore();

    /**
     * Playlist functions
     */
    QStringList names();
    int size();

    void insert(QString name, int index = -1);
    bool remove(int index);
    void rename(int index, QString newName);

    int playingPlaylistIndex();
    int playingMusicIndex();
    int playingTrackIndex();
    void playingIndex(int *pIndex, int *mIndex, int *tIndex);
    void setPlayingIndex(int pIndex, int mIndex, int tIndex);

    void exportAs(int index, QString fileName);
    void exportAll(QString path);

    void insertMusic(QStringList fileNames, int playlistIndex=-1, int index=-1);
    void insertMusic(int playlistIndex, int newIndex, intn oldIndex);
    void removeMusic(int playlistIndex, intn index);
    void removeRedundant(int playlistIndex = -1);
    void removeErrorFiles(int playlistIndex = -1);
    void clearMusics(int playlistIndex = -1);

    void save();

private:
    // Current playing index
    int mPlaylistIndex;
    int mMusiclistIndex;
    int mTracklistIndex;
    bool mFileSaving;

    void findPlaylist();
    void loadPlaylist(QString fileName);
    QString getFileName();
};

#endif // TPLAYLISTCORE_H
