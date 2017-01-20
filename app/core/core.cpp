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
        delete mPlugins;
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

/**
 * Play functions.
 */
void TCore::play(int trackIndex, int musicIndex, int playlist)
{
    if(musicIndex<0)
        musicIndex = mPlaylist->playingPlaylistIndex();

    mPlayer->setTrack(NULL);
}

void TCore::resume()
{
}

void TCore::pause()
{
}

void TCore::jump(int n)
{
}

void TCore::jumpM(int n)
{
}

void TCore::jumpP(int n)
{
}

void TCore::track(int seconds)
{
}

QByteArray TCore::sample()
{
}


void TCore::setPlayMode(PlayMode mode)
{
}


/**
 * @brief Set volume.
 * @param value 0.0~1.0
 */
float TCore::volume()
{
}

void TCore::setVolume(float value)
{
}

void TCore::toggleVolume(bool enable)
{
}


int TCore::effect()
{
}

void TCore::set3DEffect(int value)
{
}


void TCore::lrBallance()
{
}

void TCore::setLRBallance(int value)
{
}


int TCore::eqValue()
{
}

void TCore::setEqValue()
{
}


intn TCore::eqValues()
{
}

void TCore::setEqValues(intn values)
{
}


/**
 * Play list functions.
 */
QStringList TCore::playLists()
{
    return mPlaylist->names();
}

void TCore::plInsert(QString name, int index)
{
}

void TCore::plDelete(int index)
{
}

void TCore::plDelete(intn index)
{
}

void TCore::plRename(int index, QString newName)
{
}

void TCore::plSort(SortMode mode)
{
}

void TCore::plSave(int index)
{
}

void TCore::plSaveAll()
{
}

int TCore::plCurrentIndex()
{
}

void TCore::plsetCurrentIndex()
{
}


/**
 * Music list functions.
 */
TMusicInfoList TCore::musicList(int plIndex)
{
}

void TCore::mlInsert(QStringList fileNames, int index)
{
}

void TCore::mlInsert(int newIndex, int oldIndex)
{
}

void TCore::mlDelete(int index)
{
}

void TCore::mlDelete(intn indexes)
{
}

void TCore::mlDeleteReluctant()
{
}

void TCore::mlDeleteErrorFiles()
{
}

void TCore::mlClear()
{
}

void TCore::mlSort(SortMode mode)
{
}


/**
 * Track list functions.
 */
TTrackInfoList TCore::trackList(int mlIndex)
{
}

void TCore::reload(int mlIndex)
{
}

