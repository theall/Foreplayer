#include "playlistcore.h"

#define PL_DIR                  "playlist"
#define PL_INDEX                "index.dat"
#define SEC_PLAYLIST            "playlist"
#define SEC_CURRENT_PLAYLIST    "currentPlaylist"
#define SEC_CURRENT_MUSIC       "currentMusic"
#define SEC_CURRENT_TRACK       "currentTrack"

TPlaylistCore::TPlaylistCore() :
    mPlaylistIndex(-1),
    mMusiclistIndex(-1),
    mTracklistIndex(-1),
    mFileSaving(false),
    mBackendPluginManager(TBackendPluginManager::instance())
{
    mCurrentDir.setPath(qApp->applicationDirPath());
    if(!mCurrentDir.exists(PL_DIR))
        mCurrentDir.mkdir(PL_DIR);

    mPlaylistDir.setPath(mCurrentDir.absoluteFilePath(PL_DIR));

    findPlaylist();
    if(mPlaylist.count()<=0)
        insert(tr("default"));
}

TPlaylistCore::~TPlaylistCore()
{
    save();
}

QStringList TPlaylistCore::names()
{
    QStringList sl;
    foreach (TPlaylistItem *item, mPlaylist) {
        sl.append(item->name());
    }

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
    if(index<0 || index>=mPlaylist.size())
        return;

    TPlaylistItem *playlistItem = mPlaylist[index];
    playlistItem->setDisplayName(newName);
}

void TPlaylistCore::sort(SortMode mode)
{
    if(mode==TITLE_ASC)
        std::sort(mPlaylist.begin(), mPlaylist.end(), [=](TPlaylistItem *a, TPlaylistItem *b){
            return a->name().toLower() > b->name().toLower();
        });
    else if(mode==TITLE_DES)
        std::sort(mPlaylist.begin(), mPlaylist.end(), [=](TPlaylistItem *a, TPlaylistItem *b){
            return a->name().toLower() < b->name().toLower();
        });
}

int TPlaylistCore::indexOf(TPlaylistItem *item)
{
    return mPlaylist.indexOf(item);
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

}

void TPlaylistCore::exportAll(QString path)
{

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
    TMusicItem *item = new TMusicItem;
    TMusicInfo *musicInfo = new TMusicInfo;
    QFileInfo fi(file);
    QString fileBaseName = fi.baseName();
    musicInfo->musicName = fileBaseName.toStdString();
    item->setDisplayName(fileBaseName);
    item->setFileName(file);
    TBackendPlugin *plugin = mBackendPluginManager->parse(file, musicInfo);
    if(plugin)
    {
        // If this file is successful parsed, bind it to music item.
        item->plugin = (void*)plugin;
        item->setDisplayName(musicInfo->musicName.c_str());
        item->setAdditionalInfo(musicInfo->additionalInfo.c_str());
        item->setDuration(musicInfo->duration);
        TTrackItems *items = item->trackItems();
        foreach (TTrackInfo *track, musicInfo->trackList) {
            TTrackItem *trackItem = new TTrackItem;
            trackItem->fileName = file;
            trackItem->displayName = track->trackName.c_str();
            trackItem->additionalInfo = track->additionalInfo.c_str();
            trackItem->duration = track->duration;
            trackItem->index = track->indexName.c_str();
            trackItem->musicFilePath = &trackItem->fileName;
            if(trackItem->index.isEmpty())
                trackItem->index = QString::number(track->index);
            items->append(trackItem);
            delete track;
        }
    }
    delete musicInfo;
    return item;
}

TPlaylistItem *TPlaylistCore::playlistItem(int plIndex)
{
    if(plIndex<0 || plIndex>=mPlaylist.size())
        return NULL;

    return mPlaylist[plIndex];
}

TPlaylistItem *TPlaylistCore::takeAt(int plIndex)
{
    if(plIndex<0 || plIndex>=mPlaylist.size())
        return NULL;

    return mPlaylist.takeAt(plIndex);
}

void TPlaylistCore::insert(int pos, TPlaylistItem *item)
{
    mPlaylist.insert(pos, item);
}

void TPlaylistCore::findPlaylist()
{
    QSettings indexFile(mPlaylistDir.absoluteFilePath(PL_INDEX), QSettings::IniFormat);
    QStringList fileNames = indexFile.value(SEC_PLAYLIST).toStringList();
    mPlaylistIndex = indexFile.value(SEC_CURRENT_PLAYLIST).toInt();
    mMusiclistIndex = indexFile.value(SEC_CURRENT_MUSIC).toInt();
    mTracklistIndex = indexFile.value(SEC_CURRENT_TRACK).toInt();
    for(QString fileName : fileNames)
    {
        loadPlaylist(mPlaylistDir.absoluteFilePath(fileName));
    }
    int size = mPlaylist.count();
    if(mPlaylistIndex<0 || mPlaylistIndex>=size)
    {
        mPlaylistIndex = 0;
        mMusiclistIndex = 0;
        mTracklistIndex = 0;
    } else {
        TPlaylistItem *playlistItem = mPlaylist[mPlaylistIndex];
        TMusicItem *musicItem = playlistItem->musicItem(mMusiclistIndex);
        if(!musicItem)
        {
            mMusiclistIndex = 0;
            mTracklistIndex = 0;
        } else {
            if(!musicItem->trackItem(mTracklistIndex))
                mTracklistIndex = 0;
        }
    }
}

void TPlaylistCore::loadPlaylist(QString fileName)
{
    QFile file(fileName);
    if(!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        //Return back.
        return;
    }

    QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    if(object.isEmpty())
        return;

    TPlaylistItem *playlistItem = new TPlaylistItem(fileName);
    playlistItem->fromJson(object);

    mPlaylist.append(playlistItem);
}

void TPlaylistCore::save()
{
    if(mFileSaving)
        return;

    mFileSaving = true;

    if(!mCurrentDir.exists(PL_DIR))
        mCurrentDir.mkdir(PL_DIR);

    QStringList playlist;
    foreach (TPlaylistItem *playlistItem, mPlaylist) {
        playlistItem->save();
        playlist.append(playlistItem->fileName());
    }
    QSettings indexFile(mPlaylistDir.absoluteFilePath(PL_INDEX), QSettings::IniFormat);
    indexFile.setValue(SEC_PLAYLIST, playlist);
    indexFile.setValue(SEC_CURRENT_PLAYLIST, mPlaylistIndex);
    indexFile.setValue(SEC_CURRENT_MUSIC, mMusiclistIndex);
    indexFile.setValue(SEC_CURRENT_TRACK, mTracklistIndex);
    indexFile.sync();
    mFileSaving = false;
}

QString TPlaylistCore::getFileName()
{
    QString fileName;
    int listSize = mPlaylist.count();
    for(int i=0;i<listSize+1;i++)
    {
        fileName = mPlaylistDir.absoluteFilePath(QString::number(i));
        bool existed = false;
        for(int j=0;j<listSize;j++)
        {
            if(mPlaylist[j]->fileName() == fileName)
            {
                existed = true;
                break;
            }
        }
        if(!existed)
            break;
    }

    Q_ASSERT(!fileName.isEmpty());

    return fileName;
}
