#include "playlistcore.h"


TPlaylistCore::TPlaylistCore()
{

}

TPlaylistCore::~TPlaylistCore()
{
    save();
}

QStringList TPlaylistCore::names()
{
    return sl;
}

int TPlaylistCore::size()
{
    return mPlaylist.size();
}

TPlaylistItem *TPlaylistCore::currentPlaylistItem()
{
    if(mPlaylistIndex<0 || mPlaylistIndex>=mPlaylist.size())
        return NULL;

    return mPlaylist[mPlaylistIndex];
}

void TPlaylistCore::insert(QString name, int index)
{
    TPlaylistItem *playlistItem = new TPlaylistItem;
    playlistItem->setDisplayName(name);
    playlistItem->setFileName(getFileName());

    if(index>-1)
        mPlaylist.insert(index, playlistItem);
    else
        mPlaylist.append(playlistItem);
}

bool TPlaylistCore::remove(int index)
{
    if(index<0 || index>=mPlaylist.size())
        return false;

    TPlaylistItem *item = mPlaylist.at(index);
    QFile file(item->fileName());
    bool status = file.remove();
    if(status)
    {
        mPlaylist.removeAt(index);
        delete item;
    }
    return status;
}

void TPlaylistCore::rename(int index, QString newName)
{

}

void TPlaylistCore::sort(SortMethod mode)
{

}

int TPlaylistCore::indexOf(TPlaylistItem *item)
{

}

int TPlaylistCore::playingPlaylistIndex()
{
    return mPlaylistIndex;
}

int TPlaylistCore::playingMusicIndex()
{
    return mMusiclistIndex;
}

int TPlaylistCore::playingTrackIndex()
{
    return mTracklistIndex;
}

void TPlaylistCore::playingIndex(int *pIndex, int *mIndex, int *tIndex)
{
    if(mIndex==NULL || pIndex==NULL || tIndex==NULL)
        return;

    *pIndex = mPlaylistIndex;
    *mIndex = mMusiclistIndex;
    *tIndex = mTracklistIndex;
}

void TPlaylistCore::setPlayingIndex(int pIndex, int mIndex, int tIndex)
{
    mPlaylistIndex = pIndex;
    mMusiclistIndex = mIndex;
    mTracklistIndex = tIndex;
}

void TPlaylistCore::exportAs(int index, QString fileName)
{
    Q_UNUSED(index);
    Q_UNUSED(fileName);
}

void TPlaylistCore::exportAll(QString path)
{
    Q_UNUSED(path);
}

TMusicItem *TPlaylistCore::currentMusicItem()
{
    TPlaylistItem *playlistItem = currentPlaylistItem();
    if(!playlistItem)
        return NULL;

    return playlistItem->musicItem(mMusiclistIndex);
}

TTrackItem *TPlaylistCore::currentTrackItem()
{
    TMusicItem *musicItem = currentMusicItem();
    if(!musicItem)
        return NULL;

    return musicItem->trackItem(mTracklistIndex);
}

TMusicItem *TPlaylistCore::parse(QString file)
{
    TMusicItem *musicItem = new TMusicItem;
    TMusicInfo *musicInfo = new TMusicInfo;
    QFileInfo fi(file);
    QString fileBaseName = fi.baseName();
    musicInfo->musicName = fileBaseName.toStdWString();
    musicItem->setOriginalName(fileBaseName);
    musicItem->setDisplayName(fileBaseName);
    musicItem->setFileName(file);
    TBackendPlugin *plugin = mBackendPluginManager->parse(file, musicInfo);
    if(plugin)
    {
        // If this file is successful parsed, bind it to music item.
        musicItem->setDisplayName(musicInfo->musicName.c_str());
        musicItem->setOriginalName(musicItem->displayName());
        musicItem->setArtist(musicInfo->artist.c_str());
        musicItem->setYear(QString::number(musicInfo->year));
        musicItem->setSystem(musicInfo->system.c_str());
        musicItem->setAdditionalInfo(musicInfo->additionalInfo.c_str());
        musicItem->setDuration(musicInfo->duration);
        foreach (TTrackInfo *track, musicInfo->trackList) {
            TTrackItem *trackItem = new TTrackItem;
            trackItem->fileName = file;
            trackItem->displayName = track->trackName.c_str();
            trackItem->originalName = trackItem->displayName;
            trackItem->artist = track->artist.c_str();
            trackItem->year = QString::number(track->year);
            trackItem->system = track->system.c_str();
            trackItem->additionalInfo = track->additionalInfo.c_str();
            trackItem->duration = track->duration;
            trackItem->originalDuration = trackItem->duration;
            trackItem->index = QString::number(track->index);
            trackItem->indexName = track->indexName.c_str();
            if(trackItem->index.isEmpty())
                trackItem->index = QString::number(track->index);
            musicItem->addTrackItem(trackItem);
            delete track;
        }
    }
    delete musicInfo;
    return musicItem;
}

TPlaylistItem *TPlaylistCore::playlistItem(int plIndex)
{
    if(plIndex<0 || plIndex>=mPlaylist.size())
        return NULL;

    return mPlaylist[plIndex];
}

TPlaylistItem *TPlaylistCore::takeAt(int plIndex)
{

}

void TPlaylistCore::insert(int pos, TPlaylistItem *item)
{

}

void TPlaylistCore::findPlaylist()
{

}

void TPlaylistCore::loadPlaylist(QString fileName)
{

}

void TPlaylistCore::save()
{

}

QString TPlaylistCore::getFileName()
{

}
