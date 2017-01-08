#include "playlistcore.h"

#define PL_DIR "playlist"
#define PL_INDEX "index.dat"

TPlaylistCore::TPlaylistCore() :
    mCurrentPlaylistIndex(-1),
    mCurrentMusiclistIndex(-1),
    mCurrentTracklistIndex(-1)
{
    mCurrentDir.setPath(qApp->applicationDirPath());
    if(mCurrentDir.exists(PL_DIR))
        mCurrentDir.mkdir(PL_DIR);

    mPlaylistDir.setPath(mCurrentDir.absoluteFilePath(PL_DIR));

    findPlaylist();
    if(mPlaylist.count()<=0)
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
        sl.append(item->name());
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
    playlistItem->setDisplayName(name);
    playlistItem->fileName = getFileName();

    if(index>-1)
        mPlaylist.insert(index, playlistItem);
    else
        mPlaylist.append(playlistItem);
}

bool TPlaylistCore::removePlaylist(int index)
{
    if(index<0 || index>=mPlaylist.size())
        return false;

    clearMusics(index);
    TPlaylistItem *item = mPlaylist.at(index);
    QFile file(item->fileName);
    bool status = file.remove();
    if(status)
        mPlaylist.removeAt(index);
    return status;
}

void TPlaylistCore::renamePlaylist(int index, QString newName)
{
    if(index<0 || index>=mPlaylist.size())
        return;

    TPlaylistItem *playlistItem = mPlaylist[index];
    playlistItem->setDisplayName(newName);
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
    foreach (TMusicItem *item, playlistItem->musics) {
        delete item;
    }
    playlistItem->musics.clear();
}

void TPlaylistCore::findPlaylist()
{
    QFile indexFile(mPlaylistDir.absoluteFilePath(PL_INDEX));
    if(!indexFile.open(QFile::ReadOnly))
        return;
    QString fileList = indexFile.readAll();

    QStringList fileNames = fileList.split("\r\n");
    for(QString fileName : fileNames)
    {
        loadPlaylist(mPlaylistDir.absoluteFilePath(fileName));
    }
    if(mPlaylist.count() > 0)
    {
        mCurrentPlaylistIndex = 0;
        TMusicItems musicItems = mPlaylist[0]->musics;
        if(musicItems.count() > 0)
        {
            mCurrentMusiclistIndex = 0;

            if(musicItems[0]->tracks.count() > 0)
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
    if(!mCurrentDir.exists(PL_DIR))
        mCurrentDir.mkdir(PL_DIR);

    QStringList playlist;
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
        playlist.append(playlistItem->fileName);
    }
    QFile file(mPlaylistDir.absoluteFilePath(PL_INDEX));
    if(!file.open(QFile::WriteOnly))
        return;

    QByteArray data = playlist.join("\r\n").toUtf8();
    file.write(data);
    file.close();
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
            if(mPlaylist[j]->fileName == fileName)
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
