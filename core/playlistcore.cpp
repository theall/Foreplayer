#include "playlistcore.h"

#define PL_DIR "playlist"

TPlaylistCore::TPlaylistCore() :
    mCurrentPlaylistIndex(-1),
    mCurrentMusiclistIndex(-1),
    mCurrentTracklistIndex(-1)
{
    findPlaylist(PL_DIR);

    if(mPlaylistMap.count()<=0)
        insertPlaylist(tr("default"));
}

TPlaylistCore::~TPlaylistCore()
{
    save();
}

QStringList TPlaylistCore::names()
{
    QStringList sl;
    foreach (TPlaylistItem *item, mPlaylist) {
        sl.append(item->displayName);
    }

    return sl;
}

TPlaylistItems *TPlaylistCore::playlists()
{
    return &mPlaylist;
}

void TPlaylistCore::insertPlaylist(QString name, int index)
{
    TPlaylistItem *playlistItem = new TPlaylistItem;
    playlistItem->displayName = name;
    playlistItem->fileName = getFileName();
    playlistItem->modified = true;

    if(index>-1)
        mPlaylist.insert(index, playlistItem);
    else
        mPlaylist.append(playlistItem);
}

void TPlaylistCore::removePlaylist(int index)
{
    if(index<0 || index>=mPlaylist.size())
        return;

    clearMusics(index);

    mPlaylistMap.remove(mPlaylist[index]);
    mPlaylist.removeAt(index);
}

void TPlaylistCore::renamePlaylist(int index, QString newName)
{
    if(index<0 || index>=mPlaylist.size())
        return;

    TPlaylistItem *playlistItem = mPlaylist[index];
    playlistItem->displayName = newName;
}

void TPlaylistCore::sortPlaylist(SortMode mode)
{

}

int TPlaylistCore::currentIndex(IndexType type)
{
    if(type==PlayList)
        return mCurrentPlaylistIndex;
    else if (type==TrackList)
        return mCurrentTracklistIndex;
    return mCurrentMusiclistIndex;
}

void TPlaylistCore::setCurrentIndex(IndexType type, int index)
{
    if(type==PlayList)
        mCurrentPlaylistIndex = index;
    else if (type==TrackList)
        mCurrentTracklistIndex = index;
    mCurrentMusiclistIndex = index;
}

void TPlaylistCore::exportAs(int index, QString fileName)
{

}

void TPlaylistCore::exportAll(QString path)
{

}

void TPlaylistCore::clearMusics(int playlistIndex)
{
    if(playlistIndex<0 || playlistIndex>=mPlaylist.size())
        return;

    TPlaylistItem *playlistItem = mPlaylist[playlistIndex];
    TMusicItems musicItems = mPlaylistMap.value(playlistItem);
    foreach (TMusicItem *item, musicItems) {
        delete item;
    }
    musicItems.clear();
}

void TPlaylistCore::findPlaylist(QString path)
{
    QDir dir(path);
    QStringList fileNames = dir.entryList(QDir::Files);
    for(QString fileName : fileNames)
    {
        loadPlaylist(fileName);
    }
    if(mPlaylistMap.count() > 0)
    {
        mCurrentPlaylistIndex = 0;
        if(mPlaylistMap.first().count() > 0)
        {
            mCurrentMusiclistIndex = 0;
            if(mMusiclistMap.first().count() > 0)
                mCurrentTracklistIndex = 0;
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

    TPlaylistItem *playlistItem = new TPlaylistItem;
    playlistItem->fileName = fileName;
    playlistItem->fromJson(object);

    mPlaylist.append(playlistItem);
}

void TPlaylistCore::save()
{
    foreach (TPlaylistItem *playlistItem, mPlaylist) {
        if(playlistItem->modified)
        {
            QFile file(playlistItem->fileName);

            if(!file.open(QIODevice::WriteOnly))
                continue;

            QJsonDocument playlistDocument;
            //Set the playlist object to document object.
            playlistDocument.setObject(playlistItem->toJson());
            //Write document data to the file.
            file.write(playlistDocument.toJson(QJsonDocument::Indented));
            //Close the file.
            file.close();
        }
    }
}

QString TPlaylistCore::getFileName()
{
    QString fileName;
    int i = mPlaylistMap.keys().size();
    while(true)
    {
        i++;
        fileName = getFullPath(QString::number(i));
        if(!QFile(fileName).exists())
            break;
    }

    Q_ASSERT(!fileName.isEmpty());

    return fileName;
}

QString TPlaylistCore::getFullPath(QString name)
{
    return qApp->applicationDirPath() + "/" + name;
}
