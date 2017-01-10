#include "playlistcore.h"

#define PL_DIR "playlist"
#define PL_INDEX "index.dat"
#define SECTION_PLAYLIST "playlist"
#define SECTION_CURRENT "current"

TPlaylistCore::TPlaylistCore() :
    mCurrentPlaylistIndex(-1),
    mFileSaving(false)
{
    mCurrentDir.setPath(qApp->applicationDirPath());
    if(mCurrentDir.exists(PL_DIR))
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
    if(mCurrentPlaylistIndex<0 || mCurrentPlaylistIndex>=mPlaylist.size())
        return NULL;

    return mPlaylist[mCurrentPlaylistIndex];
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

int TPlaylistCore::currentIndex()
{
    return mCurrentPlaylistIndex;
}

void TPlaylistCore::setCurrentIndex(int index)
{
    mCurrentPlaylistIndex = index;
}

void TPlaylistCore::exportAs(int index, QString fileName)
{

}

void TPlaylistCore::exportAll(QString path)
{

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
    QStringList fileNames = indexFile.value(SECTION_PLAYLIST).toStringList();
    mCurrentPlaylistIndex = indexFile.value(SECTION_CURRENT).toInt();
    for(QString fileName : fileNames)
    {
        loadPlaylist(mPlaylistDir.absoluteFilePath(fileName));
    }
    int size = mPlaylist.count();
    if(mCurrentPlaylistIndex<0 || mCurrentPlaylistIndex>=size)
        mCurrentPlaylistIndex = 0;
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
    indexFile.setValue(SECTION_PLAYLIST, playlist);
    indexFile.setValue(SECTION_CURRENT, mCurrentPlaylistIndex);
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
